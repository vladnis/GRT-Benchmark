/*
 * KfoldTimeSeriesData.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Nistorica Vlad
 */

#include "KfoldTimeSeriesData.h"

KfoldTimeSeriesData::KfoldTimeSeriesData(GRT::LabelledTimeSeriesClassificationData inputDataset):
	inputDataset(inputDataset){
    crossValidationSetup = false;
    crossValidationIndexs.clear();
    kFoldValue = 0;
    classTracker = inputDataset.getClassTracker();
    numDimensions = inputDataset.getNumDimensions();
    UINT totalNumSamples = inputDataset.getNumSamples();
}

KfoldTimeSeriesData::~KfoldTimeSeriesData() {}

bool KfoldTimeSeriesData::spiltDataIntoKFolds(const GRT::UINT K) {

	kFoldValue = K;

    //K can not be zero
    if( K == 0 ){
        std::cout << "spiltDataIntoKFolds(UINT K) - K can not be zero!" << std::endl;
        return false;
    }

    //K can not be larger than the number of examples
    if( K  > inputDataset.getNumSamples()){
        std::cout << "spiltDataIntoKFolds(UINT K,bool useStratifiedSampling) - K can not be 0!" << std::endl;
        return false;
    }

    //K can not be larger than the number of examples in a specific class if the stratified sampling option is true
	for(UINT c=0; c < inputDataset.getNumClasses(); c++) {
		if( K > classTracker[c].counter ){
			cout << "spiltDataIntoKFolds(UINT K,bool useStratifiedSampling) - K can not be larger than the number of samples in any given class!" << std::endl;
			return false;
		}
	}


    //Setup the dataset for k-fold cross validation
    kFoldValue = K;
    vector< UINT > indexs( inputDataset.getNumSamples() );

    //Work out how many samples are in each fold, the last fold might have more samples than the others
    UINT numSamplesPerFold = (UINT) floor( inputDataset.getNumSamples() / double(K) );

    //Resize the cross validation indexs buffer
    crossValidationIndexs.resize( K );

    //Create the random partion indexs
    Random random;
    UINT randomIndex = 0;

	//Break the data into seperate classes
	vector< vector< UINT > > classData( inputDataset.getNumClasses() );

	//Add the indexs to their respective classes
	for(UINT i = 0; i < inputDataset.getNumSamples(); i++) {
		classData[ inputDataset.getClassLabelIndexValue(
				inputDataset[i].getClassLabel() ) ].push_back( i );
	}

	//Randomize the order of the indexs in each of the class index buffers
	for(UINT c = 0; c < inputDataset.getNumClasses(); c++) {
		UINT numSamples = (UINT)classData[c].size();
		for(UINT x = 0; x < numSamples; x++) {
			//Pick a random index
			randomIndex = random.getRandomNumberInt(0,numSamples);

			//Swap the indexs
			SWAP( classData[c][ x ] , classData[c][ randomIndex ] );
		}
	}

    //Loop over each of the classes and add the data equally to each of the k folds until there is no data left
    vector< UINT >::iterator iter;
    for(UINT c = 0; c < inputDataset.getNumClasses(); c++){
        iter = classData[ c ].begin();
        UINT k = 0;
        while( iter != classData[c].end() ){
            crossValidationIndexs[ k ].push_back( *iter );
            iter++;
            k = ++k % K;
        }
    }

    crossValidationSetup = true;
    return true;

}

LabelledTimeSeriesClassificationData KfoldTimeSeriesData::getTrainingFoldData(const UINT foldIndex, const UINT trainingDatasetSize) const {

    LabelledTimeSeriesClassificationData trainingData;

    if( !crossValidationSetup ) {
        cout << "getTrainingFoldData(UINT foldIndex) - Cross Validation has not been setup! You need to call the spiltDataIntoKFolds(UINT K,bool useStratifiedSampling) function first before calling this function!" << endl;
        return trainingData;
    }

    if( foldIndex >= kFoldValue ) {
    	cout << "Fold index too big" << endl;
    	return trainingData;
    }

    trainingData.setNumDimensions( numDimensions );
    //Add the data to the training set, this will consist of all the data that is NOT in the foldIndex
    UINT index = 0;
    for(UINT k=0; k < kFoldValue; k++) {
        if( k != foldIndex ){
            for(UINT i=0; i<crossValidationIndexs[k].size(); i++){

                index = crossValidationIndexs[k][i];
                /* Limit the number of samples in training dataset */
                if (trainingData.getNumSamples() >= trainingDatasetSize) {
                	return trainingData;
                }
                trainingData.addSample( inputDataset[ index ].getClassLabel(),
                		inputDataset[ index ].getData() );
            }
        }
    }

    return trainingData;
}

LabelledTimeSeriesClassificationData KfoldTimeSeriesData::getTestFoldData(const UINT foldIndex) const {
    LabelledTimeSeriesClassificationData testData;

    if( !crossValidationSetup ) {
        cout << "getTestFoldData(UINT foldIndex) - Cross Validation has not been setup! You need to call the spiltDataIntoKFolds(UINT K,bool useStratifiedSampling) function first before calling this function!" << endl;
        return testData;
    }

    if( foldIndex >= kFoldValue ) {
    	cout << "Fold index too big" << endl;
    	return testData;
    }

    //Add the data to the training
    testData.setNumDimensions( numDimensions );

    UINT index = 0;
    for(UINT i = 0; i < crossValidationIndexs[ foldIndex ].size(); i++) {

        index = crossValidationIndexs[ foldIndex ][i];
        testData.addSample( inputDataset[ index ].getClassLabel(),
        		inputDataset[ index ].getData() );
    }

    return testData;
}

UINT KfoldTimeSeriesData::getFoldSize() {
	if (crossValidationSetup) {
		UINT maxSize = crossValidationIndexs[0].size();
		for (UINT k = 0; k < kFoldValue; k++) {
			if (crossValidationIndexs[k].size() > maxSize) {
				maxSize = crossValidationIndexs[k].size();
			}
		}
		return inputDataset.getNumSamples() - maxSize;
	}
	return 0;
}
