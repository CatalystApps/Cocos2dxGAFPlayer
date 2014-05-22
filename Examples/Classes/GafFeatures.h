#ifndef __GafFeatures_SCENE_H__
#define __GafFeatures_SCENE_H__

#include "cocos2d.h"

#include <string>
#include <vector>

#include "GAFAnimation.h"

#include "GAFDelegates.h"

class GAFAnimatedObject;

class GafFeatures : public cocos2d::Layer, public GAFSequenceDelegate, public GAFTextureLoadDelegate
{
public:
    typedef std::pair<cocos2d::MenuItemImage*, cocos2d::Label*> MenuItemPair_t;
private:

    enum BtnType
    {
        BtnEmpty,
        BtnPlus,
        BtnMinus
    };

    MenuItemPair_t m_nextSeq, m_prevSeq;
    void enableSequenceControllers(bool value);

    typedef std::vector<std::string> ObjectSequencesNames_t;
    ObjectSequencesNames_t  m_objectSequencesNames;
    int m_currentSequence;

    cocos2d::Label* m_loadingTimeLabel;
    cocos2d::EventListenerTouchAllAtOnce* m_touchlistener;
public:
    GafFeatures();
    ~GafFeatures();

    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::Scene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GafFeatures);

    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);

    MenuItemPair_t addBtn(const char * text, float px, float py, const cocos2d::ccMenuCallback& clb, float k, BtnType btnType);

    void black(cocos2d::Ref*);
    void white(cocos2d::Ref*);
    void gray(cocos2d::Ref*);
    void addOne(cocos2d::Ref*);
    void prevFrame(cocos2d::Ref*);
    void nextFrame(cocos2d::Ref*);
    void removeOne(cocos2d::Ref*);
    void toggleReverse(cocos2d::Ref*);
    void set1(cocos2d::Ref*);
    void set5(cocos2d::Ref*);
    void set10(cocos2d::Ref*);
    void set(int n);
    void restart(cocos2d::Ref*);
    void playpause(cocos2d::Ref*);
    void cleanup(cocos2d::Ref*);
    void next_anim(cocos2d::Ref*);
    void prev_anim(cocos2d::Ref*);
    int maxFrameNumber();
    void setFrameNumber(int aFrameNumber);
    int frameNumber();
    void addObjectsToScene(int aCount);
    void removeFromScene(int aCount);

    void nextSequence(cocos2d::Ref*);
    void prevSequence(cocos2d::Ref*);

    virtual void onFinishSequence(GAFAnimatedObject * object, const std::string& sequenceName);

    virtual void onTexturePreLoad(std::string& path);

private:	
    GAFAsset*                m_asset;
    cocos2d::__Array*        m_objects;
    std::vector<std::string> m_files;
    int                      m_anim_index;

};

#endif // __GafFeatures_SCENE_H__
