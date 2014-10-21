#include "GAFPrecompiled.h"
#include "TagDefineStage.h"

#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "PrimitiveDeserializer.h"

NS_GAF_BEGIN

void TagDefineStage::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    (void)timeline;
    cocos2d::Color4B color;
    
    unsigned int fps = in->readU8();  // Scene FPS
    PrimitiveDeserializer::deserialize(in, &color);  // Scene color
    unsigned int width = in->readU16();  // Scene width
    unsigned int height = in->readU16();  // Scene height
    
	asset->setSceneFps(fps);
	asset->setSceneColor(color);
	asset->setSceneWidth(width);
	asset->setSceneHeight(height);
}

NS_GAF_END