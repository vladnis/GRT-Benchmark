/*
 * BaseTGTestModel.h
 *
 *  Created on: Jun 8, 2014
 *      Author: Nistorica Vlad
 */

#ifndef BASETGTESTMODEL_H_
#define BASETGTESTMODEL_H_

#include "../Abstract/AbstractTestModel.h"
#include <string.h>
#include "../Structures/TestModelException.h"

class BaseTGTestModel: public AbstractTestModel {

protected:
	GRT::LabelledTimeSeriesClassificationData trainingDataset;
	GRT::LabelledTimeSeriesClassificationData testDataset;
	GRT::Classifier *model;

private:
	GRT::LabelledTimeSeriesClassificationData inputDataset;
	std::ofstream *outStream, *errStream, *logStream;
	int KFolds;

public:
	BaseTGTestModel(TestModelConfig *);
	virtual ~BaseTGTestModel();
	bool runTests();

protected:
	virtual void setUpTrainingDataset(void) = 0;
	virtual void setUpTestingDataset(void) = 0;
	virtual void setUpModel(void) = 0;

private:
	void redirectOutput();
	void loadInputDataset();
};

#endif /* BASETGTESTMODEL_H_ */
