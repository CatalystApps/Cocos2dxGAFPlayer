#pragma once

NS_GAF_BEGIN

class GAFStream;

class PrimitiveDeserializer
{
public:
    static void deserialize(GAFStream* in, cocos2d::Vect* out);
    static void deserialize(GAFStream* in, cocos2d::Rect* out);
    static void deserialize(GAFStream* in, cocos2d::AffineTransform* out);
    static void deserialize(GAFStream* in, cocos2d::Size* out);
    static void deserialize(GAFStream* in, cocos2d::Color4B* out);
};

NS_GAF_END