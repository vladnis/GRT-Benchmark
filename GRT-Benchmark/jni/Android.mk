LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := GRTprebuilt
LOCAL_SRC_FILES := GRTlib/libs/$(TARGET_ARCH_ABI)/libgrt.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/GRTlib/include

LOCAL_SHARED_LIBRARIES := gnustl_static
LOCAL_LDLIBS := -llog

include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := TestModel 

LOCAL_SRC_FILES := TestModels/Structures/TestModelConfig.cpp
LOCAL_SRC_FILES += TestModels/Structures/TestModelException.cpp

LOCAL_SRC_FILES += TestModels/Abstract/AbstractTestModel.cpp

LOCAL_SRC_FILES += TestModels/Temporal-gestures/BaseTGTestModel.cpp
LOCAL_SRC_FILES += TestModels/Temporal-gestures/DynamicTimeWarping/DynamicTimeWarping.cpp
LOCAL_SRC_FILES += TestModels/Temporal-gestures/HiddenMarkovModels/HiddenMarkovModels.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/TestModels
LOCAL_SHARED_LIBRARIES := GRTprebuilt
LOCAL_SHARED_LIBRARIES += gnustl_static
LOCAL_LDLIBS := -llog
LOCAL_CPP_FEATURES := exceptions

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := TestModelFactory
LOCAL_SRC_FILES := TestModelFactory/TestModelFactory.cpp
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/TestModelFactory
LOCAL_SHARED_LIBRARIES := TestModel
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := GRTwrapper
LOCAL_SRC_FILES := GRTwrapper.cpp

LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := GRTprebuilt TestModel TestModelFactory

include $(BUILD_SHARED_LIBRARY)