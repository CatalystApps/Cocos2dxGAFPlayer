#pragma once
#include "GAFMacros.h"
#include "GAFCollections.h"

NS_GAF_BEGIN

typedef std::vector<std::string> ActionParams_t;

class GAFTimelineAction
{
public:
	enum ParameterIndex
	{
		PI_FRAME = 0,
		PI_EVENT_TYPE = 0,
		PI_EVENT_BUBBLES,
		PI_EVENT_CANCELABLE,
		PI_EVENT_DATA
	};

    void setAction(GAFActionType type, ActionParams_t params);
    GAFActionType getType();
	const std::string getParam(ParameterIndex idx);

private:
    GAFActionType m_type = GAFActionType::None;
    ActionParams_t m_params;
};

NS_GAF_END