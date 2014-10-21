#include "GAFPrecompiled.h"

#include "TagDefineTimeline.h"

#include "GAFLoader.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "PrimitiveDeserializer.h"

NS_GAF_BEGIN

void TagDefineTimeline::read(GAFStream* in, GAFAsset* ctx)
{
    /*unsigned int id = */in->readU32();
    /*unsigned int framesCount = */in->readU32();
    cocos2d::Rect aabb;
    cocos2d::Point pivot;

    PrimitiveDeserializer::deserialize(in, &aabb);
    PrimitiveDeserializer::deserialize(in, &pivot);

    // STUB
    GAFHeader header = ctx->getHeader();
    header.pivot = pivot;
    header.frameSize = aabb;
    ctx->setHeader(header);
    //////////////////////////////////////////////////////////////////////////

    char hasLinkage = in->readUByte();
    if (hasLinkage)
    {
        std::string linkageName;
        in->readString(&linkageName);
    }

    m_loader->loadTags(in, ctx);
}

TagDefineTimeline::TagDefineTimeline(GAFLoader* loader):
m_loader(loader)
{
    
}

NS_GAF_END
