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
    kFoldValue = 10;
}

KfoldTimeSeriesData::~KfoldTimeSeriesData() {}

bool KfoldTimeSeriesData::generateKFold(const GRT::UINT K, GRT::UINT window) {

    //K can not be zero
    if( K > inputDataset.getNumSamples()){
        std::cout << "spiltDataIntoKFolds(UINT K) - K can not be zero!" << std::endl;
        return false;
    }

    //K can not be larger than the number of examples
    if( K == 0){
        std::cout << "spiltDataIntoKFolds(UINT K,bool useStratifiedSampling) - K can not be larger than the total number of samples in the dataset!" << endl;
        return false;
    }

    //K can not be larger than the number of examples in a specific class if the stratified sampling option is true
	for(GRT::UINT c=0; c < inputDataset.getNumClasses(); c++) {
		if( K > inputDataset.classTracker[c].counter ){
			std::cout << "spiltDataIntoKFolds(UINT K,bool useStratifiedSampling) - K can not be larger than the number of samples in any given class!" << endl;
			return false;
		}
	}


    //Setup the dataset for k-fold cross validation
    kFoldValue = K;
    std::vector< GRT::UINT > indexs( inputDataset.getNumSamples() );

    //Work out how many samples are in each fold, the last fold might have more samples than the others
    GRT::UINT numSamplesPerFold = (GRT::UINT) floor( inputDataset.getNumSamples() / double(K) );

    //Resize the cross validation indexs buffer
    crossValidationIndexs.resize( K );

    //Create the random partion indexs
    GRT::Random random;
    GRT::UINT randomIndex = 0;

	//Break the data into seperate classes
	std::vector< std::vector< GRT::UINT > > classData( inputDataset.getNumClasses() );

	//Add the indexs to their respective classes
	for(GRT::UINT i = 0; i < inputDataset.getNumSamples(); i++) {
		GRT::UINT classLabel = inputDataset.getClassLabelIndexValue( data[i].getClassLabel() );
		classData[classLabel].push_back(i);
	}

	//Randomize the order of the indexs in each of the class index buffers
	for(GRT::UINT c = 0; c < inputDataset.getNumClasses(); c++){
		GRT::UINT numSamples = (GRT::UINT)classData[c].size();
		for(GRT::UINT x=0; x<numSamples; x++){
			//Pick a random index
			randomIndex = random.getRandomNumberInt(0,numSamples);

			//Swap the indexs
			GRT::SWAP( classData[c][ x ] , classData[c][ randomIndex ] );
		}
	}

	//Loop over each of the classes and add the data equally to each of the k folds until there is no data left
	std::vector< GRT::UINT >::iterator iter;
	for(GRT::UINT c=0; c< inputDataset.getNumClasses(); c++){
		iter = classData[ c ].begin();
		GRT::UINT k = 0;
		while( iter != classData[c].end() ){
			crossValidationIndexs[ k ].push_back( *iter );
			iter++;
			k++;
			k = k % K;
		}
	}


    crossValidationSetup = true;
    return true;

}
