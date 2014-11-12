#pragma once

#include "GAFCollections.h"

NS_GAF_BEGIN

enum GAFColorTransformIndex
{
    GAFCTI_R,
    GAFCTI_G,
    GAFCTI_B,
    GAFCTI_A
};

class GAFSubobjectState
{
private:
    Filters_t       m_filters;
    float           _colorMults[4];
    float           _colorOffsets[4];

    unsigned long   m_refCount;

public:

    unsigned int objectIdRef;
    unsigned int maskObjectIdRef;

    int zIndex;
    cocos2d::AffineTransform affineTransform;

    bool initEmpty(unsigned int objectIdRef);

    ~GAFSubobjectState();
    GAFSubobjectState();

    inline float * colorMults()
    {
        return &_colorMults[0];
    }
    inline float * colorOffsets()
    {
        return &_colorOffsets[0];
    }

    inline const float * colorMults() const 
    {
        return &_colorMults[0];
    }

    inline const float * colorOffsets() const 
    {
        return &_colorOffsets[0];
    }

    inline bool isVisible() const
    {
        return (_colorMults[GAFCTI_A] > std::numeric_limits<float>::epsilon()) || (_colorOffsets[GAFCTI_A] > std::numeric_limits<float>::epsilon());
    }

    void                ctxMakeIdentity();

    void                pushFilter(GAFFilterData* filter);
    const Filters_t&    getFilters() const;


    void                addRef();
    void                release();

}; // GAFSubobjectState

NS_GAF_END