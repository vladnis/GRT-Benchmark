/*
 * AbstractPipeline.cpp
 *
 *  Created on: Jun 8, 2014
 *      Author: Vlad Nistorica
 */

#include "AbstractTestModel.h"

AbstractTestModel::AbstractTestModel(TestModelConfig *config): config(config) {
}

AbstractTestModel::~AbstractTestModel() {
	delete(config);
}
