#pragma once

#ifndef __GAF_ACTION_OBJECT__
#define __GAF_ACTION_OBJECT__

namespace cocos2d
{
    class CCDictionary;
}

class GAFActionObject : public cocos2d::CCObject
{
public:
    std::string         name; // TODO : think what is it better than CCString
    cocos2d::CCPoint    pivotPoint;
    cocos2d::CCRect     bounds;

    static GAFActionObject * create(cocos2d::CCDictionary * aDictionary);
    bool initWithDictionary(cocos2d::CCDictionary * aDictionary);
protected:
    GAFActionObject();

}; // GAFInteractionObject

#endif // __GAF_ACTION_OBJECT__
