#pragma once

#include "GAFCollections.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"

#include "GAFDelegates.h"

NS_GAF_BEGIN

class GAFTextureAtlas;
class GAFObject;
class GAFAssetTextureManager;

class GAFLoader;

class GAFAsset : public cocos2d::Ref
{
private:
    GAFHeader               m_header;
	Timelines_t				m_timelines;
    GAFTimeline*            m_rootTimeline;

    GAFTextureLoadDelegate_t m_textureLoadDelegate;
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

    bool                        initWithGAFFile(const std::string& filePath, GAFTextureLoadDelegate_t delegate);

    bool                        initWithGAFBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate_t delegate);

	void						pushTimeline(uint32_t timelineIdRef, GAFTimeline* t);

    void                        setHeader(GAFHeader& h);
    const GAFHeader&            getHeader() const;
    
    void                        setRootTimeline(GAFTimeline* tl);
    void                        setRootTimelineWithName(const std::string& name);
    GAFTimeline*                getRootTimeline() const;
    GAFTimeline*                getTimelineByName(const std::string& name) const;

	const Timelines_t&			getTimelines() const;
    Timelines_t&                getTimelines();

    static GAFAsset*            createWithBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate_t delegate);
    static GAFAsset*            create(const std::string& gafFilePath, GAFTextureLoadDelegate_t delegate);
    static GAFAsset*            create(const std::string& gafFilePath);

    GAFAsset();
    ~GAFAsset();

    /// List of GAFAnimationFrame objects	
    static bool                 isAssetVersionPlayable(const char * version);

    GAFObject*                  createObject();
    GAFObject*                  createObjectAndRun(bool looped = false);

    /// desired content scale factor
    static float                desiredCsf();
    /// sets desired content scale factor
    static void                 setDesiredCsf(float csf);

    void                        setTextureLoadDelegate(GAFTextureLoadDelegate_t delegate);
    
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

NS_GAF_END