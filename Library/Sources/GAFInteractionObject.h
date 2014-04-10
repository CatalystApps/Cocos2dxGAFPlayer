#pragma once

#ifndef __GAF_INTERACTION_OBJECT__
#define __GAF_INTERACTION_OBJECT__


namespace cocos2d
{
    class CCDictionary;
}

using namespace cocos2d;

class GAFInteractionObject : public CCObject
{
public:
    std::string name; // TODO : think what is it better than CCString
    CCPoint     pivotPoint;
    CCRect      bounds;

    static GAFInteractionObject * create(CCDictionary * aDictionary);
    bool initWithDictionary(CCDictionary * aDictionary);
protected:
    GAFInteractionObject();

}; // GAFInteractionObject

#endif // __GAF_INTERACTION_OBJECT__
