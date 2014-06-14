/*
 * TestModelResult.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Nistorica Vlad
 */

#include "TestModelResult.h"

TestModelResult::TestModelResult() {
}

TestModelResult::~TestModelResult() {
}

void TestModelResult::addTrainingResult(unsigned int trainingNumSamples, TestModelTimer timer) {
	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Added Trainig result for data set size: %d", trainingNumSamples);

	std::vector<ModelTraingResult>::iterator result = traingResults.begin();
	for (; result != traingResults.end(); ++result) {
		if (result->trainingDatasetSize == trainingNumSamples) {
			result->totalSysTime = TestModelTimer::addTimerResults(result->totalSysTime, timer.getSExecutionTime());
			result->totalUserTime = TestModelTimer::addTimerResults(result->totalUserTime, timer.getUExecutionTime());
			result->numTests++;
			return;
		}
	}

	/* Add a new entry for current size */
	ModelTraingResult newResult;
	newResult.trainingDatasetSize = trainingNumSamples;
	newResult.totalSysTime = timer.getSExecutionTime();
	newResult.totalUserTime = timer.getUExecutionTime();
	newResult.numTests = 1;
	traingResults.push_back(newResult);
}

void TestModelResult::addPredictionResult(ModelTestResult result) {
	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Added Test result for data set size: %d", result.trainingDatasetSize);
	testResults.push_back(result);
}

void TestModelResult::printPredictionsResults() {
}

void TestModelResult::printTrainingResults() {

	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Printing training results");
	std::vector<ModelTraingResult>::iterator result = traingResults.begin();
	for (; result != traingResults.end(); ++result) {
		unsigned int totalSysTimeS = result->totalSysTime.executionTimeSec;
		double AvgSysTimeS = (double)totalSysTimeS / (double)(result->numTests);

		unsigned int totalSysTimeU = result->totalSysTime.executionTimeU;
		double AvgSysTimeU = (double)totalSysTimeU / (double)(result->numTests);

		cout << totalSysTimeS << "\t" << result->numTests;
		cout << result->trainingDatasetSize << "\t";
		cout << AvgSysTimeS << "." << AvgSysTimeU << "\t";


		totalSysTimeS = result->totalUserTime.executionTimeSec;
		AvgSysTimeS = (double)totalSysTimeS / (double)(result->numTests);

		totalSysTimeU = result->totalUserTime.executionTimeU;
		AvgSysTimeU = (double)totalSysTimeU / (double)(result->numTests);
		cout << AvgSysTimeS << "." << AvgSysTimeU << "\n";
	}
	cout << endl;
}
