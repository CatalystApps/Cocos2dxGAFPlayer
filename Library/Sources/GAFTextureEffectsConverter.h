#pragma once

#ifndef __GAF_BLURRED_SPRITE___
#define __GAF_BLURRED_SPRITE___

namespace cocos2d
{
    class CCTexture2D;
    class CCRenderTexture;
    class CCGLProgram;
}

using namespace cocos2d;

class GAFGlowFilterData;
class GAFDropShadowFilterData;

class GAFTextureEffectsConverter : public CCObject
{
public:
    GAFTextureEffectsConverter();
    ~GAFTextureEffectsConverter();
    static GAFTextureEffectsConverter * sharedConverter();

    CCRenderTexture * gaussianBlurredTextureFromTexture(CCTexture2D * aTexture, const CCRect& rect, float aBlurRadiusX, float aBlurRadiusY);
    static CCGLProgram * programForBlurShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile);

    static CCGLProgram * programForGlowShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile);
    CCRenderTexture * glowTextureFromTexture(CCTexture2D * aTexture, const CCRect& rect, GAFGlowFilterData* data);

    CCRenderTexture*  dropShadowTextureFromTexture(CCTexture2D * aTexture, const CCRect& rect, GAFDropShadowFilterData* data);
};


#endif // __GAF_BLURRED_SPRITE___
