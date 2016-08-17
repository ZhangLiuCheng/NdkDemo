LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS :=-llog

LOCAL_MODULE := nativeUtils

LOCAL_SRC_FILES := NativeUtils.cpp

include $(BUILD_SHARED_LIBRARY)
