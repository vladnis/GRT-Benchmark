/*
 * TestModelTimer.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: Nistorica Vlad
 */

#include "TestModelTimer.h"

TestModelTimer::TestModelTimer() {
	isStarted = false;
	isStoped = false;
	userExecutionTime.executionTimeSec = 0;
	userExecutionTime.executionTimeU = 0;
	sysExecutionTime.executionTimeSec = 0;
	sysExecutionTime.executionTimeU = 0;
}

TestModelTimer::~TestModelTimer() {
}

void TestModelTimer::start() {
	isStarted = true;
	isStoped = false;
	getrusage(RUSAGE_SELF, &startUsage);
}

void TestModelTimer::stop() {
	if (!isStarted) {
		__android_log_write(ANDROID_LOG_ERROR, "GRT", "Timer was not started.");
		return;
	}
	isStoped = true;
	getrusage(RUSAGE_SELF, &endUsage);

	long int uSecDif = endUsage.ru_utime.tv_sec - startUsage.ru_utime.tv_sec;
	long int uMicDif = endUsage.ru_utime.tv_usec - startUsage.ru_utime.tv_usec;
	while (uMicDif < 0) {
		uSecDif--;
		uMicDif += 1000000;
	}
	userExecutionTime.executionTimeSec = uSecDif;
	userExecutionTime.executionTimeU = uMicDif;

	long int sSecDif = endUsage.ru_stime.tv_sec - startUsage.ru_stime.tv_sec;
	long int sMicDif = endUsage.ru_stime.tv_usec - startUsage.ru_stime.tv_usec;
	while (sMicDif < 0) {
		sSecDif--;
		sMicDif += 1000000;
	}
	sysExecutionTime.executionTimeSec = sSecDif;
	sysExecutionTime.executionTimeU = sMicDif;
}

TestModelTimerResults TestModelTimer::getUExecutionTime() {
	return userExecutionTime;
}

TestModelTimerResults TestModelTimer::getSExecutionTime() {
	return sysExecutionTime;
}

