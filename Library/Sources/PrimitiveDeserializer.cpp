#include "GAFPrecompiled.h"
#include "PrimitiveDeserializer.h"

#include "GAFStream.h"

using namespace cocos2d;

void PrimitiveDeserializer::deserialize(GAFStream* in, CCPoint* out)
{
    out->x = in->readFloat();
    out->y = in->readFloat();
}

void PrimitiveDeserializer::deserialize(GAFStream* in, CCRect* out)
{
    deserialize(in, &out->origin);
    deserialize(in, &out->size);
}

void PrimitiveDeserializer::deserialize(GAFStream* in, CCAffineTransform* out)
{
    in->readNBytesOfT(out, sizeof(CCAffineTransform));
}

void PrimitiveDeserializer::deserialize(GAFStream* in, CCSize* out)
{
    out->width = in->readFloat();
    out->height = in->readFloat();
}

void PrimitiveDeserializer::deserialize(GAFStream* in, ccColor4B* out)
{
    in->readNBytesOfT(out, 4);
}
