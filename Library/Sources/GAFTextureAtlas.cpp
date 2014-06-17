#include "GAFPrecompiled.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFAsset.h"
#include "cocoa/CCInteger.h"

#if CC_ENABLE_CACHE_TEXTURE_DATA
#include "textures/CCTextureCache.h"
#endif

GAFTextureAtlas::GAFTextureAtlas()
:
m_loaded(false),
m_images(NULL),
m_textures(NULL),
m_scale(1.f)
{
}

GAFTextureAtlas::~GAFTextureAtlas()
{
    CC_SAFE_RELEASE(m_images);
    CC_SAFE_RELEASE(m_textures);

    GAF_RELEASE_MAP(GAFTextureAtlas::Elements_t, m_elements);
}

static bool compareAtlasesById(const GAFTextureAtlas::AtlasInfo& ai1, const GAFTextureAtlas::AtlasInfo& ai2)
{
    return ai1.id < ai2.id;
}

void GAFTextureAtlas::loadImages(const std::string& dir, GAFTextureLoadDelegate* delegate)
{
    std::stable_sort(m_atlasInfos.begin(), m_atlasInfos.end(), compareAtlasesById);

    CC_SAFE_RELEASE(m_images);
    m_images = new CCArray();

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

            CCImage* image = new CCImage();
            std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(source.c_str(), dir.c_str());

            if (delegate)
            {
                delegate->onTexturePreLoad(path);
            }

            image->initWithImageFile(path.c_str());
            m_images->addObject(image);
            image->release();
        }

        if (m_images->count() > 0)
        {
            m_loaded = true;
        }
    }
}

CCImage     * GAFTextureAtlas::image()
{
    if (m_images && m_images->count() > 0)
    {
        return (CCImage*)m_images->objectAtIndex(0);
    }
    return NULL;
}

CCArray     * GAFTextureAtlas::images()
{
    return m_images;
}

CCTexture2D * GAFTextureAtlas::texture()
{
    if (m_textures && m_textures->count() > 0)
    {
        return (CCTexture2D*)m_textures->objectAtIndex(0);
    }
    return NULL;
}

CCArray * GAFTextureAtlas::textures()
{
    if (!m_textures)
    {
        m_textures = CCArray::createWithCapacity(m_images->count());
        for (unsigned int i = 0; i < m_images->count(); ++i)
        {
            CCTexture2D * texture = new CCTexture2D();
            CCImage * image = (CCImage*)m_images->objectAtIndex(i);
            texture->initWithImage(image);
            m_textures->addObject(texture);
#if CC_ENABLE_CACHE_TEXTURE_DATA
            VolatileTexture::addCCImage(texture, image);
#endif
            texture->release();
        }
        m_textures->retain();
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

bool GAFTextureAtlas::loaded() const
{
    return m_loaded;
}
