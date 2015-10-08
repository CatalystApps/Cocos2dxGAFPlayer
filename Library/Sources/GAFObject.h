#pragma once

#include "GAFDelegates.h"
#include "GAFSprite.h"
#include "GAFCollections.h"
#include "GAFTextureAtlas.h"

NS_GAF_BEGIN

class GAFAsset;
class GAFTimeline;

class GAFObject : public GAFSprite
{
private:
    const cocos2d::AffineTransform AffineTransformFlashToCocos(const cocos2d::AffineTransform& aTransform);

public:

    typedef std::vector<GAFObject*> DisplayList_t;
    typedef std::vector<cocos2d::ClippingNode*> MaskList_t;
private:
    GAFSequenceDelegate_t                   m_sequenceDelegate;
    GAFAnimationFinishedPlayDelegate_t      m_animationFinishedPlayDelegate;
    GAFAnimationStartedNextLoopDelegate_t   m_animationStartedNextLoopDelegate;
    GAFFramePlayedDelegate_t                m_framePlayedDelegate;
    
    cocos2d::Node*                          m_container;

    uint32_t                                m_totalFrameCount;
    uint32_t                                m_currentSequenceStart;
    uint32_t                                m_currentSequenceEnd;

    bool                                    m_isRunning;
    bool                                    m_isLooped;
    bool                                    m_isReversed;

    double                                  m_timeDelta;
    uint32_t                                m_fps;
    bool                                    m_skipFpsCheck;

    bool                                    m_animationsSelectorScheduled;

    bool                                    m_isInResetState;

private:
    void constructObject();
    GAFObject* _instantiateObject(uint32_t id, GAFCharacterType type, uint32_t reference, bool isMask);

protected:
    GAFObject*                              m_timelineParentObject;
    GAFAsset*                               m_asset;
    GAFTimeline*                            m_timeline;
    DisplayList_t                           m_displayList;
    MaskList_t                              m_masks;
    GAFCharacterType                        m_charType;
    GAFObjectType                           m_objectType;
    uint32_t                                m_currentFrame;
    uint32_t                                m_showingFrame; // Frame number that is valid from the beginning of realize frame
    uint32_t                                m_lastVisibleInFrame; // Last frame that object was visible in
    Filters_t                               m_parentFilters;
    cocos2d::Vec4                           m_parentColorTransforms[2];

    bool                                    m_isManualColor;

    void    setTimelineParentObject(GAFObject* obj) { m_timelineParentObject = obj; }

    void    processAnimation();
    void    processAnimations(float dt);

    void    instantiateObject(const AnimationObjects_t& objs, const AnimationMasks_t& masks);

    GAFObject*   encloseNewTimeline(uint32_t reference);

    void        step();
    bool        isCurrentFrameLastInSequence() const;
    uint32_t    nextFrame();

public:
    GAFObject();

    /// @note do not forget to call setSequenceDelegate(nullptr) before deleting your subscriber
    void setSequenceDelegate(GAFSequenceDelegate_t delegate);

    /// @note do not forget to call setAnimationFinishedPlayDelegate(nullptr) before deleting your subscriber
    void setAnimationFinishedPlayDelegate(GAFAnimationFinishedPlayDelegate_t delegate);

    /// @note do not forget to call setAnimationStartedNextLoopDelegate(nullptr) before deleting your subscriber
    void setAnimationStartedNextLoopDelegate(GAFAnimationStartedNextLoopDelegate_t delegate);

    /// @note do not forget to call setFramePlayedDelegate(nullptr) before deleting your subscriber
    void setFramePlayedDelegate(GAFFramePlayedDelegate_t delegate);

#if COCOS2D_VERSION < 0x00030200
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool flags) override;
#else
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
#endif

#if COCOS2D_VERSION < 0x00030200
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool flags) override
#else
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override
#endif
    {
        (void)flags;
        (void)renderer;
        (void)transform;
    }

    void useExternalTextureAtlas(std::vector<cocos2d::Texture2D*>& textures, GAFTextureAtlas::Elements_t& elements);
public:
    // Playback accessing
    void        start();
    void        stop();

    /// Pauses animation including enclosed timelines
    void        pauseAnimation();

    /// Resumes animation including enclosed timelines
    void        resumeAnimation();


    bool        isDone() const;
    bool        getIsAnimationRunning() const;

    bool        isLooped() const;
    void        setLooped(bool looped, bool recursive = false);

    bool        isReversed() const;
    void        setReversed(bool reversed, bool fromCurrentFrame = true);

    uint32_t    getTotalFrameCount() const;
    uint32_t    getCurrentFrameIndex() const;

    bool        setFrame(uint32_t index);

    /// Plays specified frame and then stops excluding enclosed timelines
    bool        gotoAndStop(const std::string& frameLabel);
    /// Plays specified frame and then stops excluding enclosed timelines
    bool        gotoAndStop(uint32_t frameNumber);

    /// Plays animation from specified frame excluding enclosed timelines
    bool        gotoAndPlay(const std::string& frameLabel);
    /// Plays animation from specified frame excluding enclosed timelines
    bool        gotoAndPlay(uint32_t frameNumber);

    uint32_t    getStartFrame(const std::string& frameLabel);
    uint32_t    getEndFrame(const std::string& frameLabel);

    /// Plays animation sequence with specified name
    /// @param name a sequence name
    /// @param looped if true - sequence should play in cycle
    /// @param resume if true - animation will be played immediately, if false - playback will be paused after the first frame is shown
    /// @param hint specific animation playback parameters

    bool        playSequence(const std::string& name, bool looped, bool resume = true);

    /// Stops playing an animation as a sequence
    void        clearSequence();

    /// schedule/unschedule
    /// @note this function is automatically called in start/stop
    void        enableTick(bool val);

    void        setAnimationRunning(bool value, bool recurcive);
public:

    virtual ~GAFObject();

    static GAFObject * create(GAFAsset * anAsset, GAFTimeline* timeline);

    bool init(GAFAsset * anAnimationData, GAFTimeline* timeline);

    bool hasSequences() const;

    bool isVisibleInCurrentFrame() const;

    cocos2d::Rect getBoundingBoxForCurrentFrame();

    const AnimationSequences_t& getSequences() const;
    GAFTimeline* getTimeLine() { return m_timeline; }
    DisplayList_t& getDisplayList() { return m_displayList; }
    const DisplayList_t& getDisplayList() const { return m_displayList; }

    virtual const cocos2d::Mat4& getNodeToParentTransform() const override;
    virtual cocos2d::AffineTransform getNodeToParentAffineTransform() const override;

    virtual void setColor(const cocos2d::Color3B& color) override;
    virtual void setOpacity(GLubyte opacity) override;

    //////////////////////////////////////////////////////////////////////////
    // Accessors

    // Searches for an object by given string
    // @param object name e.g. "head" or object path e.g. "knight.body.arm"
    // @note it can slow down the real-time performance
    // @returns instance of GAFObject or null. Warning: the instance could be invalidated when the system catches EVENT_COME_TO_FOREGROUND event
    GAFObject* getObjectByName(const std::string& name);
    const GAFObject* getObjectByName(const std::string& name) const;

    void realizeFrame(cocos2d::Node* out, uint32_t frameIndex);
    void rearrangeSubobject(cocos2d::Node* out, cocos2d::Node* child, int zIndex);

    uint32_t getFps() const;

    void setFps(uint32_t value);

    void setFpsLimitations(bool fpsLimitations);
};

NS_GAF_END