#include "GAFPrecompiled.h"

#include "TagDefineTimeline.h"

#include "GAFLoader.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "PrimitiveDeserializer.h"

NS_GAF_BEGIN

void TagDefineTimeline::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    unsigned int id = in->readU32();
    unsigned int framesCount = in->readU32();
    cocos2d::Rect aabb;
    cocos2d::Point pivot;

    PrimitiveDeserializer::deserialize(in, &aabb);
    PrimitiveDeserializer::deserialize(in, &pivot);

    GAFTimeline *tl = new GAFTimeline(timeline, id, aabb, pivot, framesCount);

    //////////////////////////////////////////////////////////////////////////

    char hasLinkage = in->readUByte();
    if (hasLinkage)
    {
        std::string linkageName;
        in->readString(&linkageName);
        tl->setLinkageName(linkageName);
    }

    m_loader->loadTags(in, asset, tl);

    if (id == 0)
    {
        asset->setRootTimeline(tl);
    }

    asset->pushTimeline(id, tl);
}

TagDefineTimeline::TagDefineTimeline(GAFLoader* loader) :
m_loader(loader)
{

}

NS_GAF_END