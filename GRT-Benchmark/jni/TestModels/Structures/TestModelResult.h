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

using namespace GRT;
using namespace std;

struct testModelExecTime {
	UINT SampleSize;
	TestModelTimer timer;
};

struct ModelTestResult {
	UINT trainingDatasetSize;
	UINT testDatasetSize;
	UINT accuracy;
	vector<testModelExecTime> execTimeList;
};

struct ModelTraingResult {
	UINT trainingDatasetSize;
	UINT numTests;
	TestModelTimerResults totalSysTime, totalUserTime;
};

class TestModelResult {
private:
	vector< ModelTestResult > testResults;
	vector <ModelTraingResult> traingResults;

public:
	TestModelResult();
	virtual ~TestModelResult();
	virtual void setUpResults() = 0;

	void printPredictionsResults();
	void printTrainingResults();

	void addTrainingResult(unsigned int trainingNumSamples, TestModelTimer timer);
	void addPredictionResult(ModelTestResult);
};

#endif /* TESTMODELRESULT_H_ */
