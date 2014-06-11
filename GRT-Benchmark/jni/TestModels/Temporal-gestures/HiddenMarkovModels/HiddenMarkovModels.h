/*
 * HiddenMarkovModels.h
 *
 *  Created on: Jun 8, 2014
 *      Author: Vlad Nistorica
 */

#ifndef HIDDENMARKOVMODELS_H_
#define HIDDENMARKOVMODELS_H_

#include "../BaseTGTestModel.h"
#include "../../Structures/TestModelException.h"

class HiddenMarkovModels: public BaseTGTestModel {
private:
	const static GRT::UINT NUM_SYMBOLS = 10;
	GRT::KMeansQuantizer *quantizer;

public:
	HiddenMarkovModels(TestModelConfig *);
	virtual ~HiddenMarkovModels();

protected:
	void setUpTrainingDataset();
	void setUpTestingDataset();
	void setUpModel(void);
};

#endif /* HIDDENMARKOVMODELS_H_ */
