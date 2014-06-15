/*
 * KfoldTimeSeriesData.h
 *
 *  Created on: Jun 12, 2014
 *      Author: Nistorica Vlad
 */

#ifndef KFOLDTIMESERIESDATA_H_
#define KFOLDTIMESERIESDATA_H_

#include "GRT.h"
#include <android/log.h>
using namespace std;
using namespace GRT;

class KfoldTimeSeriesData {
protected:
	LabelledTimeSeriesClassificationData inputDataset;
	vector< vector< vector< UINT > > > crossValidationIndexs;
	bool crossValidationSetup;
	UINT 	kFoldValue;
	UINT numDimensions;
	vector< ClassTracker > classTracker;

public:
	KfoldTimeSeriesData(LabelledTimeSeriesClassificationData);
	virtual ~KfoldTimeSeriesData();
	bool spiltDataIntoKFolds(const UINT);
	LabelledTimeSeriesClassificationData getTestFoldData(const UINT) const;
	LabelledTimeSeriesClassificationData getTrainingFoldData(const UINT,  const UINT) const;
	UINT getFoldSize();

};

#endif /* KFOLDTIMESERIESDATA_H_ */
