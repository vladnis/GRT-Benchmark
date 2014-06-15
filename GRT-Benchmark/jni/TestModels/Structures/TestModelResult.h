/*
 * TestModelResult.h
 *
 *  Created on: Jun 12, 2014
 *      Author: Nistorica Vlad
 */

#ifndef TESTMODELRESULT_H_
#define TESTMODELRESULT_H_

#include "GRT.h"

#include "TestModelTimer.h"
#include <iomanip>

using namespace GRT;
using namespace std;

struct PredictionResultExecInfo {
	UINT SampleSize;
	TestModelTimer timerAvg;
};

struct ModelPredictionResult {
	UINT trainingDatasetSize;
	double accuracyAvg;
	UINT totalTestNumber;
	vector<PredictionResultExecInfo> execTimeList;
};

struct ModelTraingResult {
	UINT trainingDatasetSize;
	UINT numTests;
	TestModelTimerResults totalSysTime, totalUserTime;
};

class TestModelResult {
private:
	vector< ModelPredictionResult > predictionResults;
	vector <ModelTraingResult> traingResults;

public:
	TestModelResult();
	virtual ~TestModelResult();
	virtual void setUpResults() = 0;

	void printPredictionsResults();
	void printTrainingResults();

	void addTrainingResult(unsigned int trainingNumSamples, TestModelTimer timer);
	void addPredictionResult(double, UINT);
	void addPredictionTimerResult(TestModelTimer, UINT, UINT);
};

#endif /* TESTMODELRESULT_H_ */
