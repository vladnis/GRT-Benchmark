/*
 * KfoldTimeSeriesData.h
 *
 *  Created on: Jun 12, 2014
 *      Author: Nistorica Vlad
 */

#ifndef KFOLDTIMESERIESDATA_H_
#define KFOLDTIMESERIESDATA_H_


class KfoldTimeSeriesData {
private:
	GRT::LabelledTimeSeriesClassificationData inputDataset;
	std::vector< std::vector< GRT::UINT > > crossValidationIndexs;
	bool crossValidationSetup;
	GRT::UINT 	kFoldValue;

public:
	KfoldTimeSeriesData(GRT::LabelledTimeSeriesClassificationData);
	virtual ~KfoldTimeSeriesData();
	bool generateKFold(const GRT::UINT, GRT::UINT);
};

#endif /* KFOLDTIMESERIESDATA_H_ */
