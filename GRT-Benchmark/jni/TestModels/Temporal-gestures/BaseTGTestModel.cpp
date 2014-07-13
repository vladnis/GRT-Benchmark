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
	try {
		if (!inputDataset.loadDatasetFromFile(config->DatasetFilePath)) {
			throw TestModelException("BaseTGTestModel: Failed to load training data.");
		}
	} catch (...) {
		throw TestModelException("BaseTGTestModel: Failed to load training data.");
	}
}

void BaseTGTestModel::redirectOutput() {
	/* Output file for model file */
	sprintf(modelFile, "%s.GRTmodel", config->ResultFilePath);

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

	__android_log_print(ANDROID_LOG_WARN, "GRT", "Starting testing");

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

	__android_log_print(ANDROID_LOG_WARN, "GRT", "Testing ended");
	return true;
}

bool BaseTGTestModel::runTestWindow() {

	UINT maxTrainigSetSize = inputDataset.getNumSamples() * (KFolds - 1) / (KFolds * inputDataset.getNumClasses());
	__android_log_print(ANDROID_LOG_DEBUG, "GRT", "Max training size: %d", maxTrainigSetSize);

	// KFolds
	for (GRT::UINT k = 1 ; k < KFolds; k++) {

		__android_log_print(ANDROID_LOG_WARN, "GRT", "Running tests for: %d fold", k);
		//  maxTrainigSetSize
		for (UINT trainingSetSize = 1; trainingSetSize <= maxTrainigSetSize; trainingSetSize ++) {

			/* Set up training datasets for current fold */
			trainingDataset = kFoldTS->getTrainingFoldData(k, trainingSetSize);
			this->setUpTrainingDataset();

			/* Set up validation datasets for current fold */
			testDataset = kFoldTS->getTestFoldData(k);
			this->setUpTestingDataset();

			/* Log test dataset size */
			__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Data set size: training %d; testing %d",
					trainingDataset.getNumSamples(), testDataset.getNumSamples());

			/* Run test for current fold */
			runTestFold();
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

	/* Start timer */
	trainingTimer.start();


	if(!model->train(trainingDataset)) {
		throw TestModelException("BaseTGTestModel:  Failed to train classifier.\n");
	}

	/* Stop timer */
	trainingTimer.stop();

	/* Save training speed */
	ModelTraingResult trainingResult;
	addTrainingResult(trainingDataset.getNumSamples(), trainingTimer);

	model->saveModelToFile(modelFile);

	/* Start Memory usage sample */
	MemoryUsage startSample;
	startSample.takeSample();

	GRT::Classifier * aux_model = new GRT::Classifier();
	aux_model->loadModelFromFile(config->ResultFilePath);

	/* End Memory usage sample */
	MemoryUsage endSample;
	endSample.takeSample();

	delete(aux_model);

	/* Save Memory usage sample */
	MemoryUsageSample sample;
	sample.numTrainingSamples = trainingDataset.getNumSamples();
	sample.startMemoryUsage = startSample;
	sample.endMemoryUsage  = endSample;
	addMemoryUsageSample(sample);

	/* Used for saving test analysis */
	vector<PredictionResultExecInfo> testTimes;

	//Use the test dataset to test the model
	double numCorectPredictedTests = 0;
	for(GRT::UINT i = 0; i < testDataset.getNumSamples(); i++) {
		//Get the i'th test sample - this is a timeseries
		GRT::UINT classLabel = testDataset[i].getClassLabel();

		// Start timer for prediction
		TestModelTimer predictTimer;
		predictTimer.start();

		//Perform a prediction using the classifier
		if( !model->predict( testDataset[i].getData() ) ){
			throw TestModelException("BaseTGTestModel:  Failed to perform prediction for test sample");
		}
		predictTimer.stop();

		//Get the predicted class label
		/*
		GRT::UINT predictedClassLabel = model->getPredictedClassLabel();
		double maximumLikelihood =  model->getMaximumLikelihood();
		GRT::VectorDouble classLikelihoods =  model->getClassLikelihoods();
		GRT::VectorDouble classDistances =  model->getClassDistances();
		*/

		//Update the accuracy
		if( classLabel == model->getPredictedClassLabel() ) {
			numCorectPredictedTests++;
		}

		/* Save test Result */
		addPredictionTimerResult(predictTimer, testDataset[i].getLength(), trainingDataset.getNumSamples());
	}

	/* Calculate test accuracy for current fold */
	double accuracy = (double)(numCorectPredictedTests) / double(testDataset.getNumSamples())*100.0;
	__android_log_print(ANDROID_LOG_ERROR, "GRT", "Accuracy: %f", accuracy);
	/* Save tests results */
	addPredictionResult(accuracy, trainingDataset.getNumSamples());

	return true;
}

void BaseTGTestModel::setUpModel() {
	TestModelTimer timer;
	timer.start();
	_setUpModel();
	timer.stop();
}
