LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := gafplayer_static

LOCAL_MODULE_FILENAME := libgafplayer

LOCAL_SRC_FILES := GAFPrecompiled.cpp \
Sources/GAFAnimatedObject.cpp \
Sources/GAFAnimation.cpp \
Sources/GAFAnimationFrame.cpp \
Sources/GAFAnimationSequence.cpp \
Sources/GAFAsset.cpp \
Sources/GAFFile.cpp \
Sources/GAFFilterData.cpp \
Sources/GAFLoader.cpp \
Sources/GAFShaderManager.cpp \
Sources/GAFSprite.cpp \
Sources/GAFSpriteWithAlpha.cpp \
Sources/GAFStencilMaskSprite.cpp \
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
Sources/TagDefineStage.cpp

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../cocos \
$(LOCAL_PATH)/../../../cocos/2d \
$(LOCAL_PATH)/../../../cocos/platform/android \
$(LOCAL_PATH)/../../../cocos/math \
$(LOCAL_PATH)/../../../extensions \
$(LOCAL_PATH)/Sources

include $(BUILD_STATIC_LIBRARY)
