/*
 * TestModelConfig.h
 *
 *  Created on: Jun 8, 2014
 *      Author: Nistorica Vlad
 */

#ifndef TESTMODELCONFIG_H_
#define TESTMODELCONFIG_H_

#include <jni.h>

class TestModelConfig {
public:
	const char *DatasetFilePath;
	const char *Usecase;
	const char *ResultFilePath;
	const char *Pipeline;

private:
	jstring jDatasetFilePath;
	jstring jUsecase;
	jstring jResultFolderPath;
	jstring jPipeline;
	JNIEnv *env;

public:
	TestModelConfig(JNIEnv *, jstring, jstring, jstring, jstring);
	virtual ~TestModelConfig();
};

#endif /* TESTMODELCONFIG_H_ */
