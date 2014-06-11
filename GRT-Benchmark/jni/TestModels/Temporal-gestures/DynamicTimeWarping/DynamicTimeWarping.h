/*
 * DynamicTimeWarping.h
 *
 *  Created on: Jun 8, 2014
 *      Author: Vlad Nistorica
 */

#ifndef DYNAMICTIMEWARPING_H_
#define DYNAMICTIMEWARPING_H_

#include "../BaseTGTestModel.h"

class DynamicTimeWarping: public BaseTGTestModel {

public:
	DynamicTimeWarping(TestModelConfig *);
	virtual ~DynamicTimeWarping() {};

protected:
	void setUpTrainingDataset() {};
	void setUpTestingDataset() {};
	void setUpModel(void);
};

#endif /* DYNAMICTIMEWARPING_H_ */
