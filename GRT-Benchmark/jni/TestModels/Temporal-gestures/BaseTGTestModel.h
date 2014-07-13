/*
 * BaseTGTestModel.h
 *
 *  Created on: Jun 8, 2014
 *      Author: Nistorica Vlad
 */

#ifndef BASETGTESTMODEL_H_
#define BASETGTESTMODEL_H_

#include <string.h>

#include "../Abstract/AbstractTestModel.h"

#include "../Structures/TestModelException.h"
#include "../Structures/TestModelTimer.h"
#include "../Structures/KfoldTimeSeriesData.h"
#include "../Structures/TestModelResult.h"

class BaseTGTestModel: public AbstractTestModel {

protected:
	GRT::LabelledTimeSeriesClassificationData trainingDataset;
	GRT::LabelledTimeSeriesClassificationData testDataset;
	GRT::Classifier *model;
	KfoldTimeSeriesData *kFoldTS;

private:
	GRT::LabelledTimeSeriesClassificationData inputDataset;
	std::ofstream *outStream, *errStream, *logStream;
	int KFolds;
	char modelFile[1000];

public:
	BaseTGTestModel(TestModelConfig *);
	virtual ~BaseTGTestModel();
	bool runTests();

protected:
	virtual void setUpTrainingDataset(void) = 0;
	virtual void setUpTestingDataset(void) = 0;
	virtual void setUpModel(void);
	virtual void _setUpModel(void) = 0;
	void setUpResults();

private:
	void redirectOutput();
	void loadInputDataset();
	bool runTestWindow();
	bool runTestFold();
};

#endif /* BASETGTESTMODEL_H_ */
