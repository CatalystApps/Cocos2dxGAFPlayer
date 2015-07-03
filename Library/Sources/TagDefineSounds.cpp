#include "GAFPrecompiled.h"
#include "TagDefineSounds.h"

#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"
#include "GAFSoundInfo.h"

NS_GAF_BEGIN

void TagDefineSounds::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    (void)timeline;

    uint16_t count = in->readU16();

    for (uint16_t i = 0; i < count; ++i)
    {
        GAFSoundInfo* soundInfo = new GAFSoundInfo;

        soundInfo->id = in->readU16();
        in->readString(&soundInfo->linkage);
        in->readString(&soundInfo->source);

        uint8_t tmpByte = in->readUByte(); // format
        soundInfo->setFormat(tmpByte);
        tmpByte = in->readUByte(); // rate
        soundInfo->setRate(tmpByte);
        tmpByte = in->readUByte(); // sample size
        soundInfo->setSampleSize(tmpByte);

        soundInfo->stereo = in->readBool();
        soundInfo->sampleCount = in->readU32();
        asset->pushSound(soundInfo->id, soundInfo);
    }
}

NS_GAF_END