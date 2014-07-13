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

void TestModelResult::addPredictionResult(double accuracy, UINT numTrainingSamples) {
	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Added Accuracy result for data set size: %d", numTrainingSamples);
	for (std::vector<ModelPredictionResult>::iterator it = predictionResults.begin() ; it != predictionResults.end(); ++it) {
		if (it->trainingDatasetSize == numTrainingSamples) {
			it->accuracyAvg += accuracy;
			it->totalTestNumber++;
			return;
		}
	}
	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "New prediction result");
	ModelPredictionResult result;
	result.accuracyAvg = accuracy;
	result.trainingDatasetSize = numTrainingSamples;
	result.totalTestNumber = 1;
	predictionResults.push_back(result);
}

void TestModelResult::addPredictionTimerResult(TestModelTimer predictTimer, UINT sampleSize, UINT numTrainingSamples) {
	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Added Preciction time result for data set size: %d", numTrainingSamples);
	for (std::vector<ModelPredictionResult>::iterator it = predictionResults.begin() ; it != predictionResults.end(); ++it) {
		if (it->trainingDatasetSize != numTrainingSamples) {
			continue;
		}
		for (std::vector<PredictionResultExecInfo>::iterator timeInfo = it->execTimeList.begin() ; timeInfo != it->execTimeList.end(); ++timeInfo) {
			if (timeInfo->SampleSize != sampleSize) {
				continue;
			}
			timeInfo->timerAvg += predictTimer;
			return;
		}
		PredictionResultExecInfo execInfo;
		execInfo.SampleSize = sampleSize;
		execInfo.timerAvg = predictTimer;
		it->execTimeList.push_back(execInfo);
		return;
	}

	PredictionResultExecInfo execInfo;
	execInfo.SampleSize = sampleSize;
	execInfo.timerAvg = predictTimer;

	ModelPredictionResult result;
	result.accuracyAvg = 0;
	result.trainingDatasetSize = numTrainingSamples;
	result.totalTestNumber = 0;
	result.execTimeList.push_back(execInfo);
	predictionResults.push_back(result);
}

void TestModelResult::printPredictionsResults() {

	const UINT printWidth = 20;

	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Printing prediction results.");

	cout << left << std::setw(printWidth) << "TDSize" << left << std::setw(printWidth) << "NumTests" << left << std::setw(printWidth)<< "AvgAcc";
	cout << left  << std::setw(printWidth) << "AvgSysTime" << left << std::setw(printWidth) << "AvgUserTime" << "\n";

	for (std::vector<ModelPredictionResult>::iterator it = predictionResults.begin() ; it != predictionResults.end(); ++it) {
		__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Printing prediction results for size: %d", it->trainingDatasetSize);

		cout << left << std::setw(printWidth) << it->trainingDatasetSize;

		double avgAccuracy = (double)it->accuracyAvg / (double)it->totalTestNumber;
		cout << left << std::setw(printWidth) << it->totalTestNumber << left << std::setw(printWidth) << setprecision(6) << avgAccuracy;

		TestModelTimer avgTimer;
		for (std::vector<PredictionResultExecInfo>::iterator timeInfo = it->execTimeList.begin() ; timeInfo != it->execTimeList.end(); ++timeInfo) {
			avgTimer += timeInfo->timerAvg;
		}

		double SysAvgTimeSec = (double)avgTimer.getSExecutionTime().executionTimeSec / (double)it->execTimeList.size();
		SysAvgTimeSec += (double)avgTimer.getSExecutionTime().executionTimeU / (double)(it->execTimeList.size() * 1000000);

		double UserAvgTimeSec = (double)avgTimer.getUExecutionTime().executionTimeSec / (double)it->execTimeList.size();
		UserAvgTimeSec += (double)avgTimer.getUExecutionTime().executionTimeU / (double)(it->execTimeList.size() * 1000000);

		cout << left  << std::setw(printWidth) << setprecision(6) << SysAvgTimeSec;
		cout << left  << std::setw(printWidth) << setprecision(6) << UserAvgTimeSec << "\n";
	}
}

void TestModelResult::printTrainingResults() {

	__android_log_print(ANDROID_LOG_VERBOSE, "GRT", "Printing training results");

	cout << left << std::setw(printWidth) << "TDSize" << std::setw(printWidth) << left << "AvgSysTime" << std::setw(printWidth) << left << "AvgSysTime" << endl;
	std::vector<ModelTraingResult>::iterator result = traingResults.begin();
	for (; result != traingResults.end(); ++result) {

		unsigned int totalSysTimeS = result->totalSysTime.executionTimeSec;
		double AvgSysTimeS = (double)totalSysTimeS / (double)(result->numTests);

		unsigned int totalSysTimeU = result->totalSysTime.executionTimeU;
		AvgSysTimeS += (double)totalSysTimeU / (double)(result->numTests * 1000000);


		totalSysTimeS = result->totalUserTime.executionTimeSec;
		double AvgUserTimeS = (double)totalSysTimeS / (double)(result->numTests);

		totalSysTimeU = result->totalUserTime.executionTimeU;
		AvgUserTimeS += (double)totalSysTimeU / (double)(result->numTests * 1000000);

		cout << std::setw(printWidth) << left << result->trainingDatasetSize;
		cout << std::setw(printWidth) << left << setprecision(6)  << AvgSysTimeS;
		cout << std::setw(printWidth) << left<< setprecision(6)  << AvgUserTimeS << endl;
	}
	cout << endl;
}

void TestModelResult::addMemoryUsageSample(MemoryUsageSample sample) {
	memoryUsageSamples.push_back(sample);
}

void TestModelResult::printMemoryUsageResult() {

	cout << left << std::setw(printWidth) << "TDSize" << std::setw(printWidth) << left << "startPhysMem" << std::setw(printWidth) << left << "startVirtMem";
	cout << left << std::setw(printWidth) << "endphysMem" << std::setw(printWidth) << left << "endVirtMem" << std::setw(printWidth) << left << "difPhysMem";
	cout << std::setw(printWidth) << left << "difVirtMem" << std::setw(printWidth) << left << "difPhysMemUsed" << std::setw(printWidth) << left << "difVirtMemUsed" << endl;

	std::vector<MemoryUsageSample>::iterator result = memoryUsageSamples.begin();
	for (; result != memoryUsageSamples.end(); ++result) {
		cout << std::setw(printWidth) << left << result->numTrainingSamples;

		cout << std::setw(printWidth) << left << result->endMemoryUsage.physMemUsedByProccess;
		cout << std::setw(printWidth) << left << result->endMemoryUsage.virtualMemUsedByProccess;

		cout << endl;

	}
}
