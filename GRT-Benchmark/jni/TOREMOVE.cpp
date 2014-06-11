#include <jni.h>


#include <prof.h>


/* Gesture Recognition Toolkit library */
#include "GRT.h"
using namespace GRT;

/* Asset manager */
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

/* Logging */
#include <android/log.h>

void anbcExample() {
	LabelledClassificationData trainingData, testData;

	if (!trainingData.loadDatasetFromFile("/sdcard/com.licenta.grt-benchmark.in")) {
		__android_log_write(ANDROID_LOG_ERROR, "GRT", "Training dataset could not be loaded.");
		return;
	}

	testData = trainingData.partition(80);

	GestureRecognitionPipeline pipeline;

	/* Select classifier */
	pipeline.setClassifier(ANBC());

	/* Train pipe width input dataset */
	if (!pipeline.train(trainingData)) {
		__android_log_write(ANDROID_LOG_ERROR, "GRT", "Failed to train.");
		return;
	}

	/* Test pipeline */
	if (!pipeline.test(testData)) {
		__android_log_write(ANDROID_LOG_ERROR, "GRT", "Failed to test.");
		return;
	}

	/* Output the results of the test */

	cout << "Test Accuracy: " << pipeline.getTestAccuracy() << endl;

	cout << "Precision: ";
	for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
		UINT classLabel = pipeline.getClassLabels()[k];
		cout << "\t" << pipeline.getTestPrecision(classLabel);
	}cout << endl;

	cout << "Recall: ";
	for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
		UINT classLabel = pipeline.getClassLabels()[k];
		cout << "\t" << pipeline.getTestRecall(classLabel);
	}cout << endl;

	cout << "FMeasure: ";
	for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
		UINT classLabel = pipeline.getClassLabels()[k];
		cout << "\t" << pipeline.getTestFMeasure(classLabel);
	}cout << endl;

	Matrix< double > confusionMatrix = pipeline.getTestConfusionMatrix();
	cout << "ConfusionMatrix: \n";
	for(UINT i=0; i<confusionMatrix.getNumRows(); i++){
		for(UINT j=0; j<confusionMatrix.getNumCols(); j++){
			cout << confusionMatrix[i][j] << "\t";
		}cout << endl;
	}

}
