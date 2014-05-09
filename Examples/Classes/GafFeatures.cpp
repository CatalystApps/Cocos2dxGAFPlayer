#include "GafFeatures.h"

#include "GAFAsset.h"
#include "GAFAnimatedObject.h"

#ifdef WIN32
#include <windows.h>

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}
#else
static double startCounter = 0.0;
void StartCounter()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    startCounter = double(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

double GetCounter()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return double(tv.tv_sec * 1000 + tv.tv_usec / 1000) - startCounter;
}
#endif

CCPoint centerScreenPosition(GAFAsset* ast, const CCSize& screenSize)
{
    const GAFHeader& headInfo = ast->getHeader();

    return CCPoint(-headInfo.frameSize.getMinX() + (screenSize.width - headInfo.frameSize.size.width) / 2,
        headInfo.frameSize.getMinY() + (screenSize.height + headInfo.frameSize.size.height) / 2);
}

GafFeatures::GafFeatures()
    :
    m_asset(NULL),
    m_objects(NULL),
    m_currentSequence(0)
    , m_loadingTimeLabel(NULL)
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
    items->addObject(addBtn("Reverse"      , 0.95f, 0.95f - dp * 3.5f, menu_selector(GafFeatures::toggleReverse), s, BtnEmpty).first);
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

    m_files.push_back("biggreen/biggreen.gaf");
    m_files.push_back("bird_bezneba/bird_bezneba.gaf");
    m_files.push_back("christmas2013_julia2/christmas2013_julia2.gaf");
    m_files.push_back("cut_the_hope/cut_the_hope.gaf");	
    m_files.push_back("fairy2/fairy2.gaf");
    m_files.push_back("firemen/firemen.gaf");
    m_files.push_back("impiretank_05_oneplace/impiretank_05_oneplace.gaf");
    m_files.push_back("myshopsgame4/myshopsgame4.gaf");
    m_files.push_back("peacock_feb3_natasha/peacock_feb3_natasha.gaf");
    m_files.push_back("tiger/tiger.gaf");

    m_loadingTimeLabel = CCLabelTTF::create("", "Marker Felt", 24);
    m_loadingTimeLabel->setColor(ccc3(0, 0, 255));
    m_loadingTimeLabel->setAnchorPoint(CCPoint(1, 0.5));
    m_loadingTimeLabel->setPosition(ccp(300, 300));

    //addChild(m_loadingTimeLabel);

    addObjectsToScene(1);
    gray(NULL);

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


    if (m_currentSequence == 0)
    {
        // Switch to last sequence if current is first
        m_currentSequence = m_objectSequencesNames.size() - 1;
    }
    else
    {
        // Switch to prev sequence
        m_currentSequence--;
    }

    const char* secName = m_objectSequencesNames[m_currentSequence].c_str();
    object->playSequence(secName, true);
}

void GafFeatures::next_anim(CCObject*)
{
    if (!m_files.size())
    {
        return;
    }

    cleanup(NULL);
    ++m_anim_index;

    if (m_anim_index >= (int)m_files.size())
    {
        m_anim_index = 0;
    }
    addObjectsToScene(1);
}

void GafFeatures::prev_anim(CCObject*)
{
    if (!m_files.size())
    {
        return;
    }

    cleanup(NULL);
    --m_anim_index;
    if (m_anim_index < 0)
    {
        m_anim_index = m_files.size() - 1;
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

void GafFeatures::toggleReverse(CCObject*)
{
    if (m_objects->count() == 0)
    {
        return;
    }
    GAFAnimatedObject *obj = (GAFAnimatedObject *)m_objects->lastObject();
    obj->setReversed(!obj->isReversed());
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
        StartCounter();
        m_asset = GAFAsset::create(m_files[m_anim_index], this);
        double loadingTime = GetCounter();

        cocos2d::CCLog("Loading time [%f]\n", loadingTime);

        std::ostringstream ss;
        ss << m_files[m_anim_index] << " ";
        ss << loadingTime;

        //m_loadingTimeLabel->setString(ss.str().c_str());

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
            GAFAnimatedObject *object = m_asset->createObject();
 
            object->setZOrder(100 * i);

            addChild(object);

            float scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
            object->setPosition(centerScreenPosition(m_asset, size / scaleFactor));

            m_objects->addObject(object);

            enableSequenceControllers(object->hasSequences());

            m_objectSequencesNames.clear();

            const AnimationSequences_t& secDictionary = m_asset->getAnimationSequences();
            if (!secDictionary.empty())
            {
                for (AnimationSequences_t::const_iterator i = secDictionary.begin(), e = secDictionary.end(); i != e; ++i)
                {
                    m_objectSequencesNames.push_back(i->first);
                }
            }

            // will work only if animation has a sequence
            object->playSequence("walk", true);

            object->start();
            object->setLooped(true);

            object->setSequenceDelegate(this);
            object->setAnimationPlaybackDelegate(this);
        }
    }
}

void GafFeatures::onFinishSequence( GAFAnimatedObject * object, const std::string& sequenceName )
{
    //! This function will be triggered once a sequence completed
}

//! path parameter could be changed
void GafFeatures::onTexturePreLoad(std::string& path)
{
    CCLOG("Loading texture %s", path.c_str());
}

void GafFeatures::onAnimationFinishedPlayDelegate(GAFAnimation* animation)
{

}
