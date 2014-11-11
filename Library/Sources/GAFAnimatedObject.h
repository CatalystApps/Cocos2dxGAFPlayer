#pragma once

#include "GAFAnimation.h"
#include "GAFCollections.h"

#include "GAFDelegates.h"

NS_GAF_BEGIN

class GAFAnimation;
class GAFSprite;
class GAFAnimator;
class GAFAnimatedObject;
class GAFAsset;

enum PCAnimationFPSType // Obsolete. Will be removed
{
    kGAFAnimationFPSType_15 = 15,
    kGAFAnimationFPSType_30 = 30,
    kGAFAnimationFPSType_60 = 60
};

enum GAFAnimatedObjectControlFlags
{
    kGAFAnimatedObjectControl_None = 0,
    // If specified, state of controlled object will be changed every frame (like it is by default) and then
    // animatedObject:didDisplayFrameWithSubobject: will be called
    kGAFAnimatedObjectControl_ApplyState = 1 << 0
};


class GAFAnimatedObject : public cocos2d::Layer, public GAFAnimation
{
protected:
    GAFAnimatedObject();
private:
    Node* m_container;
    GAFAsset * m_asset;

    SubObjects_t m_subObjects;
    MaskObjectsList_t m_masks;
    
    CaptureObjects_t m_capturedObjects;
    bool _animationsSelectorScheduled;
    GAFFramePlayedDelegate * m_framePlayedDelegate;
    GAFAnimatedObjectControlDelegate * m_controlDelegate;

    int numberOfGlobalFramesForOneAnimationFrame();
    int m_extraFramesCounter;

    double m_timeDelta;

    PCAnimationFPSType _FPSType; // Obsolete. Will be removed
    int m_fps;

    void _constructObject();
    GAFSprite* createObject(unsigned int elementId, unsigned int atlasId, bool isMask);
    void realizeFrame(cocos2d::Node* out, size_t frameIndex);
public:
    ~GAFAnimatedObject();
    static GAFAnimatedObject * create(GAFAsset * anAsset);
    static GAFAnimatedObject * createAndRun(const std::string& gafPath, bool looped = false);

    
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
    bool init(GAFAsset * anAsset);
    void processAnimations(float dt);
    cocos2d::Vect pupilCoordinatesWithXSemiaxis(float anXSemiaxis, float anYSemiaxis, cocos2d::Vect aCenter, cocos2d::Vect anExternalPoint);
    void removeAllSubObjects();
    
    void instantiateObject(const AnimationObjects_t& objs, const AnimationMasks_t& masks);
    
    const SubObjects_t& getSubojects() const;

    void animatorDidPlayedFrame(GAFAnimator * anAnimator, int aFrameNo);

    virtual void start();
    virtual void stop();
    virtual void processAnimation();
    /// Takes control over subobject, which means that every frame control delegate will be notified to decide
    /// what to do with captured external object
    /// @note it supports only objects for now, DOES NOT SUPPORT MASKS
    /// @param subobject name taken from "namedParts" section in config
    /// @param controlFlags flags specifying what played will do with subobjects controlled externally
    /// @returns YES if control was successfully taken and all future
    bool captureControlOverSubobjectNamed(const char * aName, GAFAnimatedObjectControlFlags aControlFlags);
    bool captureControlOverSubobject(unsigned int id, GAFAnimatedObjectControlFlags aControlFlags);
    /// Releases control over subobject captured earlier	
    void releaseControlOverSubobjectNamed(const char * aName);

    void setFramePlayedDelegate(GAFFramePlayedDelegate * delegate);
    void setControlDelegate(GAFAnimatedObjectControlDelegate * delegate);
    cocos2d::Rect realBoundingBoxForCurrentFrame();

    /// Returns subobject by it id
    GAFSprite * subObjectForInnerObjectId(unsigned int anInnerObjectId);
    /// Returns object id by it name, defined in NamedParts
    unsigned int objectIdByObjectName(const std::string& aName);

    cocos2d::Sprite* renderCurrentFrameToTexture(bool usePOTTextures = false);
    void rearrangeSubobject(cocos2d::Node* out, cocos2d::Node* child, int zIndex, size_t frame, bool visible);
    int getFps() const;

    void setFps(int value);
}; // GAFAnimatedObject

NS_GAF_END
