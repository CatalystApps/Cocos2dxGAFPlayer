#include "GAFPrecompiled.h"

#include "TagDefineTimeline.h"

#include "GAFLoader.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "PrimitiveDeserializer.h"


void TagDefineTimeline::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
	unsigned int id = in->readU32();
    unsigned int framesCount = in->readU32();
    cocos2d::Rect aabb;
    cocos2d::Point pivot;

    PrimitiveDeserializer::deserialize(in, &aabb);
    PrimitiveDeserializer::deserialize(in, &pivot);

	GAFTimeline *tl = new GAFTimeline(id, aabb, pivot, framesCount);

    // STUB
    GAFHeader header = asset->getHeader();
    header.pivot = pivot;
    header.frameSize = aabb;
	asset->setHeader(header);
    //////////////////////////////////////////////////////////////////////////

    char hasLinkage = in->readUByte();
    if (hasLinkage)
    {
        std::string linkageName;
        in->readString(&linkageName);
    }
	
	m_loader->loadTags(in, asset, tl);
	asset->pushTimeline(tl);
}

TagDefineTimeline::TagDefineTimeline(GAFLoader* loader):
m_loader(loader)
{
    
}
