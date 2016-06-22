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

    virtual void apply(GAFMovieClip*) override;
};

class GAFColorMatrixFilterData : public GAFFilterData
{
public:
    float matrix[16];
    float matrix2[4];
    GAFColorMatrixFilterData();

    void setMatrix(const float m[16]);
    void setMatrix2(const float m[4]);

    virtual void apply(GAFMovieClip*) override;

    struct Grayscale
    {
        static const float matrix[16];
        static const float matrix2[4];
    };
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
    virtual void apply(GAFMovieClip*) override;
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
    virtual void apply(GAFMovieClip*) override;

    static void reset(GAFMovieClip*);
};

NS_GAF_END