LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

CCX_ROOT := $(LOCAL_PATH)/../../..
GAF_LIB_SOURCES := ./Sources

LOCAL_MODULE := gafplayer_static

LOCAL_MODULE_FILENAME := libgafplayer

LOCAL_SRC_FILES := $(GAF_LIB_SOURCES)/GAFPrecompiled.cpp \
$(GAF_LIB_SOURCES)/GAFObject.cpp \
$(GAF_LIB_SOURCES)/GAFMovieClip.cpp \
$(GAF_LIB_SOURCES)/GAFMask.cpp \
$(GAF_LIB_SOURCES)/GAFAssetTextureManager.cpp \
$(GAF_LIB_SOURCES)/GAFTimeline.cpp \
$(GAF_LIB_SOURCES)/GAFTimelineAction.cpp \
$(GAF_LIB_SOURCES)/GAFTextField.cpp \
$(GAF_LIB_SOURCES)/GAFTextData.cpp \
$(GAF_LIB_SOURCES)/GAFAnimationFrame.cpp \
$(GAF_LIB_SOURCES)/GAFAnimationSequence.cpp \
$(GAF_LIB_SOURCES)/GAFAsset.cpp \
$(GAF_LIB_SOURCES)/GAFFile.cpp \
$(GAF_LIB_SOURCES)/GAFFilterData.cpp \
$(GAF_LIB_SOURCES)/GAFLoader.cpp \
$(GAF_LIB_SOURCES)/GAFShaderManager.cpp \
$(GAF_LIB_SOURCES)/GAFQuadCommand.cpp \
$(GAF_LIB_SOURCES)/GAFSprite.cpp \
$(GAF_LIB_SOURCES)/GAFStream.cpp \
$(GAF_LIB_SOURCES)/GAFSubobjectState.cpp \
$(GAF_LIB_SOURCES)/GAFTextureAtlas.cpp \
$(GAF_LIB_SOURCES)/GAFTextureAtlasElement.cpp \
$(GAF_LIB_SOURCES)/GAFFilterManager.cpp \
$(GAF_LIB_SOURCES)/GAFCachedTexture.cpp \
$(GAF_LIB_SOURCES)/PrimitiveDeserializer.cpp \
$(GAF_LIB_SOURCES)/TagDefineAnimationFrames.cpp \
$(GAF_LIB_SOURCES)/TagDefineAnimationMasks.cpp \
$(GAF_LIB_SOURCES)/TagDefineAnimationObjects.cpp \
$(GAF_LIB_SOURCES)/TagDefineAtlas.cpp \
$(GAF_LIB_SOURCES)/TagDefineNamedParts.cpp \
$(GAF_LIB_SOURCES)/TagDefineSequences.cpp \
$(GAF_LIB_SOURCES)/TagDefineStage.cpp \
$(GAF_LIB_SOURCES)/TagDefineAnimationFrames2.cpp\
$(GAF_LIB_SOURCES)/TagDefineTimeline.cpp \
$(GAF_LIB_SOURCES)/TagDefineTextField.cpp 

LOCAL_C_INCLUDES := \
$(CCX_ROOT)/cocos \
$(CCX_ROOT)/cocos/2d \
$(CCX_ROOT)/cocos/platform/android \
$(CCX_ROOT)/cocos/math \
$(CCX_ROOT)/extensions \
$(GAF_LIB_SOURCES)

include $(BUILD_STATIC_LIBRARY)
