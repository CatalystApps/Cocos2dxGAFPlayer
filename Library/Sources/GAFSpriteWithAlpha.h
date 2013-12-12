#pragma once

#ifndef __GAF_SPRITE_WITH_ALPHA__
#define __GAF_SPRITE_WITH_ALPHA__

#include "GAFSprite.h"

using namespace cocos2d;

#define kGAFSpriteWithAlphaShaderProgramCacheKey  "kGAFSpriteWithAlphaShaderProgramCache"

class GAFSpriteWithAlpha : public GAFSprite
{
public:
	GAFSpriteWithAlpha();
	~GAFSpriteWithAlpha();
	virtual void setUniformsForFragmentShader();
	virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated);
	static CCGLProgram * programForShader();
	void setColorTransform(const GLfloat * mults, const GLfloat * offsets);
	const GLfloat * getColorTransform() const
	{
		return _colorTransform;
	}
	void setColorTransform(const GLfloat * colorTransform);
	void setBlurRadius(const CCSize& blurRadius);
protected:
	void updateTextureWithEffects();
private:
	void _setBlendingFunc();
	GLfloat _colorTransform[8]; // 0-3 mults, 4-7 offsets
	CCSize   _blurRadius;
	CCTexture2D * _initialTexture;
	CCRect        _initialTextureRect;
};

#endif // __GAF_SPRITE_WITH_ALPHA__
