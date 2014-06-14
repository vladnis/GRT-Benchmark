/*
 * BaseTGTestModel.cpp
 *
 *  Created on: Jun 8, 2014
 *      Author: Nistorica Vlad
 */

#include "BaseTGTestModel.h"

BaseTGTestModel::BaseTGTestModel(TestModelConfig *config)
	: AbstractTestModel(config){

	/* Redirect output */
	redirectOutput();
	__android_log_write(ANDROID_LOG_VERBOSE, "GRT", "Redirected output.");

	/* Load input dataset */
	loadInputDataset();
	__android_log_write(ANDROID_LOG_VERBOSE, "GRT", "BaseTGTestModel: Successfully loaded data set from file.");

	/* Setup Kfolds */
	KFolds = 4;
	kFoldTS = NULL;

	/* Set result outut */
	setUpResults();
}

void BaseTGTestModel::setUpResults() {
	// TODO: Delete
}

void BaseTGTestModel::loadInputDataset() {
	/* Load input dataset */
	if (!inputDataset.loadDatasetFromFile(config->DatasetFilePath)) {
		throw TestModelException("BaseTGTestModel: Failed to load training data.");
	}
}

void BaseTGTestModel::redirectOutput() {
	/* Redirect stdout to new file*/
	outStream = new std::ofstream(config->ResultFilePath);
	std::cout.rdbuf(outStream->rdbuf());
	if (outStream->fail()) {
		throw TestModelException("BaseTGTestModel: Could not redirect stdout.");
	}

	/* Redirect stderr to file */
	char errFileName[100];
	strcpy(errFileName, config->ResultFilePath);
	strcat(errFileName, ".err");
	errStream = new std::ofstream(errFileName);
	std::cerr.rdbuf(errStream->rdbuf());
	if (outStream->fail()) {
		throw TestModelException("BaseTGTestModel: Could not redirect stderr.");
	}

	/* Redirect stdlog to file */
	char logFileName[100];
	strcpy(logFileName, config->ResultFilePath);
	strcat(logFileName, ".log");
	logStream = new std::ofstream(errFileName);
	std::clog.rdbuf(errStream->rdbuf());
	if (outStream->fail()) {
		throw TestModelException("BaseTGTestModel: Could not redirect stdlog.");
	}
}

BaseTGTestModel::~BaseTGTestModel() {
	/* Close output and error stream */
	if (!outStream->fail()) {
		outStream->close();
	}
	if (errStream->fail()) {
		errStream->close();
	}
	if (logStream->fail()) {
		errStream->close();
	}
	__android_log_write(ANDROID_LOG_VERBOSE, "GRT", "BaseTGTestModel: Closed output and error stream.");
}

bool BaseTGTestModel::runTests(){

	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Starting testing");

	/* Setup model */
	setUpModel();

	/* Test if dataset has enought instances */
	for (UINT c = 0; c < inputDataset.getNumClasses(); c++) {
		if (inputDataset[c].getLength() < 2) {
			throw TestModelException("BaseTGTestModel: Input dataset is not big enough!");
			return false;
		}
	}

	/* Separate input dataset using KFold */
	kFoldTS  = new KfoldTimeSeriesData(inputDataset);
	if( !kFoldTS->spiltDataIntoKFolds(KFolds) ) {
		throw TestModelException("BaseTGTestModel: Failed to spiltDataIntoKFolds!");
	}

	/* Run tests for current window */
	runTestWindow();

	delete(kFoldTS);

	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Testing ended");
	return true;
}

bool BaseTGTestModel::runTestWindow() {
	UINT foldSize = kFoldTS->getFoldSize();

	for (GRT::UINT k = 1 ; k < KFolds; k++) {

		__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Running tests for: %d fold", k);

		UINT incrementSize = 1;

		// foldSize
		for (UINT trainingSetSize = 1; trainingSetSize < 20; trainingSetSize += incrementSize) {

			__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Running tests for: %d training set size", trainingSetSize);

			/* Set up training datasets for current fold */
			trainingDataset = kFoldTS->getTrainingFoldData(k, trainingSetSize);
			this->setUpTrainingDataset();

			/* Set up validation datasets for current fold */
			testDataset = kFoldTS->getTestFoldData(k);
			this->setUpTestingDataset();

			/* Run test for current fold */
			runTestFold();

			if ((trainingSetSize > 10) && (incrementSize < 6)) {
				incrementSize++;
			}
		}
	}
	return true;
}

bool BaseTGTestModel::runTestFold() {

	if (!(trainingDataset.getNumSamples() > 0) && !(testDataset.getNumSamples() > 0)) {
		return false;
	}

	/* Traing model */
	TestModelTimer trainingTimer;
	trainingTimer.start();
	if(!model->train(trainingDataset)) {
		throw TestModelException("BaseTGTestModel:  Failed to train classifier.\n");
	}
	trainingTimer.stop();

	/* Save training speed */
	ModelTraingResult trainingResult;
	addTrainingResult(trainingDataset.getNumSamples(), trainingTimer);

	/* Used for saving test analysis */
	vector<testModelExecTime> testTimes;

	//Use the test dataset to test the model
	double numCorectPredictedTests = 0;
	for(GRT::UINT i = 0; i < testDataset.getNumSamples(); i++) {
		//Get the i'th test sample - this is a timeseries
		GRT::UINT classLabel = testDataset[i].getClassLabel();

		TestModelTimer predictTimer;
		predictTimer.start();
		//Perform a prediction using the classifier
		if( !model->predict( testDataset[i].getData() ) ){
			throw TestModelException("BaseTGTestModel:  Failed to perform prediction for test sample");
		}
		predictTimer.stop();

		//Get the predicted class label
		GRT::UINT predictedClassLabel = model->getPredictedClassLabel();
		double maximumLikelihood =  model->getMaximumLikelihood();
		GRT::VectorDouble classLikelihoods =  model->getClassLikelihoods();
		GRT::VectorDouble classDistances =  model->getClassDistances();

		//Update the accuracy
		if( classLabel == predictedClassLabel ) {
			numCorectPredictedTests++;
		}

		/* Save test Result */
		testModelExecTime currentTestTime;
		currentTestTime.timer = predictTimer;
		currentTestTime.SampleSize = testDataset[i].getLength();
		testTimes.push_back(currentTestTime);
	}

	/* Calculate test accuracy for current fold */
	double accuracy = numCorectPredictedTests / double(testDataset.getNumSamples())*100.0;

	/* Save tests results */
	ModelTestResult testResult;
	testResult.accuracy = accuracy;
	testResult.execTimeList = testTimes;
	testResult.trainingDatasetSize = trainingDataset.getNumSamples();
	testResult.testDatasetSize = testDataset.getNumSamples();

	addPredictionResult(testResult);

	return true;
}

void BaseTGTestModel::setUpModel() {
	TestModelTimer timer;
	timer.start();
	_setUpModel();
	timer.stop();
}
