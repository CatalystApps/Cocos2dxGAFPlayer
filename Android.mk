LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := gafplayer_static

LOCAL_MODULE_FILENAME := libgafplayer

LOCAL_SRC_FILES := \
Library/Sources/GAFActionObject.cpp \
Library/Sources/GAFAnimatedObject.cpp \
Library/Sources/GAFAnimation.cpp \
Library/Sources/GAFAnimationFrame.cpp \
Library/Sources/GAFAnimationSequence.cpp \
Library/Sources/GAFAsset.cpp \
Library/Sources/GAFFile.cpp \
Library/Sources/GAFFilterData.cpp \
Library/Sources/GAFInteractionObject.cpp \
Library/Sources/GAFLoader.cpp \
Library/Sources/GAFPrecompiled.cpp \
Library/Sources/GAFShaderManager.cpp \
Library/Sources/GAFSprite.cpp \
Library/Sources/GAFSpriteWithAlpha.cpp \
Library/Sources/GAFStencilMaskSprite.cpp \
Library/Sources/GAFStream.cpp \
Library/Sources/GAFSubobjectState.cpp \
Library/Sources/GAFTextureAtlas.cpp \
Library/Sources/GAFTextureAtlasElement.cpp \
Library/Sources/GAFTextureEffectsConverter.cpp \
Library/Sources/PrimitiveDeserializer.cpp \
Library/Sources/TagDefineAnimationFrames.cpp \
Library/Sources/TagDefineAnimationMasks.cpp \
Library/Sources/TagDefineAnimationObjects.cpp \
Library/Sources/TagDefineAtlas.cpp \
Library/Sources/TagDefineNamedParts.cpp \
Library/Sources/TagDefineSequences.cpp \
Library/Sources/TagDefineStage.cpp \
Library/Sources/TagDefineTimeline.cpp \
Library/Sources/TagDefineAnimationFrames2.cpp

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../cocos2dx/include \
$(LOCAL_PATH)/../../cocos2dx \
$(LOCAL_PATH)/../../cocos2dx/platform/android \
$(LOCAL_PATH)/../../cocos2dx/kazmath/include \
$(LOCAL_PATH)/../../extensions \
$(LOCAL_PATH)/Library/Sources

include $(BUILD_STATIC_LIBRARY)
