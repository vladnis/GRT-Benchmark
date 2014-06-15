/*
 * TestModelTimer.h
 *
 *  Created on: Jun 11, 2014
 *      Author: Nistorica Vlad
 */

#ifndef TESTMODELTIMER_H_
#define TESTMODELTIMER_H_
#include <sys/time.h>
#include <sys/resource.h>
#include <android/log.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

struct TestModelTimerResults {
	unsigned long int executionTimeSec, executionTimeU;
};

class TestModelTimer {
private:
	struct rusage startUsage, endUsage;
	bool isStarted, isStoped;
	TestModelTimerResults userExecutionTime;
	TestModelTimerResults sysExecutionTime;

public:
	TestModelTimer();
	virtual ~TestModelTimer();
	void start();
	void stop();
	TestModelTimerResults getUExecutionTime();
	TestModelTimerResults getSExecutionTime();

	static TestModelTimerResults addTimerResults(TestModelTimerResults elem1, TestModelTimerResults elem2){
		unsigned long int sTime = elem1.executionTimeSec + elem2.executionTimeSec;
		unsigned long int uTime = elem1.executionTimeU + elem2.executionTimeU;
		if (uTime > 10000000) {
			sTime += uTime / 10000000;
			uTime = uTime % 10000000;
		}
		TestModelTimerResults result;
		result.executionTimeSec = sTime;
		result.executionTimeU = uTime;
		return result;
	}

	inline TestModelTimer *operator+=(TestModelTimer elem2) {
		TestModelTimer result;
		this->sysExecutionTime = TestModelTimer::addTimerResults(this->userExecutionTime, elem2.getSExecutionTime());
		this->userExecutionTime = TestModelTimer::addTimerResults(this->userExecutionTime, elem2.getUExecutionTime());
		return this;
	}

	TestModelTimer * operator= (TestModelTimer *source) {
		startUsage = source->startUsage;
		endUsage = source->endUsage;
		isStarted = source->isStarted;
		isStoped = source->isStoped;
		userExecutionTime = source->userExecutionTime;
		sysExecutionTime = source->sysExecutionTime;
		return this;
	}
};

#endif /* TESTMODELTIMER_H_ */
