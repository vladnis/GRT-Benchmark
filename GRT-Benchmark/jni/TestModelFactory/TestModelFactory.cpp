/*
 * PipelineFactory.cpp
 *
 *  Created on: Jun 8, 2014
 *      Author: Vlad Nistorica
 */
#include "TestModelFactory.h"

AbstractTestModel *TestModelFactory::generatePipeline(TestModelConfig *config) {
	if (std::strcmp(config->Usecase, "Static-postures")) {
	}
	if (std::strcmp(config->Usecase, "Temporal-gestures") == 0) {

		/* DTW */
		if (std::strcmp(config->Pipeline, "Dynamic-time-warping") == 0) {
			return new DynamicTimeWarping(config);
		}

		/* HMM */
		if (std::strcmp(config->Pipeline, "Hidden-Markov-Models") == 0) {
			return new HiddenMarkovModels(config);
		}
	}
	if (std::strcmp(config->Usecase, "Regression")) {
	}
	return NULL;
}
