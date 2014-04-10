#pragma once

#ifndef __GAF_FILTER_DATA__
#define __GAF_FILTER_DATA__

#include "GAFCollections.h"

class GAFSpriteWithAlpha;

class GAFFilterData
{
protected:
    const GAFFilterType m_type;
public:

    virtual ~GAFFilterData() {}
    
    GAFFilterType           getType() const
    {
        return m_type;
    }

    GAFFilterData(GAFFilterType type) : m_type(type)
    {}

    virtual void apply(GAFSpriteWithAlpha*) = 0;
};

class GAFBlurFilterData : public GAFFilterData
{
public:
    CCSize blurSize;
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
    ccColor4F   color;
    CCSize      blurSize;
    float       strength;
    bool        innerGlow;
    bool        knockout;

    GAFGlowFilterData();
    virtual void apply(GAFSpriteWithAlpha*);
};

class GAFDropShadowFilterData : public GAFFilterData
{
public:
    ccColor4F       color;
    CCSize          blurSize;
    float           angle;
    float           distance;
    float           strength;
    bool            innerShadow;
    bool            knockout;

    GAFDropShadowFilterData();
    virtual void apply(GAFSpriteWithAlpha*);

    static void reset(GAFSpriteWithAlpha*);
};

#endif // __GAF_FILTER_DATA__
