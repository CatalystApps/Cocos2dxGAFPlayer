LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

CCX_ROOT := $(LOCAL_PATH)/../../..

LOCAL_MODULE := gafplayer_static

LOCAL_MODULE_FILENAME := libgafplayer

LOCAL_SRC_FILES := GAFPrecompiled.cpp \
Sources/GAFObject.cpp \
Sources/GAFMovieClip.cpp \
Sources/GAFMask.cpp \
Sources/GAFAssetTextureManager.cpp \
Sources/GAFTimeline.cpp \
Sources/GAFTimelineAction.cpp \
Sources/GAFTextField.cpp \
Sources/GAFTextData.cpp \
Sources/GAFAnimationFrame.cpp \
Sources/GAFAnimationSequence.cpp \
Sources/GAFAsset.cpp \
Sources/GAFFile.cpp \
Sources/GAFFilterData.cpp \
Sources/GAFLoader.cpp \
Sources/GAFShaderManager.cpp \
Sources/GAFQuadCommand.cpp \
Sources/GAFSprite.cpp \
Sources/GAFStream.cpp \
Sources/GAFSubobjectState.cpp \
Sources/GAFTextureAtlas.cpp \
Sources/GAFTextureAtlasElement.cpp \
Sources/GAFFilterManager.cpp \
Sources/GAFCachedTexture.cpp \
Sources/PrimitiveDeserializer.cpp \
Sources/TagDefineAnimationFrames.cpp \
Sources/TagDefineAnimationMasks.cpp \
Sources/TagDefineAnimationObjects.cpp \
Sources/TagDefineAtlas.cpp \
Sources/TagDefineNamedParts.cpp \
Sources/TagDefineSequences.cpp \
Sources/TagDefineStage.cpp \
Sources/TagDefineAnimationFrames2.cpp\
Sources/TagDefineTimeline.cpp \
Sources/TagDefineTextField.cpp 

LOCAL_C_INCLUDES := \
$(CCX_ROOT)/cocos \
$(CCX_ROOT)/cocos/2d \
$(CCX_ROOT)/cocos/platform/android \
$(CCX_ROOT)/cocos/math \
$(CCX_ROOT)/extensions \
$(LOCAL_PATH)/Sources

include $(BUILD_STATIC_LIBRARY)
