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
	uExecutionTime = NULL;
	sExecutionTime = NULL;
}

TestModelTimer::~TestModelTimer() {
	if (uExecutionTime != NULL) {
		free(uExecutionTime);
	}
	if (sExecutionTime != NULL) {
		free(sExecutionTime);
	}
}

void TestModelTimer::start() {
	isStarted = true;
	isStoped = false;
	if (isStoped) {
		if (uExecutionTime != NULL) {
			free(uExecutionTime);
		}
		if (sExecutionTime != NULL) {
			free(sExecutionTime);
		}
	}
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
	uExecutionTime = (char *) malloc(1000);
	sprintf(uExecutionTime, "%ld.%lds", uSecDif, uMicDif);

	long int sSecDif = endUsage.ru_stime.tv_sec - startUsage.ru_stime.tv_sec;
	long int sMicDif = endUsage.ru_stime.tv_usec - startUsage.ru_stime.tv_usec;
	while (sMicDif < 0) {
		sSecDif--;
		sMicDif += 1000000;
	}
	sExecutionTime = (char *) malloc(1000);
	sprintf(sExecutionTime, "%ld.%lds", sSecDif, sMicDif);
}

char *TestModelTimer::getUExecutionTime() {
	return uExecutionTime;
}

char *TestModelTimer::getSExecutionTime() {
	return sExecutionTime;
}

void TestModelTimer::printUExecutionTime() {
	std::cout << "User time:\t" << getUExecutionTime() << std::endl;
}

void TestModelTimer::printSExecutionTime() {
	std::cout << "System time:\t" << getSExecutionTime() << std::endl;
}

void TestModelTimer::printResults(std::string tag) {
	std::cout << "Timer Tag:\t" << tag << std::endl;
	printUExecutionTime();
	printSExecutionTime();
}

