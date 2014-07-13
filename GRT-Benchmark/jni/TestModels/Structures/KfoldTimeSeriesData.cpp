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
			randomIndex = random.getRandomNumberInt(0, numSamples);

			//Swap the indexs
			SWAP( classData[c][ x ] , classData[c][ randomIndex ] );
		}
	}


    //Resize the cross validation indexs buffer
    crossValidationIndexs.resize( K );
    for (UINT k = 0; k < K; k++) {
    	crossValidationIndexs[k].resize(inputDataset.getNumClasses());
    }

    //Loop over each of the classes and add the data equally to each of the k folds until there is no data left
    vector< UINT >::iterator iter;
    for(UINT c = 0; c < inputDataset.getNumClasses(); c++){
        iter = classData[ c ].begin();
        UINT k = 0;
        while( iter != classData[c].end() ){
            crossValidationIndexs[ k ][c].push_back( *iter );
            iter++;
            k = ++k % K;
        }
    }

    crossValidationSetup = true;
    return true;

}

LabelledTimeSeriesClassificationData KfoldTimeSeriesData::getTrainingFoldData(const UINT foldIndex, const UINT numSamplesPerClass) const {

    UINT index = 0;
	unsigned int randomNumber;
	unsigned int indexClassLabel;
	unsigned int numSamplesRemaining;
    LabelledTimeSeriesClassificationData trainingData;

    if( !crossValidationSetup ) {
        cout << "getTrainingFoldData(UINT foldIndex) - Cross Validation has not been setup! You need to call the spiltDataIntoKFolds(UINT K,bool useStratifiedSampling) function first before calling this function!" << endl;
        return trainingData;
    }

    if( foldIndex >= kFoldValue ) {
    	cout << "Fold index too big" << endl;
    	return trainingData;
    }

    Random random;

    trainingData.setNumDimensions( numDimensions );

    /* Put all K-1 training folds in one data set */
    vector <vector< UINT > >  MergedIndexs(inputDataset.getNumClasses());
    for(UINT k = 0; k < kFoldValue; k++) {
           if( k == foldIndex ) {
           	continue;
           }
           for (UINT classLabel = 0 ; classLabel < crossValidationIndexs[k].size(); classLabel++) {
        	   for (UINT i = 0; i < crossValidationIndexs[k][classLabel].size(); i++) {
        		   MergedIndexs[classLabel].push_back(crossValidationIndexs[k][classLabel][i]);
        	   }
           }
    }

	/* For each class peak randomly "numSamplesPerClass" samples */
	for (unsigned int classLabel = 0; classLabel < inputDataset.getNumClasses() ; classLabel++) {

		for (unsigned int numSamples = 1; numSamples <= numSamplesPerClass; numSamples++) {

			numSamplesRemaining = MergedIndexs[classLabel].size();
			if (numSamplesRemaining == 0) {
				__android_log_print(ANDROID_LOG_ERROR, "GRT", "The \"numSamplesPerClass\" variable is bigger that the samples for this class");
				break;
			}
			randomNumber = random.getRandomNumberInt(0, numSamplesRemaining);
			index = MergedIndexs[classLabel][randomNumber];

			/* Remove added sample so that it is not added again */
			MergedIndexs[classLabel].erase(MergedIndexs[classLabel].begin() + randomNumber);

			trainingData.addSample( inputDataset[ index ].getClassLabel(),
					inputDataset[ index ].getData() );
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
    for(UINT classLabel = 0; classLabel < crossValidationIndexs[foldIndex].size(); classLabel++) {
    	for (UINT i = 0; i <  crossValidationIndexs[foldIndex][classLabel].size(); i++) {
			index = crossValidationIndexs[foldIndex][classLabel][i];
			testData.addSample( inputDataset[ index ].getClassLabel(),
					inputDataset[ index ].getData() );
    	}
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
