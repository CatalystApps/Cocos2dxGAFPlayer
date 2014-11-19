#include "GAFPrecompiled.h"
#include "TagDefineAtlas.h"

#include "GAFFile.h"
#include "GAFHeader.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"

#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "PrimitiveDeserializer.h"

NS_GAF_BEGIN

void TagDefineAtlas::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    (void)asset;
    GAFTextureAtlas* txAtlas = new GAFTextureAtlas();

    txAtlas->setScale(in->readFloat());

    unsigned char atlasesCount = in->readUByte();

    for (unsigned char i = 0; i < atlasesCount; ++i)
    {
        GAFTextureAtlas::AtlasInfo ai;

        ai.id = in->readU32();

        unsigned char sources = in->readUByte();

        for (unsigned char j = 0; j < sources; ++j)
        {
            GAFTextureAtlas::AtlasInfo::Source aiSource;

            in->readString(&aiSource.source);
            aiSource.csf = in->readFloat();

            ai.m_sources.push_back(aiSource);
        }

        txAtlas->pushAtlasInfo(ai);
    }

    unsigned int elementsCount = in->readU32();

    for (unsigned int i = 0; i < elementsCount; ++i)
    {
        GAFTextureAtlasElement* element = new GAFTextureAtlasElement();

        PrimitiveDeserializer::deserialize(in, &element->pivotPoint);
        cocos2d::Vect origin;
        PrimitiveDeserializer::deserialize(in, &origin);
        element->scale = in->readFloat();

        // TODO: Optimize this to read CCRect
        float width = in->readFloat();
        float height = in->readFloat();

        element->atlasIdx = in->readU32();

        if (element->atlasIdx > 0)
        {
            element->atlasIdx--;
        }

        element->elementAtlasIdx = in->readU32();

        element->bounds.origin = origin;
        element->bounds.size = cocos2d::Size(width, height);

        txAtlas->pushElement(element->elementAtlasIdx, element);

        if (in->getInput()->getHeader().getMajorVersion() >= 4)
        {
            element->scale *= txAtlas->getScale();
            
            char hasScale9Grid = in->readUByte();

            if (hasScale9Grid)
            {
                cocos2d::Rect scale9GridRect;
                PrimitiveDeserializer::deserialize(in, &scale9GridRect);
            }
        }
    }

    timeline->pushTextureAtlas(txAtlas);
}

NS_GAF_END
