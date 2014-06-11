/*
 * PipelineFactory.h
 *
 *  Created on: Jun 8, 2014
 *      Author: root
 */

#ifndef PIPELINEFACTORY_H_
#define PIPELINEFACTORY_H_

#include "TestModel.h"
#include <cstring>
#include <android/log.h>

class TestModelFactory {
public:
	static AbstractTestModel *generatePipeline(TestModelConfig *);
};

#endif /* PIPELINEFACTORY_H_ */
