#pragma once

class GAFStream;

class PrimitiveDeserializer
{
public:
    static void deserialize(GAFStream* in, CCPoint* out);
    static void deserialize(GAFStream* in, CCRect* out);
    static void deserialize(GAFStream* in, CCAffineTransform* out);
    static void deserialize(GAFStream* in, CCSize* out);
    static void deserialize(GAFStream* in, ccColor4B* out);
};