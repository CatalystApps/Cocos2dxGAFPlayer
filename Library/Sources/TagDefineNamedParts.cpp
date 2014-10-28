#include "GAFPrecompiled.h"
#include "TagDefineNamedParts.h"

#include "GAFAsset.h"
#include "GAFStream.h"
#include "GAFTimeline.h"

NS_GAF_BEGIN

void TagDefineNamedParts::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    (void)asset;
    unsigned int count = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int objectIdRef = in->readU32();
        std::string name;
        in->readString(&name);

        timeline->pushNamedPart(objectIdRef, name);
    }
}

NS_GAF_END