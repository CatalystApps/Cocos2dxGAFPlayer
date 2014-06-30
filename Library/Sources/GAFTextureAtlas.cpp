#include "GAFPrecompiled.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFAsset.h"

#if CC_ENABLE_CACHE_TEXTURE_DATA
#include "renderer/CCTextureCache.h"
#endif

GAFTextureAtlas::GAFTextureAtlas()
:
_loaded(false),
_images(NULL),
_textures(NULL),
m_scale(1.f)
{
}

GAFTextureAtlas::~GAFTextureAtlas()
{
    CC_SAFE_RELEASE(_images);
    CC_SAFE_RELEASE(_textures);

    GAF_RELEASE_MAP(GAFTextureAtlas::Elements_t, m_elements);
}

static bool compareAtlasesById(const GAFTextureAtlas::AtlasInfo& ai1, const GAFTextureAtlas::AtlasInfo& ai2)
{
    return ai1.id < ai2.id;
}

void GAFTextureAtlas::loadImages(const std::string& dir, GAFTextureLoadDelegate* delegate)
{
    std::stable_sort(m_atlasInfos.begin(), m_atlasInfos.end(), compareAtlasesById);

    CC_SAFE_RELEASE(_images);
    _images = new cocos2d::__Array();
    _images->initWithCapacity(m_atlasInfos.size());

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

            image->initWithImageFile(path.c_str());
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
            _images->addObject(image);
            image->release();
        }

        if (_images->count() > 0)
        {
            _loaded = true;
        }
    }
}

cocos2d::Image * GAFTextureAtlas::image()
{
    if (_images && _images->count() > 0)
    {
        return (cocos2d::Image*)_images->getObjectAtIndex(0);
    }
    return NULL;
}

cocos2d::__Array * GAFTextureAtlas::images()
{
    return _images;
}

cocos2d::Texture2D * GAFTextureAtlas::texture()
{
    if (_textures && _textures->count() > 0)
    {
        return (cocos2d::Texture2D*)_textures->getObjectAtIndex(0);
    }
    return NULL;
}

cocos2d::__Array * GAFTextureAtlas::textures()
{
    if (!_textures)
    {
        _textures = cocos2d::__Array::createWithCapacity(_images->count());
        for (int i = 0; i < _images->count(); ++i)
        {
            cocos2d::Texture2D * texture = new cocos2d::Texture2D();
            cocos2d::Image * image = (cocos2d::Image*)_images->getObjectAtIndex(i);
            texture->initWithImage(image);
            _textures->addObject(texture);
#if CC_ENABLE_CACHE_TEXTURE_DATA
            cocos2d::VolatileTextureMgr::addImage(texture, image);
#endif
            texture->release();
        }
        _textures->retain();
    }
    return _textures;
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
