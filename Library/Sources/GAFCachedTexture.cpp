#include "GAFPrecompiled.h"
#include "GAFCachedTexture.h"

USING_NS_CC;
using namespace std;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WP8

#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
using cocos2d::gettimeofday;
#endif

NS_GAF_BEGIN

cocos2d::Texture2D* GAFCachedTexture::operator *()
{
    timeval now;
    gettimeofday(&now, nullptr);
    m_lastUsed = now.tv_sec * 1000 + now.tv_usec;
    ++m_timesUsed;
    return m_texture;
}

GAFCachedTexture::GAFCachedTexture()
: m_texture(nullptr)
, m_lastUsed(0)
, m_created(0)
, m_timesUsed(1)
, m_memoryUsed(0)
{

}

GAFCachedTexture::GAFCachedTexture(Texture2D* tex)
: m_texture(tex)
, m_lastUsed(0)
, m_created(0)
, m_timesUsed(1)
, m_memoryUsed(0)
{
    m_memoryUsed = tex->getBitsPerPixelForFormat() / 8 *
        static_cast<size_t>(tex->getContentSizeInPixels().width * tex->getContentSizeInPixels().height);
    CC_SAFE_RETAIN(m_texture);
    timeval now;
    gettimeofday(&now, nullptr);
    m_created = now.tv_sec * 1000 + now.tv_usec;
    m_lastUsed = m_created;
}

const GAFCachedTexture& GAFCachedTexture::operator =(const GAFCachedTexture& other)
{
    if (&other == this)
        return *this;
    if (m_texture == other.m_texture)
        return *this;
    m_texture = other.m_texture;
    m_lastUsed = other.m_lastUsed;
    m_created = other.m_created;
    m_timesUsed = other.m_timesUsed;
    m_memoryUsed = other.m_memoryUsed;
    CC_SAFE_RETAIN(m_texture);
    return *this;
}

GAFCachedTexture::GAFCachedTexture(const GAFCachedTexture& other)
: m_texture(other.m_texture)
, m_lastUsed(other.m_lastUsed)
, m_created(other.m_created)
, m_timesUsed(other.m_timesUsed)
, m_memoryUsed(other.m_memoryUsed)
{
    CC_SAFE_RETAIN(m_texture);
}

GAFCachedTexture::~GAFCachedTexture()
{
    CC_SAFE_RELEASE(m_texture);
}

NS_GAF_END
