#include "GAFPrecompiled.h"
#include "GAFTimelineAction.h"

NS_GAF_BEGIN

GAFTimelineAction::GAFTimelineAction()
: m_type(GAFActionType::None)
{

}

void GAFTimelineAction::setAction(GAFActionType type, ActionParams_t params, const std::string& scope)
{
    m_type = type;
    m_scope = scope;

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
        CCASSERT(params.size() > 0 && params.size() < 5, "Something wrong with action parameters");
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

const std::string GAFTimelineAction::getParam(ParameterIndex idx)
{
	if (m_params.size() <= idx)
		return "";

    return m_params[idx];
}

NS_GAF_END