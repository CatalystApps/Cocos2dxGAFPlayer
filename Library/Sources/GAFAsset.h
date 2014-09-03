#pragma once

#include "GAFCollections.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "GAFAssetTextureManager.h"

#include "GAFDelegates.h"

class GAFTextureAtlas;
class GAFAnimatedObject;
class GAFAnimationSequence;
class GAFAnimationFrame;

class GAFLoader;

class GAFAsset : public cocos2d::Ref
{
private:
    AnimationFrames_t       m_animationFrames;
    AnimationSequences_t    m_animationSequences;
    GAFHeader               m_header;
	Timelines_t				m_timelines;
    GAFTimeline*            m_rootTimeline;

    GAFTextureLoadDelegate* m_textureLoadDelegate;
	GAFAssetTextureManager	m_textureManager;

    unsigned int            m_sceneFps;
    unsigned int            m_sceneWidth;
    unsigned int            m_sceneHeight;
    cocos2d::Color4B        m_sceneColor;

private:
    int _majorVersion;
    int _minorVersion;
public:
    /// Initializes asset with bGAF data

    bool                        initWithGAFFile(const std::string& filePath, GAFTextureLoadDelegate* delegate = NULL);

    bool                        initWithGAFBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate* delegate = NULL);

	void                        pushAnimationFrame(GAFAnimationFrame* frame);
    void                        pushAnimationSequence(const std::string nameId, int start, int end);
	void						pushTimeline(uint32_t timelineIdRef, GAFTimeline* t);

    void                        setHeader(GAFHeader& h);
    const GAFHeader&            getHeader() const;
    
    void                        setRootTimeline(GAFTimeline* tl);
    GAFTimeline*                getRootTimeline() const;

	const AnimationFrames_t&    getAnimationFrames() const;
	const Timelines_t&			getTimelines() const;
    Timelines_t&                getTimelines();

    /// get all of the sequences
    const AnimationSequences_t& getAnimationSequences() const;

    static GAFAsset*            createWithBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate* delegate = NULL);
    static GAFAsset*            create(const std::string& gafFilePath, GAFTextureLoadDelegate* delegate = NULL);

    GAFAsset();
    ~GAFAsset();
    /// total number of frames in animation
    size_t                      getAnimationFramesCount() const;

    /// get GAFAnimationSequence by name specified in editor
    const GAFAnimationSequence* getSequence(const std::string& name) const;

    /// get GAFAnimationSequence by last frame number in sequence	
    const GAFAnimationSequence* getSequenceByLastFrame(size_t frame) const;

    /// get GAFAnimationSequence by first frame number in sequence	
    const GAFAnimationSequence* getSequenceByFirstFrame(size_t frame) const;
    /// List of GAFAnimationFrame objects	
    static bool                 isAssetVersionPlayable(const char * version);

    GAFAnimatedObject *         createObject();
    GAFAnimatedObject *         createObjectAndRun(bool looped = false);

    /// desired content scale factor
    static float                desiredCsf();
    /// sets desired content scale factor
    static void                 setDesiredCsf(float csf);

    void                        setTextureLoadDelegate(GAFTextureLoadDelegate* delegate);
    
	GAFAssetTextureManager& getTextureManager();

    const unsigned int getSceneFps() const;
    const unsigned int getSceneWidth() const;
    const unsigned int getSceneHeight() const;
    const cocos2d::Color4B& getSceneColor() const;
    void setSceneFps(unsigned int);
    void setSceneWidth(unsigned int);
    void setSceneHeight(unsigned int);
    void setSceneColor(const cocos2d::Color4B&);
};
