#pragma once

#include "GAFTextureAtlas.h"

class GAFAssetTextureManager
{
public:
	GAFAssetTextureManager();
	~GAFAssetTextureManager();

	void					appendInfoFromTextureAtlas(GAFTextureAtlas* atlas);
	void					loadImages(const std::string& dir, GAFTextureLoadDelegate* delegate, cocos2d::ZipFile* bundle = nullptr);
	cocos2d::Texture2D*		getTextureById(uint32_t id);
	uint32_t				getMemoryConsumptionStat() const;

private:
	bool isAtlasInfoPresent(const GAFTextureAtlas::AtlasInfo &ai);

	GAFTextureAtlas::AtlasInfos_t m_atlasInfos;

	std::map<size_t, cocos2d::Image*> m_images;
	std::map<size_t, cocos2d::Texture2D*> m_textures;

	uint32_t m_memoryConsumption;
};
