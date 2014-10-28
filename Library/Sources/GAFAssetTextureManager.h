#pragma once

#include "GAFTextureAtlas.h"

NS_GAF_BEGIN

class GAFAssetTextureManager : public cocos2d::Ref
{
public:
	GAFAssetTextureManager();
	~GAFAssetTextureManager();

	void					appendInfoFromTextureAtlas(GAFTextureAtlas* atlas);
	void					loadImages(const std::string& dir, GAFTextureLoadDelegate_t delegate, cocos2d::ZipFile* bundle = nullptr);
	cocos2d::Texture2D*		getTextureById(uint32_t id);
	uint32_t				getMemoryConsumptionStat() const;

private:
	typedef std::map<size_t, cocos2d::Image*> ImagesMap_t;
	typedef std::map<size_t, cocos2d::Texture2D*> TexturesMap_t;

	bool isAtlasInfoPresent(const GAFTextureAtlas::AtlasInfo &ai);

	GAFTextureAtlas::AtlasInfos_t m_atlasInfos;

	ImagesMap_t m_images;
	TexturesMap_t m_textures;

	uint32_t m_memoryConsumption;
};

NS_GAF_END