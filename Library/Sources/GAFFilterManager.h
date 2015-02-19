#pragma once
#include "GAFCollections.h"
#include "GAFCachedTexture.h"

NS_GAF_BEGIN

class GAFSprite;
class GAFBlurFilterData;
class GAFGlowFilterData;
class GAFDropShadowFilterData;

class GAFFilterManager : public cocos2d::Ref
{
    typedef std::map<unsigned int, GAFCachedTexture> Cache_t;
    typedef std::pair<unsigned int, GAFCachedTexture> CachePair_t;
    
public:
    bool init();
    static GAFFilterManager* getInstance();
    ~GAFFilterManager() {}

    cocos2d::Texture2D* applyFilter(cocos2d::Sprite*, GAFFilterData*);

    void update(float dt);

    static void setCacheSize(size_t newSize);
    
private:
    GAFFilterManager() {}

    unsigned int hash(cocos2d::Sprite*, GAFFilterData*);
    bool hasTexture(unsigned int);
    cocos2d::Texture2D* renderFilteredTexture(cocos2d::Sprite* sprite, GAFFilterData* filter);
    cocos2d::Texture2D* renderFilteredTexture(cocos2d::Sprite* sprite, GAFFilterData* filter, unsigned int hash);

    cocos2d::Texture2D* renderBlurTexture(cocos2d::Sprite* sprite, GAFBlurFilterData* filter);
    cocos2d::Texture2D* renderGlowTexture(cocos2d::Sprite* sprite, GAFGlowFilterData* filter);
    cocos2d::Texture2D* renderShadowTexture(cocos2d::Sprite* sprite, GAFDropShadowFilterData* filter);
    
    void insertTexture(cocos2d::Texture2D*, unsigned int hash);
    
private:

    static Cache_t s_cache;
    static GAFFilterManager* s_instance;
    
    static size_t s_maxCacheSize;
};

NS_GAF_END
