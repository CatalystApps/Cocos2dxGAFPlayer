#pragma once

NS_GAF_BEGIN

class GAFStream;

class PrimitiveDeserializer
{
    struct GAFReadColor
    {
        unsigned char b, g, r, a;
    };
public:
    static void deserialize(GAFStream* in, cocos2d::Vect* out);
    static void deserialize(GAFStream* in, cocos2d::Rect* out);
    static void deserialize(GAFStream* in, cocos2d::AffineTransform* out);
    static void deserialize(GAFStream* in, cocos2d::Size* out);
    static void deserialize(GAFStream* in, cocos2d::Color4B* out);
    static void translateColor(cocos2d::Color4F& out, unsigned int in);
};

NS_GAF_END