#pragma once
#include "GAFCollections.h"

NS_GAF_BEGIN

class GAFCachedTexture
{
public:
    cocos2d::Texture2D* operator *();
    const GAFCachedTexture& operator =(const GAFCachedTexture&);
    GAFCachedTexture();
    GAFCachedTexture(cocos2d::Texture2D* tex);
    GAFCachedTexture(const GAFCachedTexture& other);
    ~GAFCachedTexture();

    time_t created() const { return m_created; }
    time_t lastUsed() const { return m_lastUsed; }
    unsigned int timesUsed() const { return m_timesUsed; }
    size_t memoryUsed() const { return m_memoryUsed; }
private:
    cocos2d::
    Texture2D*      m_texture;
    time_t          m_lastUsed;
    time_t          m_created;
    unsigned int    m_timesUsed;
    size_t          m_memoryUsed;
};

NS_GAF_END
