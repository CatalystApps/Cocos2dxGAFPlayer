#include "GAFPrecompiled.h"
#include "GAFTimeline.h"
#include "GAFTextureAtlas.h"
#include "GAFAnimationFrame.h"

GAFTimeline::GAFTimeline(uint32_t id, const cocos2d::Rect& aabb, cocos2d::Point& pivot, uint32_t framesCount):
m_id(id)
, m_aabb(aabb)
, m_pivot(pivot)
, m_framesCount(framesCount)
{

}

GAFTimeline::~GAFTimeline()
{
    //GAF_RELEASE_ARRAY(TextureAtlases_t, m_textureAtlases);
    GAF_RELEASE_ARRAY(AnimationFrames_t, m_animationFrames);
}

/*void GAFTimeline::pushTextureAtlas(GAFTextureAtlas* atlas)
{
    m_textureAtlases.push_back(atlas);
}*/

void GAFTimeline::pushAnimationMask(unsigned int objectId, unsigned int elementAtlasIdRef, GAFCharacterType charType)
{
    m_animationMasks[objectId] = std::make_tuple(elementAtlasIdRef, charType);
}

void GAFTimeline::pushAnimationObject(uint32_t objectId, uint32_t elementAtlasIdRef, GAFCharacterType charType)
{
    m_animationObjects[objectId] = std::make_tuple(elementAtlasIdRef, charType);
}

void GAFTimeline::pushAnimationFrame(GAFAnimationFrame* frame)
{
    m_animationFrames.push_back(frame);
}

void GAFTimeline::pushAnimationSequence(const std::string nameId, int start, int end)
{
    GAFAnimationSequence seq;
    seq.name = nameId;
    seq.startFrameNo = start;
    seq.endFrameNo = end;

    m_animationSequences[nameId] = seq;
}

void GAFTimeline::pushNamedPart(unsigned int objectIdRef, const std::string& name)
{
    m_namedParts[name] = objectIdRef;
}

void GAFTimeline::setSceneFps(unsigned int v)
{
    m_sceneFps = v;
}

void GAFTimeline::setSceneWidth(unsigned int v)
{
    m_sceneWidth = v;
}

void GAFTimeline::setSceneHeight(unsigned int v)
{
    m_sceneHeight = v;
}

void GAFTimeline::setSceneColor(const cocos2d::Color4B& v)
{
    m_sceneColor = v;
}

const AnimationObjects_t& GAFTimeline::getAnimationObjects() const
{
    return m_animationObjects;
}

const AnimationMasks_t& GAFTimeline::getAnimationMasks() const
{
    return m_animationMasks;
}

/*const TextureAtlases_t& GAFTimeline::getTextureAtlases() const
{
	return m_textureAtlases;
}*/

void GAFTimeline::setLinkageName(const std::string &linkageName)
{
    m_linkageName = linkageName;
}

uint32_t GAFTimeline::getFramesCount() const
{
    return m_framesCount;
}

/*void GAFTimeline::loadImages()
{
    _chooseTextureAtlas();
    
}

/*void GAFTimeline::_chooseTextureAtlas()
{
    float atlasScale = m_textureAtlases[0]->getScale();
    
    m_currentTextureAtlas = m_textureAtlases[0];
    
    const size_t count = m_textureAtlases.size();
    
	//TODO: check
	float _currentDeviceScale = 1.f;

    for (size_t i = 1; i < count; ++i)
    {
        float as = m_textureAtlases[i]->getScale();
        if (fabs(atlasScale - _currentDeviceScale) > fabs(as - _currentDeviceScale))
        {
            m_currentTextureAtlas = m_textureAtlases[i];
            atlasScale = as;
        }
    }
    
    //_usedAtlasContentScaleFactor = atlasScale;
}*/
