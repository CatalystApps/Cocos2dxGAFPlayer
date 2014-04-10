#include "GAFPrecompiled.h"
#include "TagDefineNamedParts.h"

#include "GAFAsset.h"
#include "GAFStream.h"

void TagDefineNamedParts::read(GAFStream* in, GAFAsset* ctx)
{
    unsigned int count = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int objectIdRef = in->readU32();
        std::string name;
        in->readString(&name);

        ctx->pushNamedPart(objectIdRef, name);
    }
}
