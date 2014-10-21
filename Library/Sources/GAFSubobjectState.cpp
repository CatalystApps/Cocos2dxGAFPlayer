#include "GAFPrecompiled.h"
#include "GAFSubobjectState.h"
#include "GAFFilterData.h"
#include "GAFAsset.h"

#include "GAFCollections.h"

NS_GAF_BEGIN

GAFSubobjectState::GAFSubobjectState()
:
objectIdRef(IDNONE),
maskObjectIdRef(IDNONE)
{
    m_refCount = 1;
}

GAFSubobjectState::~GAFSubobjectState()
{
    GAF_RELEASE_ARRAY(Filters_t, m_filters);
}

bool GAFSubobjectState::initEmpty(unsigned int ref)
{
    objectIdRef = ref;
    zIndex = 0;
    affineTransform = cocos2d::AffineTransformMake(1, 0, 0, 1, 0, 0);
    _colorOffsets[0] = _colorOffsets[1] = _colorOffsets[2] = _colorOffsets[3] = _colorMults[GAFCTI_A] = 0;
    _colorMults[GAFCTI_R] = _colorMults[GAFCTI_G] = _colorMults[GAFCTI_B] = 1;
    return true;
}

void GAFSubobjectState::ctxMakeIdentity()
{
    _colorOffsets[0] = _colorOffsets[1] = _colorOffsets[2] = _colorOffsets[3] = 0;
    _colorMults[GAFCTI_R] = _colorMults[GAFCTI_G] = _colorMults[GAFCTI_B] = 1;
}

void GAFSubobjectState::pushFilter(GAFFilterData* filter)
{
    m_filters.push_back(filter);
}

const Filters_t& GAFSubobjectState::getFilters() const
{
    return m_filters;
}

void GAFSubobjectState::addRef()
{
    m_refCount++;
}

void GAFSubobjectState::release()
{
    --m_refCount;

    if (0 == m_refCount)
    {
        delete this;
    }
}

NS_GAF_END