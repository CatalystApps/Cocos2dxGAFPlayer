#pragma once

#include "GAFCollections.h"
#include "GAFResourcesInfo.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "GAFTextureAtlas.h"

#include "GAFDelegates.h"

NS_GAF_BEGIN

class GAFTextureAtlas;
class GAFObject;
class GAFAssetTextureManager;
class GAFTimelineAction;

class GAFLoader;

class GAFAsset : public cocos2d::Ref
{
    friend class GAFObject;
private:
    GAFHeader               m_header;
	Timelines_t				m_timelines;
    GAFTimeline*            m_rootTimeline;
    SoundInfos_t            m_soundInfos;
    TextureAtlases_t        m_textureAtlases; // custom regions
    GAFTextureAtlas*        m_currentTextureAtlas;

    void setRootTimeline(GAFTimeline* tl);

    void parseReferences(std::vector<GAFResourcesInfo*> &dest);
    void loadTextures(const std::string& filePath, GAFTextureLoadDelegate_t delegate, cocos2d::ZipFile* bundle = nullptr);
    void _chooseTextureAtlas(float desiredAtlasScale);
    GAFTextureLoadDelegate_t m_textureLoadDelegate;
	GAFAssetTextureManager*	m_textureManager;

    GAFSoundDelegate_t m_soundDelegate;

    unsigned int            m_sceneFps;
    unsigned int            m_sceneWidth;
    unsigned int            m_sceneHeight;
    cocos2d::Color4B        m_sceneColor;

    float                   m_desiredAtlasScale;

    std::string             m_gafFileName;

    enum class State : uint8_t
    {
        Normal = 0,
        DryRun
    };
    State                   m_state; // avoid to pass this parameter to public methods to prevent usage

private:
    int _majorVersion;
    int _minorVersion;
public:
    /// Initializes asset with bGAF data

    bool                        initWithGAFFile(const std::string& filePath, GAFTextureLoadDelegate_t delegate, GAFLoader* customLoader = nullptr);

    bool                        initWithGAFBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate_t delegate, GAFLoader* customLoader = nullptr);

	void						pushTimeline(uint32_t timelineIdRef, GAFTimeline* t);
    void                        pushSound(uint32_t id, GAFSoundInfo* sound);
    void                        soundEvent(GAFTimelineAction *action);

    void                        pushTextureAtlas(GAFTextureAtlas* atlas);

    void                        setHeader(GAFHeader& h);
    const GAFHeader&            getHeader() const;
    
    bool                        setRootTimeline(const std::string& name);
    bool                        setRootTimeline(uint32_t id);
    GAFTimeline*                getRootTimeline() const;
    GAFTimeline*                getTimelineByName(const std::string& name) const;

	const Timelines_t&			getTimelines() const;
    Timelines_t&                getTimelines();

    static GAFAsset*            createWithBundle(const std::string& zipfilePath, const std::string& entryFile, GAFTextureLoadDelegate_t delegate, GAFLoader* customLoader = nullptr);
    static GAFAsset*            createWithBundle(const std::string& zipfilePath, const std::string& entryFile);
    static GAFAsset*            create(const std::string& gafFilePath, GAFTextureLoadDelegate_t delegate, GAFLoader* customLoader = nullptr);
    static GAFAsset*            create(const std::string& gafFilePath);

    static void                 getResourceReferences(const std::string& gafFilePath, std::vector<GAFResourcesInfo*> &dest);
    static void                 getResourceReferencesFromBundle(const std::string& zipfilePath, const std::string& entryFile, std::vector<GAFResourcesInfo*> &dest);
    
    void                        useExternalTextureAtlas(std::vector<cocos2d::Texture2D*>& textures, GAFTextureAtlas::Elements_t& elements);

    GAFTextureAtlas*            getTextureAtlas();
    void                        loadImages(float desiredAtlasScale);
    GAFSprite*                  getCustomRegion(const std::string& name);

    GAFAsset();
    ~GAFAsset();

    /// List of GAFAnimationFrame objects	
    static bool                 isAssetVersionPlayable(const char * version);

    GAFObject*                  createObject();
    GAFObject*                  createObjectAndRun(bool looped);

    /// Desired atlas scale. Default is 1.0f
    float                       desiredAtlasScale();
    /// Sets desired atlas scale. Will choose nearest atlas scale from available
    void                        setDesiredAtlasScale(float scale);

    void                        setTextureLoadDelegate(GAFTextureLoadDelegate_t delegate);
    void                        setSoundDelegate(GAFSoundDelegate_t delagate);

    GAFAssetTextureManager*     getTextureManager();

    const unsigned int getSceneFps() const;
    const unsigned int getSceneWidth() const;
    const unsigned int getSceneHeight() const;
    const cocos2d::Color4B& getSceneColor() const;
    void setSceneFps(unsigned int);
    void setSceneWidth(unsigned int);
    void setSceneHeight(unsigned int);
    void setSceneColor(const cocos2d::Color4B&);

    const std::string&          getGAFFileName() const;
};

NS_GAF_END