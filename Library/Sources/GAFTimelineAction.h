#pragma once
#include "GAFMacros.h"
#include "GAFCollections.h"

NS_GAF_BEGIN

typedef std::vector<std::string> ActionParams_t;

class GAFTimelineAction
{
public:
    void setAction(GAFActionType type, ActionParams_t params);
    GAFActionType getType();
    const ActionParams_t& getParams();

private:
    GAFActionType m_type = GAFActionType::None;
    ActionParams_t m_params;
};

NS_GAF_END