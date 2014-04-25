#ifndef __GafFeatures_SCENE_H__
#define __GafFeatures_SCENE_H__

#include "cocos2d.h"

#include <string>
#include <vector>

#include "GAFAnimation.h"

#include "GAFDelegates.h"

using namespace cocos2d;
class GAFAnimatedObject;

class GafFeatures : public cocos2d::CCLayer, public GAFSequenceDelegate, public GAFTextureLoadDelegate
{
public:
    typedef std::pair<CCMenuItemImage*, CCLabelTTF*> MenuItemPair_t;
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
    unsigned int m_currentSequence;

    CCLabelTTF* m_loadingTimeLabel;
public:
    GafFeatures();
    ~GafFeatures();

    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GafFeatures);

    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);

    MenuItemPair_t addBtn(const char * text, float px, float py, SEL_MenuHandler handler, float k, BtnType btnType);

    void black(CCObject*);
    void white(CCObject*);
    void gray(CCObject*);
    void addOne(CCObject*);
    void prevFrame(CCObject*);
    void nextFrame(CCObject*);
    void removeOne(CCObject*);
    void set1(CCObject*);
    void set5(CCObject*);
    void toggleReverse(CCObject*);
    void set(int n);
    void restart(CCObject*);
    void playpause(CCObject*);
    void cleanup(CCObject*);
    void next_anim(CCObject*);
    void prev_anim(CCObject*);
    int maxFrameNumber();
    void setFrameNumber(int aFrameNumber);
    int frameNumber();
    void addObjectsToScene(int aCount);
    void removeFromScene(int aCount);

    void nextSequence(CCObject*);
    void prevSequence(CCObject*);

    virtual void onFinishSequence(GAFAnimatedObject * object, const std::string& sequenceName);

    virtual void onTexturePreLoad(std::string& path);

private:
    GAFAsset*              m_asset;
    CCArray*               m_objects;
    std::vector<std::string> m_files;
    int                     m_anim_index;

};

#endif // __GafFeatures_SCENE_H__
