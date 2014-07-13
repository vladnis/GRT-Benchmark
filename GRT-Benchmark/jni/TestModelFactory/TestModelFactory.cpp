/*
 * PipelineFactory.cpp
 *
 *  Created on: Jun 8, 2014
 *      Author: Vlad Nistorica
 */
#include "TestModelFactory.h"

AbstractTestModel *TestModelFactory::generatePipeline(TestModelConfig *config) {

	__android_log_print(ANDROID_LOG_DEBUG, "GRT", "%s" ,config->Usecase);

	if (std::strcmp(config->Usecase, "Segmented gestures") == 0) {

		/* DTW */
		if (std::strcmp(config->Pipeline, "Dynamic-time-warping") == 0) {
			return new DynamicTimeWarping(config);
		}

		/* HMM */
		if (std::strcmp(config->Pipeline, "Hidden-Markov-Models") == 0) {
			return new HiddenMarkovModels(config);
		}
	}
	return NULL;
}
