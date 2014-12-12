#include "GAFPrecompiled.h"
#include "GAFTimeline.h"
#include "GAFTextureAtlas.h"
#include "GAFAnimationFrame.h"
#include "GAFTextData.h"

NS_GAF_BEGIN

GAFTimeline::GAFTimeline(GAFTimeline* parent, uint32_t id, const cocos2d::Rect& aabb, cocos2d::Point& pivot, uint32_t framesCount) :
m_id(id)
, m_aabb(aabb)
, m_pivot(pivot)
, m_framesCount(framesCount)
, m_parent(parent)
, m_sceneFps(0)
, m_sceneWidth(0)
, m_sceneHeight(0)
{

}

GAFTimeline::~GAFTimeline()
{
    GAF_RELEASE_ARRAY(TextureAtlases_t, m_textureAtlases);
    GAF_RELEASE_ARRAY(AnimationFrames_t, m_animationFrames);
    GAF_RELEASE_MAP(TextsData_t, m_textsData);
}

void GAFTimeline::pushTextureAtlas(GAFTextureAtlas* atlas)
{
    m_textureAtlases.push_back(atlas);
}

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

void GAFTimeline::pushAnimationSequence(const std::string& nameId, int start, int end)
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

void GAFTimeline::pushTextData(uint32_t objectIdRef, GAFTextData* textField)
{
    m_textsData[objectIdRef] = textField;
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

const AnimationFrames_t& GAFTimeline::getAnimationFrames() const
{
    return m_animationFrames;
}

const AnimationSequences_t& GAFTimeline::getAnimationSequences() const
{
    return m_animationSequences;
}

const NamedParts_t& GAFTimeline::getNamedParts() const
{
	return m_namedParts;
}

TextsData_t const& GAFTimeline::getTextsData() const
{
    return m_textsData;
}

const GAFAnimationSequence* GAFTimeline::getSequence(const std::string& name) const
{
    AnimationSequences_t::const_iterator it = m_animationSequences.find(name);

    if (it != m_animationSequences.end())
    {
        return &it->second;
    }

    return nullptr;
}

const GAFAnimationSequence * GAFTimeline::getSequenceByLastFrame(size_t frame) const
{
    if (m_animationSequences.empty())
    {
        return nullptr;
    }

    for (AnimationSequences_t::const_iterator i = m_animationSequences.begin(), e = m_animationSequences.end(); i != e; ++i)
    {
        if (i->second.endFrameNo == frame + 1)
        {
            return &i->second;
        }
    }

    return nullptr;
}

const GAFAnimationSequence * GAFTimeline::getSequenceByFirstFrame(size_t frame) const
{
    if (m_animationSequences.empty())
    {
        return nullptr;
    }

    for (AnimationSequences_t::const_iterator i = m_animationSequences.begin(), e = m_animationSequences.end(); i != e; ++i)
    {
        if (i->second.startFrameNo == frame)
        {
            return &i->second;
        }
    }

    return nullptr;
}

GAFTextureAtlas* GAFTimeline::getTextureAtlas()
{
    return m_currentTextureAtlas;
}

void GAFTimeline::setLinkageName(const std::string &linkageName)
{
    m_linkageName = linkageName;
}

uint32_t GAFTimeline::getFramesCount() const
{
    return m_framesCount;
}

const cocos2d::Rect GAFTimeline::getRect() const
{
    return m_aabb;
}

const cocos2d::Point GAFTimeline::getPivot() const
{
    return m_pivot;
}

const std::string GAFTimeline::getLinkageName() const
{
    return m_linkageName;
}

GAFTimeline* GAFTimeline::getParent() const
{
    return m_parent;
}

void GAFTimeline::loadImages()
{
    if (m_textureAtlases.empty())
    {
        m_currentTextureAtlas = nullptr;
        return;
    }
    _chooseTextureAtlas();

}

void GAFTimeline::_chooseTextureAtlas()
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

    m_usedAtlasContentScaleFactor = atlasScale;
}

float GAFTimeline::usedAtlasContentScaleFactor() const
{
    return m_usedAtlasContentScaleFactor;
}

NS_GAF_END