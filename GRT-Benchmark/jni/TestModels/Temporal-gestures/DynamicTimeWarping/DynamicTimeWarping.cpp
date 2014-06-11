/*
 * DynamicTimeWarping.cpp
 *
 *  Created on: Jun 8, 2014
 *      Author: Vlad Nistorica
 */

#include "DynamicTimeWarping.h"

DynamicTimeWarping::DynamicTimeWarping(TestModelConfig *config)
	: BaseTGTestModel(config) {
}

void DynamicTimeWarping::setUpModel() {
	model = new GRT::DTW();
	reinterpret_cast<GRT::DTW *>(model)->enableTrimTrainingData(true,0.1,90);
}
