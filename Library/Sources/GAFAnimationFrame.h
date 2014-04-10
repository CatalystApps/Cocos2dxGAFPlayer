#pragma once

#ifndef __GAF_ANIMATION_FRAME__
#define __GAF_ANIMATION_FRAME__

class GAFTextureAtlas;
class GAFSubobjectState;

class GAFAnimationFrame
{
public:
    typedef std::vector<GAFSubobjectState*> SubobjectStates_t;
private:
    SubobjectStates_t       m_subObjectStates;
public:
    GAFAnimationFrame();
    ~GAFAnimationFrame();
    const SubobjectStates_t& getObjectStates() const;

    void    pushObjectState(GAFSubobjectState*);
};

#endif // __GAF_ANIMATION_FRAME__
