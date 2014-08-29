#include "GAFPrecompiled.h"

#include "GAFAssetTextureManager.h"
#include "GAFAsset.h"

GAFAssetTextureManager::GAFAssetTextureManager():
m_memoryConsumption(0)
{

}

GAFAssetTextureManager::~GAFAssetTextureManager()
{

}

void GAFAssetTextureManager::appendInfoFromTextureAtlas(GAFTextureAtlas* atlas)
{
	GAFTextureAtlas::AtlasInfos_t atlasInfos = atlas->getAtlasInfos();
	GAFTextureAtlas::AtlasInfos_t::const_iterator i = atlasInfos.begin(), e = atlasInfos.end();
	for (; i != e; i++)
	{
		if (!isAtlasInfoPresent(*i))
		{
			m_atlasInfos.push_back(*i);
		}
	}
}

bool GAFAssetTextureManager::isAtlasInfoPresent(const GAFTextureAtlas::AtlasInfo &ai)
{
	GAFTextureAtlas::AtlasInfos_t::const_iterator i = m_atlasInfos.begin(), e = m_atlasInfos.end();
	for (; i != e; i++)
	{
		if (i->id == ai.id)
		{
			return true;
		}
	}
	return false;
}

void GAFAssetTextureManager::loadImages(const std::string& dir, GAFTextureLoadDelegate* delegate, cocos2d::ZipFile* bundle)
{
	std::stable_sort(m_atlasInfos.begin(), m_atlasInfos.end(), GAFTextureAtlas::compareAtlasesById);

	m_images.clear(); // check
	
	if (!m_atlasInfos.empty())
	{
		for (unsigned int i = 0; i < m_atlasInfos.size(); ++i)
		{
			GAFTextureAtlas::AtlasInfo& info = m_atlasInfos[i];

			std::string source;

			for (unsigned int j = 0; j < info.m_sources.size(); ++j)
			{
				GAFTextureAtlas::AtlasInfo::Source& aiSource = info.m_sources[j];
				if (1.f == aiSource.csf)
				{
					source = aiSource.source;
				}

				if (aiSource.csf == GAFAsset::desiredCsf())
				{
					source = aiSource.source;
					break;
				}
			}

			cocos2d::Image* image = new cocos2d::Image();
			std::string path = cocos2d::FileUtils::getInstance()->fullPathFromRelativeFile(source.c_str(), dir.c_str());

			if (delegate)
			{
				delegate->onTexturePreLoad(path);
			}

			if (!bundle)
			{
				image->initWithImageFile(path.c_str());
			}
			else
			{
				ssize_t sz = 0;
				unsigned char* imgData = bundle->getFileData(path, &sz);
				if (!imgData || !sz)
					return;

				image->initWithImageData(imgData, sz);
			}

			m_memoryConsumption += image->getDataLen();

#if ENABLE_GAF_MANUAL_PREMULTIPLY
			if (!image->isPremultipliedAlpha() && image->hasAlpha())
			{
				//Premultiply
				unsigned char* begin = image->getData();
				unsigned int width = image->getWidth();
				unsigned int height = image->getHeight();
				int Bpp = image->getBitPerPixel() / 8;
				unsigned char* end = begin + width * height * Bpp;
				for (auto data = begin; data < end; data += Bpp)
				{
					unsigned int* wordData = (unsigned int*)(data);
					*wordData = CC_RGB_PREMULTIPLY_ALPHA(data[0], data[1], data[2], data[3]);
				}
			}
#endif
			m_images[info.id] = image;//->addObject(image);
			//image->release(); ???
		}
	}
}

cocos2d::Texture2D* GAFAssetTextureManager::getTextureById(uint32_t id)
{
	return nullptr;
}