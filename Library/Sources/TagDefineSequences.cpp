#include "GAFPrecompiled.h"
#include "TagDefineSequences.h"

#include "GAFStream.h"
#include "GAFAsset.h"

void TagDefineSequences::read(GAFStream* in, GAFAsset* ctx)
{
    unsigned int count = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        std::string id;
        in->readString(&id);
        int start = in->readU16();
        int end = in->readU16();

        ctx->pushAnimationSequence(id, start, end);
    }
}
