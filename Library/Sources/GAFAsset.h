#pragma once

#ifndef __GAF_ASSET_H__
#define __GAF_ASSET_H__

#include "GAFCollections.h"
#include "GAFHeader.h"

#include "GAFDelegates.h"

class GAFTextureAtlas;
class GAFAnimatedObject;
class GAFAnimationSequence;
class GAFAnimationFrame;

class GAFLoader;

class GAFAsset : public cocos2d::CCObject
{
private:
    TextureAtlases_t        m_textureAtlases;
    AnimationMasks_t        m_animationMasks;
    AnimationObjects_t      m_animationObjects;
    AnimationFrames_t       m_animationFrames;
    AnimationSequences_t    m_animationSequences;
    NamedParts_t            m_namedParts;
    GAFHeader               m_header;

    GAFTextureAtlas*        m_currentTextureAtlas;
    GAFTextureLoadDelegate* m_textureLoadDelegate;

    unsigned int            m_sceneFps;
    unsigned int            m_sceneWidth;
    unsigned int            m_sceneHeight;
    cocos2d::ccColor4B      m_sceneColor;

private:
    float                   m_usedAtlasContentScaleFactor;
    int                     m_majorVersion;
    int                     m_minorVersion;

public:
    /// Initializes asset with bGAF data

    bool                        initWithGAFFile(const std::string& filePath, GAFTextureLoadDelegate* delegate = NULL);

    void                        pushTextureAtlas(GAFTextureAtlas* atlas);
    void                        pushAnimationMask(unsigned int objectId, unsigned int elementAtlasIdRef);
    void                        pushAnimationObjects(unsigned int objectId, unsigned int elementAtlasIdRef);
    void                        pushAnimationFrame(GAFAnimationFrame* frame);
    void                        pushAnimationSequence(const std::string nameId, int start, int end);
    void                        pushNamedPart(unsigned int objectIdRef, const std::string& name);

    void                        setHeader(GAFHeader& h);
    const GAFHeader&            getHeader() const;

    const AnimationObjects_t&   getAnimationObjects() const;
    const AnimationMasks_t&     getAnimationMasks() const;
    const AnimationFrames_t&    getAnimationFrames() const;
    const NamedParts_t&         getNamedParts() const;

    /// get all of the sequences
    const AnimationSequences_t& getAnimationSequences() const;

    static GAFAsset*            create(const std::string& gafFilePath, GAFTextureLoadDelegate* delegate = NULL);

    GAFAsset();
    ~GAFAsset();
    /// total number of frames in animation
    int                         animationFramesCount() const;

    GAFTextureAtlas *           textureAtlas();

    /// get GAFAnimationSequence by name specified in editor
    const GAFAnimationSequence* getSequence(const std::string& name) const;

    /// get GAFAnimationSequence by last frame number in sequence	
    const GAFAnimationSequence* getSequenceByLastFrame(int frame) const;

    /// get GAFAnimationSequence by first frame number in sequence	
    const GAFAnimationSequence* getSequenceByFirstFrame(int frame) const;
    /// List of GAFAnimationFrame objects	
    static bool                 isAssetVersionPlayable(const char * version);

    GAFAnimatedObject *         createObject();
    GAFAnimatedObject *         createObjectAndRun(bool looped = false);

    /// desired content scale factor
    static float                desiredCsf();
    /// sets desired content scale factor
    static void                 setDesiredCsf(float csf);
    /// used content scale factor
    float                       usedAtlasContentScaleFactor()const;

    void                        setTextureLoadDelegate(GAFTextureLoadDelegate* delegate);
    
    const unsigned int          getSceneFps() const;
    const unsigned int          getSceneWidth() const;
    const unsigned int          getSceneHeight() const;
    const cocos2d::ccColor4B&   getSceneColor() const;
    void                        setSceneFps(unsigned int);
    void                        setSceneWidth(unsigned int);
    void                        setSceneHeight(unsigned int);
    void                        setSceneColor(const cocos2d::ccColor4B&);
};


#endif // __GAF_ASSET_H__
