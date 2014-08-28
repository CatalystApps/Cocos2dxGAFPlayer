#include "GAFPrecompiled.h"
#include "TagDefineSequences.h"

#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"

void TagDefineSequences::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    unsigned int count = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        std::string id;
        in->readString(&id);
        int start = in->readU16() - 1;
        int end = in->readU16(); // It is not actually the last frame, but the frame after the last, like an STL iterator

        timeline->pushAnimationSequence(id, start, end);
    }
}
