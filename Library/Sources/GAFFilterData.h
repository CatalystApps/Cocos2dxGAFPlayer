#pragma once
#include "GAFCollections.h"

NS_GAF_BEGIN

class GAFMovieClip;

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

    virtual void apply(GAFMovieClip*){};
};

class GAFBlurFilterData : public GAFFilterData
{
public:
    cocos2d::Size blurSize;
    GAFBlurFilterData();

    virtual void apply(GAFMovieClip*);
};

class GAFColorColorMatrixFilterData : public GAFFilterData
{
public:
    float matrix[16];
    float matrix2[4];
    GAFColorColorMatrixFilterData();

    virtual void apply(GAFMovieClip*);
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
    virtual void apply(GAFMovieClip*);
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
    virtual void apply(GAFMovieClip*);

    static void reset(GAFMovieClip*);
};

NS_GAF_END