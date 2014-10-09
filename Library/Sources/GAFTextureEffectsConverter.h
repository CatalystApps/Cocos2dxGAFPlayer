#pragma once

#ifndef __GAF_BLURRED_SPRITE___
#define __GAF_BLURRED_SPRITE___

namespace cocos2d
{
    class CCTexture2D;
    class CCRenderTexture;
    class CCGLProgram;
}

class GAFGlowFilterData;
class GAFDropShadowFilterData;

class GAFTextureEffectsConverter : public cocos2d::CCObject
{
public:
    GAFTextureEffectsConverter();
    ~GAFTextureEffectsConverter();
    static GAFTextureEffectsConverter * sharedConverter();

    cocos2d::CCRenderTexture * gaussianBlurredTextureFromTexture(cocos2d::CCTexture2D * aTexture, const cocos2d::CCRect& rect, float aBlurRadiusX, float aBlurRadiusY);
    static cocos2d::CCGLProgram * programForBlurShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile);

    static cocos2d::CCGLProgram * programForGlowShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile);
    cocos2d::CCRenderTexture * glowTextureFromTexture(cocos2d::CCTexture2D * aTexture, const cocos2d::CCRect& rect, GAFGlowFilterData* data);

    cocos2d::CCRenderTexture*  dropShadowTextureFromTexture(cocos2d::CCTexture2D * aTexture, const cocos2d::CCRect& rect, GAFDropShadowFilterData* data);
};


#endif // __GAF_BLURRED_SPRITE___
