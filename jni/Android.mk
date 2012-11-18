LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#LOCAL_JNI_SHARED_LIBRARIES := libbluetooth_jni
#LOCAL_JAVA_LIBRARIES := javax.obex
#LOCAL_STATIC_JAVA_LIBRARIES := com.android.vcard

LOCAL_SRC_FILES := \
	bluedroid-test.c

LOCAL_SHARED_LIBRARIES := \
	libandroid_runtime \
	libnativehelper \
	libcutils \
	libutils \
	libhardware

LOCAL_MODULE := bluedroid-test
LOCAL_MODULE_TAGS := optional
LOCAL_REQUIRED_MODULES := bluetooth.default

LOCAL_PROGUARD_ENABLED := disabled

include $(BUILD_EXECUTABLE)
