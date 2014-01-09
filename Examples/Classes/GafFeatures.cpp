#include "GafFeatures.h"

#include "GAFAsset.h"
#include "GAFAnimatedObject.h"

using namespace cocos2d;

GafFeatures::GafFeatures()
    :
    m_asset(NULL),
    m_objects(NULL),
    m_currentSequence(0)
{
}
GafFeatures::~GafFeatures()
{
    CC_SAFE_RELEASE(m_asset);
    CC_SAFE_RELEASE(m_objects);
}

CCScene* GafFeatures::scene()
{
    CCScene *scene = CCScene::create();
    GafFeatures *layer = GafFeatures::create();
    scene->addChild(layer);
    return scene;
}

GafFeatures::MenuItemPair_t GafFeatures::addBtn(const char * text, float px, float py, SEL_MenuHandler handler, float k, BtnType btnType)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCMenuItemImage *res = NULL;

    if (btnType == BtnEmpty)
    {
        res = CCMenuItemImage::create(
            "button_on.png",
            "button_off.png",
            this,
            handler);
    }
    else if (btnType == BtnPlus)
    {
        res = CCMenuItemImage::create(
            "button_on_plus.png",
            "button_off_plus.png",
            this,
            handler);
    }
    else if (btnType == BtnMinus)
    {
        res = CCMenuItemImage::create(
            "button_on_minus.png",
            "button_off_minus.png",
            this,
            handler);
    }

    res->setPosition( ccp(size.width * px, size.height * py) );
    res->setScale(k);

    CCLabelTTF* pLabel = CCLabelTTF::create(text, "Marker Felt", 24);
    pLabel->setColor(ccc3(0, 0, 255));

    pLabel->setAnchorPoint(CCPoint(1, 0.5));
    pLabel->setScale(k);

    CCPoint labelPos = ccp(res->getPositionX() - res->getContentSize().width * k * 0.5, res->getPositionY());

    pLabel->setPosition( ccp(res->getPositionX() - res->getContentSize().width * k * 0.5, res->getPositionY()) );
    addChild(pLabel, 100000);

    MenuItemPair_t p = std::make_pair(res, pLabel);

    return p;
}

void GafFeatures::black(CCObject*)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GafFeatures::white(CCObject*)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GafFeatures::gray(CCObject*)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void GafFeatures::prevFrame(CCObject*)
{
    int f = frameNumber();
    if (-1 == f)
    {
        return;
    }
    if (f)
    {
        setFrameNumber(f - 1);
    }
}

void GafFeatures::nextFrame(CCObject*)
{
    int f = frameNumber();
    if (-1 == f)
    {
        return;
    }
    if (f != maxFrameNumber())
    {
        setFrameNumber(f + 1);
    }
}

bool GafFeatures::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GAFAsset::setDesiredCsf(CCDirector::sharedDirector()->getContentScaleFactor());
#endif
    CCSize size = CCDirector::sharedDirector()->getWinSizeInPixels();

    /*float s = size.height / 640.f;
    if (s > 1) s = 1.f;*/

    float s = 1.f;

    float dp = 0.13f;

    CCArray * items = CCArray::create();
    items->addObject(addBtn("Play/Pause"    , 0.95f, 0.95f,            menu_selector(GafFeatures::playpause), s, BtnEmpty).first);
    items->addObject(addBtn("Restart"       , 0.95f, 0.95f - dp,       menu_selector(GafFeatures::restart), s, BtnEmpty).first);
    items->addObject(addBtn("B"             , 0.75f, 0.95f - dp * 2.f, menu_selector(GafFeatures::black), s, BtnEmpty).first);
    items->addObject(addBtn("W"             , 0.85f, 0.95f - dp * 2.f, menu_selector(GafFeatures::white ), s, BtnEmpty).first);
    items->addObject(addBtn("G"             , 0.95f, 0.95f - dp * 2.f, menu_selector(GafFeatures::gray), s, BtnEmpty).first);
    //items->addObject(addBtn("Remove"        , 0.95f, 0.95f - dp * 3.5f, menu_selector(GafFeatures::removeOne), s, BtnEmpty).first);
    //items->addObject(addBtn("Add"           , 0.95f, 0.95f - dp * 3.f, menu_selector(GafFeatures::addOne), s, BtnEmpty).first);
    //items->addObject(addBtn("1"             , 0.75f, 0.95f - dp * 4.f, menu_selector(GafFeatures::set1), s, BtnEmpty).first);
    //items->addObject(addBtn("5"             , 0.85f, 0.95f - dp * 4.f, menu_selector(GafFeatures::set5), s, BtnEmpty).first);
    //items->addObject(addBtn("10"            , 0.95f, 0.95f - dp * 4.f, menu_selector(GafFeatures::set10), s, BtnEmpty).first);
    items->addObject(addBtn("Frame"        , 0.95f, 0.95f - dp * 4.0f, menu_selector(GafFeatures::prevFrame), s, BtnMinus).first);
    items->addObject(addBtn("Frame"        , 0.95f, 0.95f - dp * 4.5f, menu_selector(GafFeatures::nextFrame), s, BtnPlus).first);
    //items->addObject(addBtn("Full cleanup"  , 0.95f, 0.95f - dp * 5.0f, menu_selector(GafFeatures::cleanup), s, BtnEmpty).first);
    items->addObject(addBtn("Anim"         , 0.95f, 0.95f - dp * 5.5f, menu_selector(GafFeatures::prev_anim), s, BtnMinus).first);
    items->addObject(addBtn("Anim"         , 0.95f, 0.95f - dp * 6.0f, menu_selector(GafFeatures::next_anim), s, BtnPlus).first);

    m_nextSeq = addBtn("Next sequence"      , 0.95f, 0.95f - dp * 2.5f, menu_selector(GafFeatures::nextSequence), s, BtnPlus);
    m_prevSeq = addBtn("Prev sequence"      , 0.95f, 0.95f - dp * 3.f, menu_selector(GafFeatures::prevSequence), s, BtnMinus);

    items->addObject(m_nextSeq.first);
    items->addObject(m_prevSeq.first);

    m_nextSeq.second->setColor(ccc3(255, 0, 0));
    m_prevSeq.second->setColor(ccc3(255, 0, 0));
    
    CCMenu* pMenu = CCMenu::createWithArray(items);
    pMenu->setPosition(CCPointZero);
    pMenu->setTouchEnabled(true);
    addChild(pMenu, 10000);
    m_anim_index = 0;

    m_jsons.push_back("SampleAnimations/2/2.json");
    m_jsons.push_back("SampleAnimations/1/1.json");
    m_jsons.push_back("SampleAnimations/3/3.json");
    m_jsons.push_back("SampleAnimations/4/4.json");

    addObjectsToScene(1);
    black(NULL);

    setTouchEnabled(true);
    return true;
}

void GafFeatures::enableSequenceControllers( bool value )
{
    m_nextSeq.first->setVisible(value);
    m_prevSeq.first->setVisible(value);

    m_nextSeq.second->setVisible(value);
    m_prevSeq.second->setVisible(value);
}

void GafFeatures::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFAnimatedObject * node = (GAFAnimatedObject*)m_objects->objectAtIndex(0);
    CCTouch * pTouch = (CCTouch*) pTouches->anyObject();
    CCPoint pt = pTouch->getLocation();
    node->setPosition(pt.x, pt.y);
}

void GafFeatures::nextSequence( CCObject* )
{
    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->objectAtIndex(0);

    m_currentSequence++;

    if (m_currentSequence >= m_objectSequencesNames.size())
    {
        m_currentSequence = 0;
    }

    const char* secName = m_objectSequencesNames[m_currentSequence].c_str();
    object->playSequence(secName, true);
}

void GafFeatures::prevSequence( CCObject* )
{
    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->objectAtIndex(0);

    m_currentSequence--;

    if (m_currentSequence < 0)
    {
        m_currentSequence = m_objectSequencesNames.size() - 1;
    }

    const char* secName = m_objectSequencesNames[m_currentSequence].c_str();
    object->playSequence(secName, true);
}

void GafFeatures::next_anim(CCObject*)
{
    if (!m_jsons.size())
    {
        return;
    }

    cleanup(NULL);
    ++m_anim_index;

    if (m_anim_index >= (int)m_jsons.size())
    {
        m_anim_index = 0;
    }
    addObjectsToScene(1);
}

void GafFeatures::prev_anim(CCObject*)
{
    if (!m_jsons.size())
    {
        return;
    }

    cleanup(NULL);
    --m_anim_index;
    if (m_anim_index < 0)
    {
        m_anim_index = m_jsons.size() - 1;
    }

    addObjectsToScene(1);
}

void GafFeatures::restart(CCObject*)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->objectAtIndex(0);
    object->stop();
    object->start();
}

void GafFeatures::playpause(CCObject*)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->objectAtIndex(0);

    if (object->isAnimationRunning())
    {
        object->pause();
    }
    else
    {
        object->resume();
    }
}

int GafFeatures::maxFrameNumber()
{
    if (!m_objects || !m_objects->count())
    {
        return -1;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->objectAtIndex(0);
    return object->totalFrameCount();	
}

void GafFeatures::setFrameNumber(int aFrameNumber)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->objectAtIndex(0);
    object->setFrame(aFrameNumber);	
}

int GafFeatures::frameNumber()
{
    if (!m_objects || !m_objects->count())
    {
        return -1;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->objectAtIndex(0);
    return object->currentFrameIndex();
}

void GafFeatures::set1(CCObject*)
{
    set(1);
}

void GafFeatures::set5(CCObject*)
{
    set(5);
}

void GafFeatures::set10(CCObject*)
{
    set(10);
}

void GafFeatures::set(int n)
{
    if (!m_objects)
    {
        m_objects = CCArray::create();
        m_objects->retain();
    }

    int c = m_objects->count();

    if (c == n)
    {
        return;
    }

    if (n > c)
    {
        addObjectsToScene(n - c);
    }
    else
    {
        removeFromScene(c - n);
    }
}

void GafFeatures::addOne(CCObject*)
{
    addObjectsToScene(1);
}

void GafFeatures::removeOne(CCObject*)
{
    removeFromScene(1);
}

void GafFeatures::cleanup(CCObject*)
{
    CC_SAFE_RELEASE_NULL(m_asset);

    if (!m_objects)
    {
        return;
    }

    removeFromScene(m_objects->count());
    CC_SAFE_RELEASE_NULL(m_objects);
}

void GafFeatures::removeFromScene(int aCount)
{
    if (!m_objects || !aCount)
    {
        return;
    }

    if (aCount > int(m_objects->count()))
    {
        aCount = m_objects->count();
    }

    for (int i = 0; i < aCount; ++i)
    {
        GAFAnimatedObject *obj = (GAFAnimatedObject *) m_objects->lastObject();
        removeChild(obj);
        m_objects->removeLastObject();
    }
}

void GafFeatures::addObjectsToScene(int aCount)
{
    if (!aCount)
    {
        return;
    }

    if (!m_asset)
    {
        m_asset =  GAFAsset::create(m_jsons[m_anim_index]);
        CC_SAFE_RETAIN(m_asset);
    }

    if (!m_objects)
    {
        m_objects = CCArray::create();
        m_objects->retain();
    }

    const CCSize size = CCDirector::sharedDirector()->getWinSizeInPixels();

    if (m_asset)
    {
        int initialCount = m_objects->count();

        for (int i = initialCount; i < initialCount + aCount; ++i)
        {
            GAFAnimatedObject *object = m_asset->createObjectAndRun(true);

            object->setZOrder(100 * i);
            addChild(object);

            object->setPosition(size.width * 0.5f, size.height * 0.5f);
            m_objects->addObject(object);

            enableSequenceControllers(object->hasSequences());

            m_objectSequencesNames.clear();

            CCDictionary* secDictionary = m_asset->getSequences();
            if (secDictionary)
            {
                CCDictElement* pElement = 0;
                CCDICT_FOREACH(secDictionary, pElement)
                {
                    m_objectSequencesNames.push_back(pElement->getStrKey());
                }
            }

            // will work only if animation has a sequence
            object->playSequence("walk", true);

            object->setSequenceDelegate(this);
        }
    }
}

void GafFeatures::onFinishSequence( GAFAnimatedObject * object, const std::string& sequenceName )
{
    //! This function will be triggered once a sequence completed
}
