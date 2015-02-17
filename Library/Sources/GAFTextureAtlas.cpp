#include "GAFPrecompiled.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFAsset.h"

#if CC_ENABLE_CACHE_TEXTURE_DATA
#include "renderer/CCTextureCache.h"
#endif

NS_GAF_BEGIN

#if COCOS2D_VERSION <= 0x00030101
#define ENABLE_GAF_MANUAL_PREMULTIPLY 1
#else
#define ENABLE_GAF_MANUAL_PREMULTIPLY 0
#endif

GAFTextureAtlas::GAFTextureAtlas()
:
m_scale(1.f),
m_loaded(false),
m_images(nullptr),
m_textures(nullptr),
m_memoryConsumption(0)
{
}

GAFTextureAtlas::~GAFTextureAtlas()
{
    CC_SAFE_RELEASE(m_images);
    CC_SAFE_RELEASE(m_textures);

    GAF_RELEASE_MAP(GAFTextureAtlas::Elements_t, m_elements);
}

bool GAFTextureAtlas::compareAtlasesById(const AtlasInfo& ai1, const AtlasInfo& ai2)
{
    return ai1.id < ai2.id;
}

void GAFTextureAtlas::loadImages(const std::string& dir, GAFTextureLoadDelegate_t dlg, cocos2d::ZipFile* bundle)
{
    std::stable_sort(m_atlasInfos.begin(), m_atlasInfos.end(), compareAtlasesById);

    CC_SAFE_RELEASE(m_images);
    m_images = new cocos2d::__Array();
    m_images->initWithCapacity(m_atlasInfos.size());

    if (!m_atlasInfos.empty())
    {
        for (unsigned int i = 0; i < m_atlasInfos.size(); ++i)
        {
            AtlasInfo& info = m_atlasInfos[i];

            std::string source;

            for (unsigned int j = 0; j < info.m_sources.size(); ++j)
            {
                AtlasInfo::Source& aiSource = info.m_sources[j];
                if (1.f == aiSource.csf)
                {
                    source = aiSource.source;
                }

                if (aiSource.csf == cocos2d::CCDirector::getInstance()->getContentScaleFactor())
                {
                    source = aiSource.source;
                    break;
                }
            }

            cocos2d::Image* image = new cocos2d::Image();
            std::string path = cocos2d::FileUtils::getInstance()->fullPathFromRelativeFile(source.c_str(), dir.c_str());

            if (dlg)
            {
                path = dlg(path);
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
            m_images->addObject(image);
            image->release();
        }

        if (m_images->count() > 0)
        {
            m_loaded = true;
        }
    }
}

cocos2d::Image * GAFTextureAtlas::image()
{
    if (m_images && m_images->count() > 0)
    {
        return (cocos2d::Image*)m_images->getObjectAtIndex(0);
    }
    return NULL;
}

cocos2d::__Array * GAFTextureAtlas::images()
{
    return m_images;
}

cocos2d::Texture2D * GAFTextureAtlas::texture()
{
    if (m_textures && m_textures->count() > 0)
    {
        return (cocos2d::Texture2D*)m_textures->getObjectAtIndex(0);
    }
    return NULL;
}

cocos2d::__Array * GAFTextureAtlas::textures()
{
    if (!m_textures)
    {
        m_textures = cocos2d::__Array::createWithCapacity(m_atlasInfos.size());
        for (size_t i = 0; i < m_atlasInfos.size(); ++i)
        {
            cocos2d::Texture2D * texture = new cocos2d::Texture2D();
            cocos2d::Image * image = (cocos2d::Image*)m_images->getObjectAtIndex(i);
            texture->initWithImage(image);
            m_textures->addObject(texture);
#if CC_ENABLE_CACHE_TEXTURE_DATA
            cocos2d::VolatileTextureMgr::addImage(texture, image);
#endif
            texture->release();
        }
        m_textures->retain();

        CC_SAFE_RELEASE(m_images);
        m_images = nullptr;
    }
    return m_textures;
}

void GAFTextureAtlas::setScale(float val)
{
    m_scale = val;
}

float GAFTextureAtlas::getScale() const
{
    return m_scale;
}

void GAFTextureAtlas::pushAtlasInfo(const AtlasInfo& ai)
{
    m_atlasInfos.push_back(ai);
}

void GAFTextureAtlas::pushElement(unsigned int idx, GAFTextureAtlasElement* el)
{
    m_elements[idx] = el;
}

const GAFTextureAtlas::Elements_t& GAFTextureAtlas::getElements() const
{
    return m_elements;
}

const GAFTextureAtlas::AtlasInfos_t& GAFTextureAtlas::getAtlasInfos() const
{
	return m_atlasInfos;
}

uint32_t GAFTextureAtlas::getMemoryConsumptionStat() const
{
    return m_memoryConsumption;
}

NS_GAF_END