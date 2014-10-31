#include "GAFPrecompiled.h"
#include "GAFTextData.h"

NS_GAF_BEGIN

cocos2d::TextHAlignment GAFTextData::TextFormat::getTextAlignForCocos() const
{
    switch (m_align)
    {
    case TextAlign::Left:
        return cocos2d::TextHAlignment::LEFT;
    case TextAlign::Right:
        return cocos2d::TextHAlignment::RIGHT;
    case TextAlign::Center:
        return cocos2d::TextHAlignment::CENTER;
    default:
        CCAssert(false, "Not Implemented");
    }
    return cocos2d::TextHAlignment::LEFT;
}

NS_GAF_END