#pragma once

#ifndef __GAF_TEXTURE_ATLAS___
#define __GAF_TEXTURE_ATLAS___

#include "cocoa/CCObject.h"

namespace cocos2d
{
	class CCImage;
	class CCArray;
	class CCTexture2D;
	class CCDictionary;
}
using namespace cocos2d;

class GAFTextureAtlas : public CCObject
{
public:
	~GAFTextureAtlas();
	
	static GAFTextureAtlas * create(const char * aTexturesDirectory, CCDictionary * aTextureAtlasConfigDictionary);
	bool init(const char * aTexturesDirectory, CCDictionary * aTextureAtlasConfigDictionary);
	
	inline bool loaded() const
	{
		return _loaded;
	}
	
	CCImage     * image();
	CCArray     * images();
	
	CCTexture2D * texture();
	CCArray     * textures();
	
	CCDictionary * elements();
	
	bool loadElementsFromAnimationConfigDictionary(CCDictionary * aConfigDictionary);
protected:
	GAFTextureAtlas();
private:
	bool           _loaded;
	CCArray      *  _images;
	CCArray      *  _textures;
	CCDictionary * _elements;
};

#endif // __GAF_TEXTURE_ATLAS___
