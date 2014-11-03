#include "GAFPrecompiled.h"
#include "GAFTimelineAction.h"

NS_GAF_BEGIN

void GAFTimelineAction::setAction(GAFActionType type, ActionParams_t params)
{
    m_type = type;

    switch (type)
    {
    case GAFActionType::None:
    case GAFActionType::Stop:
    case GAFActionType::Play:
        CCASSERT(params.size() == 0, "Something wrong with action parameters");
        break;
    case GAFActionType::GotoAndStop:
    case GAFActionType::GotoAndPlay:
        CCASSERT(params.size() == 1, "Something wrong with action parameters");
        m_params = params;
        break;
    case GAFActionType::DispatchEvent:
        CCASSERT(params.size() > 0 && params.size() < 4, "Something wrong with action parameters");
        m_params = params;
        break;
    default:
        break;
    }
}

GAFActionType GAFTimelineAction::getType()
{
    return m_type;
}

ActionParams_t const& GAFTimelineAction::getParams()
{
    return m_params;
}

NS_GAF_END