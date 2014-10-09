#include "GAFPrecompiled.h"
#include "TagDefineStage.h"

#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "PrimitiveDeserializer.h"

using namespace cocos2d;

void TagDefineStage::read(GAFStream* in, GAFAsset* ctx)
{
    ccColor4B color;
    
    unsigned int fps = in->readU8();  // Scene FPS
    PrimitiveDeserializer::deserialize(in, &color);  // Scene color
    unsigned int width = in->readU16();  // Scene width
    unsigned int height = in->readU16();  // Scene height
    
    ctx->setSceneFps(fps);
    ctx->setSceneColor(color);
    ctx->setSceneWidth(width);
    ctx->setSceneHeight(height);
}
