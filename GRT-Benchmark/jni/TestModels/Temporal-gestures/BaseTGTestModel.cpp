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
	originalKFolds = 10;
}

void BaseTGTestModel::loadInputDataset() {
	/* Load input dataset */
	if (!inputDataset.loadDatasetFromFile(config->DatasetFilePath)) {
		throw TestModelException("BaseTGTestModel: Failed to load training data.");
	}
	originalInputDataset = inputDataset;
	__android_log_write(ANDROID_LOG_VERBOSE, "GRT data from:", config->DatasetFilePath);
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
	/* Setup model */
	setUpModel();
	__android_log_write(ANDROID_LOG_VERBOSE, "GRT", "BaseTGTestModel: Model successfully set up.");

	for (GRT::UINT windowNum = 1; windowNum <= 10; windowNum ++) {
		//int windowNum = partition / 10;
		cout << endl<< "** Window " << windowNum << " **" << endl;

		/* Set up input dataset */
		// inputDataset = originalInputDataset;
		// GRT::LabelledTimeSeriesClassificationData aux;
		// aux = inputDataset.partition(100 - partition, true);
		// inputDataset = aux;

		int numSamplesPerClass = inputDataset.getNumSamples() / inputDataset.getNumClasses();
		if (numSamplesPerClass < 2) {
			continue;
		}

		/* Set up number of Folds */
		// KFolds = originalKFolds;
		//if (numSamplesPerClass <= KFolds) {
		// KFolds = numSamplesPerClass - 1;
		//}
		KFolds = windowNum;

		/* Logging */
		char numInst[100];
		sprintf(numInst,"Number of input instances: %d; Number of classes: %d; KFolds: %d; Num Samples per class: %d",
				inputDataset.getNumSamples(), inputDataset.getNumClasses(), KFolds, numSamplesPerClass);
		cout << numInst << endl;
		__android_log_write(ANDROID_LOG_VERBOSE, "GRT", numInst);

		/* Separate input dataset using KFold */
		if( !inputDataset.spiltDataIntoKFolds(KFolds, true) ) {
			continue;
			// throw TestModelException("BaseTGTestModel: Failed to spiltDataIntoKFolds!\n");
		}

		/* Run tests for current window */
		runTestWindow();
	}

	return true;
}

bool BaseTGTestModel::runTestWindow() {

	for (GRT::UINT k = 0 ; k < this->KFolds; k++) {
		cout << "Fold number: " << k << endl;
		char kFoldNum[100];
		sprintf(kFoldNum,"BaseTGTestModel: Starting testing for %d kFold.", k);
		__android_log_write(ANDROID_LOG_VERBOSE, "GRT", kFoldNum);

		/* Set up training datasets for current fold */
		trainingDataset = inputDataset.getTrainingFoldData(k);
		this->setUpTrainingDataset();

		/* Set up validation datasets for current fold */
		testDataset = inputDataset.getTestFoldData(k);
		this->setUpTestingDataset();

		char numInst[100];
		sprintf(numInst,"BaseTGTestModel: Number of instances: Training: %d; Testing: %d", trainingDataset.getNumSamples(), testDataset.getNumSamples());
		cout << numInst << endl;
		__android_log_write(ANDROID_LOG_VERBOSE, "GRT", numInst);

		if ((trainingDataset.getNumSamples() > 0) && (testDataset.getNumSamples() > 0)) {
			runTestFold();
		}
	}

	return true;
}

bool BaseTGTestModel::runTestFold() {
	/* Traing model */
	TestModelTimer trainingTimer;
	trainingTimer.start();
	if(!model->train(trainingDataset)) {
		throw TestModelException("BaseTGTestModel:  Failed to train classifier.\n");
	}
	trainingTimer.stop();
	trainingTimer.printResults("Training Model");
	__android_log_write(ANDROID_LOG_VERBOSE, "GRT", "BaseTGTestModel: Model successfully trained.");

	cout << "TestSample\tClassLabel\t PredictedClassLabel\t MaximumLikelihood:\tsExecTime\tuExecTime" << endl;;

	//Use the test dataset to test the model
	double accuracy = 0;
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
			accuracy++;
		}

		char maximumLikelihoodS[100];
		sprintf(maximumLikelihoodS,"%f", maximumLikelihood);
		cout << i  << "\t" << classLabel << "\t";
		cout << predictedClassLabel << "\t" << maximumLikelihoodS << "\t";
		cout << predictTimer.getSExecutionTime() << "\t" << predictTimer.getUExecutionTime() << endl;

		char datasetNum[100];
		sprintf(datasetNum,"BaseTGTestModel:  %d dataset tested.",i);
		__android_log_write(ANDROID_LOG_VERBOSE, "GRT", datasetNum);
	}
	__android_log_write(ANDROID_LOG_VERBOSE, "GRT", "BaseTGTestModel:  Model succesfully tested");

	/* Calculate test accuracy for current fold */
	/* Accuracy = number of test instances correctly predicted / total number of test intances */
	char floattNum[100];
	sprintf(floattNum,"%f", accuracy/double(testDataset.getNumSamples())*100.0);
	cout << "Test Accuracy: " << floattNum << "%" << endl << endl;

	return true;
}

void BaseTGTestModel::setUpModel() {
	TestModelTimer timer;
	timer.start();
	_setUpModel();
	timer.stop();
	timer.printResults("Setup model.");
}
