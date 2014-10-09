#pragma once

#ifndef __GAF_INTERACTION_OBJECT__
#define __GAF_INTERACTION_OBJECT__


namespace cocos2d
{
    class CCDictionary;
}

class GAFInteractionObject : public cocos2d::CCObject
{
public:
    std::string name; // TODO : think what is it better than CCString
    cocos2d::CCPoint     pivotPoint;
    cocos2d::CCRect      bounds;

    static GAFInteractionObject * create(cocos2d::CCDictionary * aDictionary);
    bool initWithDictionary(cocos2d::CCDictionary * aDictionary);
protected:
    GAFInteractionObject();

}; // GAFInteractionObject

#endif // __GAF_INTERACTION_OBJECT__
