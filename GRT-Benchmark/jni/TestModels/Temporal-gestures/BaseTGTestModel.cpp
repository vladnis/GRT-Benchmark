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

	/* Generate Kfolds */
	KFolds = 10;
	if( !inputDataset.spiltDataIntoKFolds(KFolds, true) ) {
		throw TestModelException("BaseTGTestModel: Failed to spiltDataIntoKFolds!\n");
	}
}

void BaseTGTestModel::loadInputDataset() {
	/* Load input dataset */
	if (!inputDataset.loadDatasetFromFile(config->DatasetFilePath)) {
		throw TestModelException("BaseTGTestModel: Failed to load training data.");
	}
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

	for (GRT::UINT k = 0 ; k < this->KFolds; k++) {

		cout <<"Fold number: " << k << endl;
		char kFoldNum[100];
		sprintf(kFoldNum,"BaseTGTestModel: Starting testing for %d kFold.", k);
		__android_log_write(ANDROID_LOG_VERBOSE, "GRT", kFoldNum);

		/* Set up datasets for current fold */
		trainingDataset = inputDataset.getTrainingFoldData(k);
		this->setUpTrainingDataset();
		testDataset = inputDataset.getTestFoldData(k);
		this->setUpTestingDataset();

		char numInst[100];
		sprintf(numInst,"BaseTGTestModel: Number of instances: Training: %d; Testing: %d", trainingDataset.getNumSamples(), testDataset.getNumSamples());
		__android_log_write(ANDROID_LOG_VERBOSE, "GRT", numInst);

		/* Traing model */
		if(!model->train(trainingDataset)) {
			throw TestModelException("BaseTGTestModel:  Failed to train classifier.\n");
		}
		__android_log_write(ANDROID_LOG_VERBOSE, "GRT", "BaseTGTestModel: Model successfully trained.");

		cout << "TestSample\tClassLabel\t PredictedClassLabel\t MaximumLikelihood:" << endl;;
    	//Use the test dataset to test the HMM model
		double accuracy = 0;
		for(GRT::UINT i=0; i < testDataset.getNumSamples(); i++) {
			//Get the i'th test sample - this is a timeseries
			GRT::UINT classLabel = testDataset[i].getClassLabel();

			//Perform a prediction using the classifier
			if( !model->predict( testDataset[i].getData() ) ){
				throw TestModelException("BaseTGTestModel:  Failed to perform prediction for test sample");
			}

			//Get the predicted class label
			GRT::UINT predictedClassLabel = model->getPredictedClassLabel();
			double maximumLikelihood =  model->getMaximumLikelihood();
			GRT::VectorDouble classLikelihoods =  model->getClassLikelihoods();
			GRT::VectorDouble classDistances =  model->getClassDistances();

			//Update the accuracy
			if( classLabel == predictedClassLabel ) {
				accuracy++;
			}

			char floattNum[100];
			sprintf(floattNum,"%f", maximumLikelihood);
			cout << i  << "\t" << classLabel << "\t" <<  predictedClassLabel << "\t" << floattNum << endl;

			char datasetNum[100];
			sprintf(datasetNum,"BaseTGTestModel:  %d dataset tested.",i);
			__android_log_write(ANDROID_LOG_VERBOSE, "GRT", datasetNum);
		}
		__android_log_write(ANDROID_LOG_VERBOSE, "GRT", "BaseTGTestModel:  Model succesfully tested");

		char floattNum[100];
		sprintf(floattNum,"%f", accuracy/double(testDataset.getNumSamples())*100.0);
		cout << "Test Accuracy: " << floattNum << "%" << endl;
	}

	return true;
}
