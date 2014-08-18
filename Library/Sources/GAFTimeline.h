#pragma once

#include "GAFCollections.h"
#include "GAFHeader.h"

#include "GAFDelegates.h"

class GAFTextureAtlas;

class GAFTimeline : public cocos2d::Ref
{
private:
    TextureAtlases_t        m_textureAtlases;
    AnimationMasks_t        m_animationMasks;
    AnimationObjects_t      m_animationObjects;
    AnimationFrames_t       m_animationFrames;
    AnimationSequences_t    m_animationSequences;
    NamedParts_t            m_namedParts;

    uint32_t                m_id;
    cocos2d::Rect           m_aabb;
    cocos2d::Point          m_pivot;

    unsigned int            m_sceneFps = 0;
    unsigned int            m_sceneWidth = 0;
    unsigned int            m_sceneHeight = 0;
    cocos2d::Color4B        m_sceneColor;
    
    uint32_t                m_framesCount;
    
    std::string             m_linkageName;
    
    GAFTextureAtlas*        m_currentTextureAtlas;
    GAFTextureLoadDelegate* m_textureLoadDelegate;

    void                    _chooseTextureAtlas();
public:

    GAFTimeline(uint32_t id, const cocos2d::Rect& aabb, cocos2d::Point& pivot, uint32_t framesCount);
    virtual ~GAFTimeline();

    void                        pushTextureAtlas(GAFTextureAtlas* atlas);
    void                        pushAnimationMask(unsigned int objectId, unsigned int elementAtlasIdRef, CharacterType charType);
    void                        pushAnimationObject(uint32_t objectId, uint32_t elementAtlasIdRef, CharacterType charType);
    void                        pushAnimationFrame(GAFAnimationFrame* frame);
    void                        pushAnimationSequence(const std::string nameId, int start, int end);
    void                        pushNamedPart(unsigned int objectIdRef, const std::string& name);

    void                        setSceneFps(unsigned int);
    void                        setSceneWidth(unsigned int);
    void                        setSceneHeight(unsigned int);
    void                        setSceneColor(const cocos2d::Color4B&);
    
    void                        setLinkageName(const std::string& linkageName);
    
    const AnimationObjects_t&   getAnimationObjects() const;
    const AnimationMasks_t&     getAnimationMasks() const;
    uint32_t                    getFramesCount() const;
    
    void                        loadImages();
};