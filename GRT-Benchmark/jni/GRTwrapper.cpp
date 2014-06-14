#include <jni.h>

#include <prof.h>

/* Gesture Recognition Toolkit library */
#include "GRT.h"

/* Test Model Factory */
#include "TestModelFactory.h"

using namespace GRT;

/* Logging */
#include <android/log.h>

namespace grt_wrapper {

	/*
	 * Class:     com_example_grt_GRTexample
	 * Method:    GRTnative
	 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
	 */
	static void test(JNIEnv *env, jclass clazz, jstring jDatasetFilePath,
			jstring jResultFolderPath, jstring jUsecase, jstring jPipeline) {

		/* Build Test Model Config */
		TestModelConfig *config = new TestModelConfig(env, jDatasetFilePath, jResultFolderPath,jUsecase, jPipeline);

		/* Generate Test Model from Factory */
		AbstractTestModel *model = TestModelFactory::generatePipeline(config);
		try {
			if (model == NULL) {
				throw TestModelException("Can not identify test model.");
			}
			model->runTests();
			model->printTrainingResults();
			delete(model);
		} catch (exception& e) {
			__android_log_write(ANDROID_LOG_ERROR, "GRT", e.what());
			delete(model);
		}
	}

	static JNINativeMethod method_table[] = {
		{"test", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", (void *) test},
	};

}

using namespace grt_wrapper;

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
		__android_log_write(ANDROID_LOG_WARN, "JNI_OnLoad", "1");
    	return -1;
    }
    jclass clazz = env->FindClass("com/licenta/grt_benchmark/GRTwrapper");
    if (!clazz) {
		__android_log_write(ANDROID_LOG_WARN, "JNI_OnLoad", "Class com/licenta/grt_benchmark/GRTwrapper was not found!");
    	return -1;
    }
    env->RegisterNatives(clazz, method_table, sizeof(method_table) / sizeof(method_table[0]));
    env->DeleteLocalRef(clazz);

    return JNI_VERSION_1_6;
}
