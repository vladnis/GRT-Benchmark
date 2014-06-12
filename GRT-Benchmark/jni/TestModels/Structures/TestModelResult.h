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

struct testModelTime {
	UINT SampleSize;
	TestModelTimer timer;
};

struct testModelResult {
	UINT trainingDatasetSize;
	UINT testDatasetSize;
	UINT accuracy;
	vector<testModelTime> timers;
};

struct modelTraingSpeed {
	UINT trainingDatasetSize;
	testModelTime timer;
};

class TestModelResult {
public:
	vector< testModelResult > TestModelResults;
	vector <modelTraingSpeed> ModelTraingSpeed;

public:
	TestModelResult();
	virtual ~TestModelResult();
	void printResults();
	virtual void setUpResults() = 0;
};

#endif /* TESTMODELRESULT_H_ */
