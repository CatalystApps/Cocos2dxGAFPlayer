LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libgaftests_shared

LOCAL_MODULE_FILENAME := libgaftests

LOCAL_SRC_FILES := gaftests/main.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/BaseTest.cpp \
../../Classes/controller.cpp \
../../Classes/testBasic.cpp \
../../Classes/VisibleRect.cpp \
../../Classes/FiltersTest/FiltersTest.cpp \
../../Classes/FlipTest/FlipTest.cpp \
../../Classes/FramePlaybackTest/FramePlaybackTest.cpp \
../../Classes/MaskTimelineTest/MaskTimelineTest.cpp \
../../Classes/BundlesTest/BundlesTest.cpp \
../../Classes/MultipleTimelineTest/MultipleTimelineTest.cpp \
../../Classes/UITest/UITest.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../Library \
					$(LOCAL_PATH)/../../../Library/Sources

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += gafplayer_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,library)
