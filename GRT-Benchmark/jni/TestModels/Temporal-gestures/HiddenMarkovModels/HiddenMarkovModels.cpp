/*
 * HiddenMarkovModels.cpp
 *
 *  Created on: Jun 8, 2014
 *      Author: Vlad Nistorica
 */

#include "HiddenMarkovModels.h"

HiddenMarkovModels::HiddenMarkovModels(TestModelConfig *config)
	: BaseTGTestModel(config) {

	/* Initialize Quantizer */
	quantizer = NULL;
}

HiddenMarkovModels::~HiddenMarkovModels() {
	delete quantizer;
}

void HiddenMarkovModels::_setUpModel() {
	/* Setup model */
	model = new GRT::HMM();

	//Set the number of states in each model
	reinterpret_cast<GRT::HMM *>(model)->setNumStates( 4 );

	//Set the number of symbols in each model, this must match the number of symbols in the quantizer
	reinterpret_cast<GRT::HMM *>(model)->setNumSymbols( NUM_SYMBOLS );

	//Set the HMM model type to LEFTRIGHT with a delta of 1
	reinterpret_cast<GRT::HMM *>(model)->setModelType( GRT::HiddenMarkovModel::LEFTRIGHT );
	reinterpret_cast<GRT::HMM *>(model)->setDelta( 1 );

	//Set the training parameters
	reinterpret_cast<GRT::HMM *>(model)->setMinImprovement( 1.0e-5 );
	reinterpret_cast<GRT::HMM *>(model)->setMaxNumIterations( 100 );
	reinterpret_cast<GRT::HMM *>(model)->setNumRandomTrainingIterations( 20 );
}

void HiddenMarkovModels::setUpTrainingDataset() {
	/* Setup Quantizer */
	if (quantizer != NULL) {
		delete(quantizer);
	}
	quantizer = new GRT::KMeansQuantizer( trainingDataset.getNumDimensions(), NUM_SYMBOLS );

	//Train the quantizer using the training data
	if(!quantizer->train( trainingDataset ) ){
		throw TestModelException("HiddenMarkovModels: ERROR: Failed to train quantizer!");
	}

	//Quantize the training data
	GRT::LabelledTimeSeriesClassificationData quantizedTrainingData( 1 );

	for( GRT::UINT i=0; i<trainingDataset.getNumSamples(); i++){

		GRT::UINT classLabel = trainingDataset[i].getClassLabel();
		GRT::MatrixDouble quantizedSample;

		for(GRT::UINT j=0; j<trainingDataset[i].getLength(); j++) {
			quantizer->quantize( trainingDataset[i].getData().getRowVector(j) );
			quantizedSample.push_back( quantizer->getFeatureVector() );
		}

		if( !quantizedTrainingData.addSample(classLabel, quantizedSample) ){
			throw TestModelException("HiddenMarkovModels:  ERROR: Failed to quantize training data!");
		}

	}
	this->trainingDataset = quantizedTrainingData;
}

void HiddenMarkovModels::setUpTestingDataset() {
	//Quantize the test data
	GRT::LabelledTimeSeriesClassificationData quantizedTestData( 1 );

	for(GRT::UINT i=0; i < testDataset.getNumSamples(); i++){

		GRT::UINT classLabel = testDataset[i].getClassLabel();
		GRT::MatrixDouble quantizedSample;

		for(GRT::UINT j=0; j<testDataset[i].getLength(); j++){
			quantizer->quantize( testDataset[i].getData().getRowVector(j) );

			quantizedSample.push_back( quantizer->getFeatureVector() );
		}

		if( !quantizedTestData.addSample(classLabel, quantizedSample) ){
			throw TestModelException("HiddenMarkovModels:  ERROR: Failed to quantize training data!");
		}
	}

	this->testDataset = quantizedTestData;
}

