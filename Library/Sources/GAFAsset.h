#pragma once

#include "GAFCollections.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"

#include "GAFDelegates.h"

class GAFTextureAtlas;
class GAFAnimatedObject;
class GAFAssetTextureManager;

class GAFLoader;

class GAFAsset : public cocos2d::Ref
{
private:
    GAFHeader               m_header;
	Timelines_t				m_timelines;
    GAFTimeline*            m_rootTimeline;

    GAFTextureLoadDelegate* m_textureLoadDelegate;
	GAFAssetTextureManager*	m_textureManager;

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

	void						pushTimeline(uint32_t timelineIdRef, GAFTimeline* t);

    void                        setHeader(GAFHeader& h);
    const GAFHeader&            getHeader() const;
    
    void                        setRootTimeline(GAFTimeline* tl);
    GAFTimeline*                getRootTimeline() const;

	const Timelines_t&			getTimelines() const;
    Timelines_t&                getTimelines();

    static GAFAsset*            createWithBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate* delegate = NULL);
    static GAFAsset*            create(const std::string& gafFilePath, GAFTextureLoadDelegate* delegate = NULL);

    GAFAsset();
    ~GAFAsset();

    /// List of GAFAnimationFrame objects	
    static bool                 isAssetVersionPlayable(const char * version);

    GAFAnimatedObject *         createObject();
    GAFAnimatedObject *         createObjectAndRun(bool looped = false);

    /// desired content scale factor
    static float                desiredCsf();
    /// sets desired content scale factor
    static void                 setDesiredCsf(float csf);

    void                        setTextureLoadDelegate(GAFTextureLoadDelegate* delegate);
    
	GAFAssetTextureManager* getTextureManager();

    const unsigned int getSceneFps() const;
    const unsigned int getSceneWidth() const;
    const unsigned int getSceneHeight() const;
    const cocos2d::Color4B& getSceneColor() const;
    void setSceneFps(unsigned int);
    void setSceneWidth(unsigned int);
    void setSceneHeight(unsigned int);
    void setSceneColor(const cocos2d::Color4B&);
};
