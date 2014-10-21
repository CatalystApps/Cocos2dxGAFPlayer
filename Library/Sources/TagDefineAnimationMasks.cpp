#include "GAFPrecompiled.h"
#include "TagDefineAnimationMasks.h"

#include "GAFFile.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"

NS_GAF_BEGIN

void TagDefineAnimationMasks::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    (void)asset;
    unsigned int count = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int objectId = in->readU32();
        unsigned int elementAtlasIdRef = in->readU32();

        if (in->getInput()->getHeader().getMajorVersion() >= 4)
        {
            unsigned short objType = in->readU16();
            timeline->pushAnimationMask(objectId, elementAtlasIdRef, static_cast<GAFCharacterType>(objType));
        }
        else
        {
            timeline->pushAnimationMask(objectId, elementAtlasIdRef, GAFCharacterType::Texture);
        }
    }
}

NS_GAF_END