#include "GAFPrecompiled.h"
#include "TagDefineAnimationObjects.h"

#include "GAFFile.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"

void TagDefineAnimationObjects::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    unsigned int count = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int objectId = in->readU32();
        unsigned int elementAtlasIdRef = in->readU32();

		if (in->getInput()->getHeader().getMajorVersion() >= 4)
        {
            unsigned short objType = in->readU16();
			timeline->pushAnimationObject(objectId, elementAtlasIdRef, static_cast<GAFCharacterType>(objType));
        }
		else
		{
			asset->pushAnimationObjects(objectId, elementAtlasIdRef);
		}
    }
}
