#pragma once
#include "GAFTimelineAction.h"

NS_GAF_BEGIN

class GAFTextureAtlas;
class GAFSubobjectState;

class GAFAnimationFrame
{
public:
    typedef std::vector<GAFSubobjectState*> SubobjectStates_t;
    typedef std::vector<GAFTimelineAction> TimelineActions_t;
private:
    SubobjectStates_t       m_subObjectStates;
    TimelineActions_t       m_timelineActions;
public:
    GAFAnimationFrame();
    ~GAFAnimationFrame();
    const SubobjectStates_t& getObjectStates() const;
    const TimelineActions_t& getTimelineActions() const;

    void    pushObjectState(GAFSubobjectState*);
    void    pushTimelineAction(GAFTimelineAction action);
};

NS_GAF_END