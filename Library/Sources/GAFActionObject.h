#pragma once

#ifndef __GAF_ACTION_OBJECT__
#define __GAF_ACTION_OBJECT__

namespace cocos2d
{
    class CCDictionary;
}

using namespace cocos2d;

class GAFActionObject : public CCObject
{
public:
    std::string name; // TODO : think what is it better than CCString
    CCPoint     pivotPoint;
    CCRect      bounds;

    static GAFActionObject * create(CCDictionary * aDictionary);
    bool initWithDictionary(CCDictionary * aDictionary);
protected:
    GAFActionObject();

}; // GAFInteractionObject

#endif // __GAF_ACTION_OBJECT__
