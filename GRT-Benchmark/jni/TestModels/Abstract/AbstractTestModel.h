/*
 * AbstractPipeline.h
 *
 *  Created on: Jun 8, 2014
 *      Author: Vlad Nistorica
 */

#ifndef ABSTRACTPIPELINE_H_
#define ABSTRACTPIPELINE_H_

#include "../Structures/TestModelConfig.h"
#include "../Structures/TestModelResult.h"
#include <stddef.h>
#include "GRT.h"
#include <android/log.h>

class AbstractTestModel: public TestModelResult {
protected:
	TestModelConfig *config;

public:
	AbstractTestModel(TestModelConfig *);
	virtual ~AbstractTestModel();
	virtual bool runTests() = 0;
};

#endif /* ABSTRACTPIPELINE_H_ */
