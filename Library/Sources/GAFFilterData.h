#pragma once
#include "GAFCollections.h"

NS_GAF_BEGIN

class GAFSpriteWithAlpha;

class GAFFilterData
{
protected:
    GAFFilterType m_type;
public:

    virtual ~GAFFilterData() {}
    
    GAFFilterType           getType() const
    {
        return m_type;
    }

    GAFFilterData(GAFFilterType type) : m_type(type)
    {}

    virtual void apply(GAFSpriteWithAlpha*){};
};

class GAFBlurFilterData : public GAFFilterData
{
public:
    cocos2d::Size blurSize;
    GAFBlurFilterData();

    virtual void apply(GAFSpriteWithAlpha*);
};

class GAFColorColorMatrixFilterData : public GAFFilterData
{
public:
    float matrix[16];
    float matrix2[4];
    GAFColorColorMatrixFilterData();

    virtual void apply(GAFSpriteWithAlpha*);
};

class GAFGlowFilterData : public GAFFilterData
{
public:
    cocos2d::Color4F   color;
    cocos2d::Size      blurSize;
    float       strength;
    bool        innerGlow;
    bool        knockout;

    GAFGlowFilterData();
    virtual void apply(GAFSpriteWithAlpha*);
};

class GAFDropShadowFilterData : public GAFFilterData
{
public:
    cocos2d::Color4F       color;
    cocos2d::Size          blurSize;
    float           angle;
    float           distance;
    float           strength;
    bool            innerShadow;
    bool            knockout;

    GAFDropShadowFilterData();
    virtual void apply(GAFSpriteWithAlpha*);

    static void reset(GAFSpriteWithAlpha*);
};

NS_GAF_END