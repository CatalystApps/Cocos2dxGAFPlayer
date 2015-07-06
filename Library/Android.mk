LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

CCX_ROOT := $(LOCAL_PATH)/../../..
CCX_ROOT_2 := $(LOCAL_PATH)/../../cocos2d-x
GAF_LIB_SOURCES := $(LOCAL_PATH)/Sources

LOCAL_MODULE := gafplayer_static

LOCAL_MODULE_FILENAME := libgafplayer

CLASSES_FILES := $(wildcard $(GAF_LIB_SOURCES)/*.cpp)
LOCAL_SRC_FILES := $(CLASSES_FILES:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := \
$(CCX_ROOT)/cocos \
$(CCX_ROOT)/cocos/platform/android \
$(CCX_ROOT)/external \
$(CCX_ROOT_2)/cocos \
$(CCX_ROOT_2)/cocos/platform/android \
$(CCX_ROOT_2)/external \
$(GAF_LIB_SOURCES)

include $(BUILD_STATIC_LIBRARY)
