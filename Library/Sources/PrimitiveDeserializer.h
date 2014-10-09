#pragma once

class GAFStream;

class PrimitiveDeserializer
{
public:
    static void deserialize(GAFStream* in, cocos2d::CCPoint* out);
    static void deserialize(GAFStream* in, cocos2d::CCRect* out);
    static void deserialize(GAFStream* in, cocos2d::CCAffineTransform* out);
    static void deserialize(GAFStream* in, cocos2d::CCSize* out);
    static void deserialize(GAFStream* in, cocos2d::ccColor4B* out);
};