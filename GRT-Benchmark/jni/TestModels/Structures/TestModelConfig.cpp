/*
 * TestModelConfig.cpp
 *
 *  Created on: Jun 8, 2014
 *      Author: Nistorica Vlad
 */

#include "TestModelConfig.h"

TestModelConfig::TestModelConfig(JNIEnv *env, jstring jDatasetFilePath,
		jstring jResultFolderPath, jstring jUsecase, jstring jPipeline):
		env(env),
		jDatasetFilePath(jDatasetFilePath),
		jResultFolderPath(jResultFolderPath),
		jPipeline(jPipeline),
		jUsecase(jUsecase) {

	DatasetFilePath = env->GetStringUTFChars(jDatasetFilePath, JNI_FALSE);
	Usecase = env->GetStringUTFChars(jUsecase, JNI_FALSE);
	ResultFilePath = env->GetStringUTFChars(jResultFolderPath, JNI_FALSE);
	Pipeline = env->GetStringUTFChars(jPipeline, JNI_FALSE);
}

TestModelConfig::~TestModelConfig() {
	env->ReleaseStringUTFChars(jDatasetFilePath, DatasetFilePath);
	env->ReleaseStringUTFChars(jUsecase, Usecase);
	env->ReleaseStringUTFChars(jResultFolderPath, ResultFilePath);
	env->ReleaseStringUTFChars(jPipeline, Pipeline);
}
