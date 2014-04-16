LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := gafexample_shared

LOCAL_MODULE_FILENAME := libgafexample

LOCAL_SRC_FILES := gafexample/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GafFeatures.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../../../external/Cocos2dxGAFPlayer/Library/Sources

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static \
					gafplayer_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,external/Cocos2dxGAFPlayer)
