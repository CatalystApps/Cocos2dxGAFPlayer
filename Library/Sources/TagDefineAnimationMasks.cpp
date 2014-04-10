#include "GAFPrecompiled.h"
#include "TagDefineAnimationMasks.h"

#include "GAFStream.h"
#include "GAFAsset.h"

void TagDefineAnimationMasks::read(GAFStream* in, GAFAsset* ctx)
{
    unsigned int count = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int objectId = in->readU32();
        unsigned int elementAtlasIdRef = in->readU32();

        ctx->pushAnimationMask(objectId, elementAtlasIdRef);
    }
}
