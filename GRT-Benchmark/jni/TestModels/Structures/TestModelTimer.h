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

class TestModelTimer {
private:
	struct rusage startUsage, endUsage;
	bool isStarted, isStoped;
	char *uExecutionTime , *sExecutionTime;

public:
	TestModelTimer();
	virtual ~TestModelTimer();
	void start();
	void stop();
	char *getUExecutionTime();
	char *getSExecutionTime();
	void printUExecutionTime();
	void printSExecutionTime();
	void printResults(std::string  tag);
};

#endif /* TESTMODELTIMER_H_ */
