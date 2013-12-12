#pragma once

#ifndef __GAF_BLURRED_SPRITE___
#define __GAF_BLURRED_SPRITE___


#include "cocoa/CCObject.h"
#include "cocoa/CCGeometry.h"

namespace cocos2d
{
	class CCTexture2D;
	class CCRenderTexture;
	class CCDictionary;
	class CCGLProgram;
}

using namespace cocos2d;

class GAFTextureEffectsConverter : public CCObject
{
public:
	GAFTextureEffectsConverter();
	~GAFTextureEffectsConverter();
	static GAFTextureEffectsConverter * sharedConverter();
	CCRenderTexture * gaussianBlurredTextureFromTexture(CCTexture2D * aTexture, const CCRect& rect, float aBlurRadiusX, float aBlurRadiusY);
	static CCGLProgram * programForBlurShaderWithName(const char * aShaderName, const char * aVertexShaderFile, const char * aFragmentShaderFile);
private:
	CCDictionary * _vertexShaderUniforms;
};


#endif // __GAF_BLURRED_SPRITE___
