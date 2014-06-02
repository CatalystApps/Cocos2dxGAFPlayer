#pragma once
#include "GAFShaderManager.h"

class GAFGlowFilterData;
class GAFDropShadowFilterData;

class GAFTextureEffectsConverter : public cocos2d::Ref
{
public:
    GAFTextureEffectsConverter();
    ~GAFTextureEffectsConverter();
    static GAFTextureEffectsConverter * sharedConverter();

    cocos2d::RenderTexture * gaussianBlurredTextureFromTexture(cocos2d::Texture2D * aTexture, const cocos2d::Rect& rect, float aBlurRadiusX, float aBlurRadiusY);
    static cocos2d::GLProgram * programForBlurShaderWithName(const char * aShaderName, GAFShaderManager::EVertexShader::Name aVertexShader, GAFShaderManager::EFragmentShader::Name aFragmentShader);

    static cocos2d::GLProgram * programForGlowShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile);
    cocos2d::RenderTexture * glowTextureFromTexture(cocos2d::Texture2D * aTexture, const cocos2d::Rect& rect, GAFGlowFilterData* data);

    cocos2d::RenderTexture*  dropShadowTextureFromTexture(cocos2d::Texture2D * aTexture, const cocos2d::Rect& rect, GAFDropShadowFilterData* data);
};

