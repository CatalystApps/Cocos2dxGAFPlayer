#include "GAFPrecompiled.h"
#include "GAFAnimationFrame.h"
#include "GAFSubobjectState.h"
#include "GAFTextureAtlas.h"

GAFAnimationFrame::GAFAnimationFrame()
{

}

GAFAnimationFrame::~GAFAnimationFrame()
{
    for (GAFAnimationFrame::SubobjectStates_t::iterator i = m_subObjectStates.begin(), e = m_subObjectStates.end(); i != e; ++i)
    {
        (*i)->release();
    }
}

const GAFAnimationFrame::SubobjectStates_t& GAFAnimationFrame::getObjectStates() const
{
    return m_subObjectStates;
}

void GAFAnimationFrame::pushObjectState(GAFSubobjectState* state)
{
    m_subObjectStates.push_back(state);
    state->addRef();
}

