#include "GafFeatures.h"

#include "GAFAsset.h"
#include "GAFAnimatedObject.h"

#include <iostream>

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


cocos2d::Point centerScreenPosition(GAFAsset* ast, const cocos2d::Size& screenSize)
{
    const GAFHeader& headInfo = ast->getHeader();
    
    return cocos2d::Point(-headInfo.frameSize.getMinX() + (screenSize.width - headInfo.frameSize.size.width) / 2,
                           headInfo.frameSize.getMinY() + (screenSize.height + headInfo.frameSize.size.height) / 2);
}

GafFeatures::GafFeatures()
    :
    m_asset(NULL),
    m_objects(NULL),
    m_currentSequence(0)
    , m_loadingTimeLabel(NULL)
{
    m_touchlistener = cocos2d::EventListenerTouchAllAtOnce::create();

    m_touchlistener->onTouchesBegan = CC_CALLBACK_2(cocos2d::Layer::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchlistener, this);
}
GafFeatures::~GafFeatures()
{
    CC_SAFE_RELEASE(m_asset);
    CC_SAFE_RELEASE(m_objects);

    _eventDispatcher->removeEventListener(m_touchlistener);
}

cocos2d::Scene* GafFeatures::scene()
{
    cocos2d::Scene *scene = cocos2d::Scene::create();
    GafFeatures *layer = GafFeatures::create();
    scene->addChild(layer);
    return scene;
}

GafFeatures::MenuItemPair_t GafFeatures::addBtn(const char * text, float px, float py, const cocos2d::ccMenuCallback& clb, float k, BtnType btnType)
{
    cocos2d::Size size = cocos2d::Director::getInstance()->getWinSize();

    cocos2d::MenuItemImage *res = NULL;

    if (btnType == BtnEmpty)
    {
        res = cocos2d::MenuItemImage::create(
            "button_on.png",
            "button_off.png",
            "",
            clb);
    }
    else if (btnType == BtnPlus)
    {
        res = cocos2d::MenuItemImage::create(
            "button_on_plus.png",
            "button_off_plus.png",
            "",
            clb);
    }
    else if (btnType == BtnMinus)
    {
        res = cocos2d::MenuItemImage::create(
            "button_on_minus.png",
            "button_off_minus.png",
            "",
            clb);
    }

    res->setPosition( cocos2d::Point(size.width * px, size.height * py) );
    res->setScale(k);

    cocos2d::Label* pLabel = cocos2d::Label::create(text, "Marker Felt", 24);
    pLabel->setColor(cocos2d::Color3B(0, 0, 255));

    pLabel->setAnchorPoint(cocos2d::Point(1, 0.5));
    //pLabel->setScale(k);

    cocos2d::Point labelPos = cocos2d::Point(res->getPositionX() - res->getContentSize().width * k * 0.5f, res->getPositionY());

    pLabel->setPosition( cocos2d::Point(res->getPositionX() - res->getContentSize().width * k * 0.5f, res->getPositionY()) );
    addChild(pLabel, 100000);

    MenuItemPair_t p = std::make_pair(res, pLabel);

    return p;
}

void GafFeatures::black(cocos2d::Ref*)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GafFeatures::white(cocos2d::Ref*)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GafFeatures::gray(cocos2d::Ref*)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void GafFeatures::prevFrame(cocos2d::Ref*)
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

void GafFeatures::nextFrame(cocos2d::Ref*)
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
{    float s = cocos2d::Director::getInstance()->getContentScaleFactor();

    float dp = 0.13f;

    using namespace std::placeholders;

    cocos2d::Vector<cocos2d::MenuItem*> items;
    items.pushBack(addBtn("Play/Pause"    , 0.95f, 0.95f,            std::bind(&GafFeatures::playpause, this, _1), s, BtnEmpty).first);
    items.pushBack(addBtn("Restart"       , 0.95f, 0.95f - dp,       std::bind(&GafFeatures::restart, this, _1), s, BtnEmpty).first);
    items.pushBack(addBtn("B"             , 0.75f, 0.95f - dp * 2.f, std::bind(&GafFeatures::black, this, _1), s, BtnEmpty).first);
    items.pushBack(addBtn("W"             , 0.85f, 0.95f - dp * 2.f, std::bind(&GafFeatures::white, this, _1), s, BtnEmpty).first);
    items.pushBack(addBtn("G"             , 0.95f, 0.95f - dp * 2.f, std::bind(&GafFeatures::gray, this, _1), s, BtnEmpty).first);
    //items.pushBack(addBtn("Remove"        , 0.95f, 0.95f - dp * 3.5f, std::bind(&GafFeatures::removeOne, this, _1), s, BtnEmpty).first);
    //items.pushBack(addBtn("Add"           , 0.95f, 0.95f - dp * 3.f, std::bind(&GafFeatures::addOne, this, _1), s, BtnEmpty).first);
    items.pushBack(addBtn("Reverse"       , 0.95f, 0.95f - dp * 3.5f, std::bind(&GafFeatures::toggleReverse, this, _1), s, BtnEmpty).first);
    //items.pushBack(addBtn("1"             , 0.75f, 0.95f - dp * 4.f, std::bind(&GafFeatures::set1, this, _1), s, BtnEmpty).first);
    //items.pushBack(addBtn("5"             , 0.85f, 0.95f - dp * 4.f, std::bind(&GafFeatures::set5, this, _1), s, BtnEmpty).first);
    //items.pushBack(addBtn("10"            , 0.95f, 0.95f - dp * 4.f, std::bind(&GafFeatures::set10, this, _1), s, BtnEmpty).first);
    items.pushBack(addBtn("Frame"        , 0.95f, 0.95f - dp * 4.0f, std::bind(&GafFeatures::prevFrame, this, _1), s, BtnMinus).first);
    items.pushBack(addBtn("Frame"        , 0.95f, 0.95f - dp * 4.5f, std::bind(&GafFeatures::nextFrame, this, _1), s, BtnPlus).first);
    //items.pushBack(addBtn("Full cleanup"  , 0.95f, 0.95f - dp * 5.0f, std::bind(&GafFeatures::cleanup, this, _1), s, BtnEmpty).first);
    items.pushBack(addBtn("Anim"         , 0.95f, 0.95f - dp * 5.5f, std::bind(&GafFeatures::prev_anim, this, _1), s, BtnMinus).first);
    items.pushBack(addBtn("Anim"         , 0.95f, 0.95f - dp * 6.0f, std::bind(&GafFeatures::next_anim, this, _1), s, BtnPlus).first);

    m_nextSeq = addBtn("Next sequence", 0.95f, 0.95f - dp * 2.5f, std::bind(&GafFeatures::nextSequence, this, _1), s, BtnPlus);
    m_prevSeq = addBtn("Prev sequence", 0.95f, 0.95f - dp * 3.f, std::bind(&GafFeatures::prevSequence, this, _1), s, BtnMinus);

    items.pushBack(m_nextSeq.first);
    items.pushBack(m_prevSeq.first);

    m_nextSeq.second->setColor(cocos2d::Color3B(255, 0, 0));
    m_prevSeq.second->setColor(cocos2d::Color3B(255, 0, 0));
    
    cocos2d::Menu* pMenu = cocos2d::Menu::createWithArray(items);
    pMenu->setPosition(cocos2d::Point::ZERO);

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
    

    addObjectsToScene(1);
    gray(NULL);

    return true;
}

void GafFeatures::enableSequenceControllers( bool value )
{
    m_nextSeq.first->setVisible(value);
    m_prevSeq.first->setVisible(value);

    m_nextSeq.second->setVisible(value);
    m_prevSeq.second->setVisible(value);
}

void GafFeatures::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFAnimatedObject * node = (GAFAnimatedObject*)m_objects->getObjectAtIndex(0);
    cocos2d::Touch * pTouch = touches[0];
    cocos2d::Point pt = pTouch->getLocation();
    node->setPosition(pt.x, pt.y);
}

void GafFeatures::nextSequence( cocos2d::Ref* )
{
    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->getObjectAtIndex(0);

    m_currentSequence++;

    if (m_currentSequence >= m_objectSequencesNames.size())
    {
        m_currentSequence = 0;
    }

    const char* secName = m_objectSequencesNames[m_currentSequence].c_str();
    object->playSequence(secName, true);
}

void GafFeatures::prevSequence( cocos2d::Ref* )
{
    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->getObjectAtIndex(0);

    m_currentSequence--;

    if (m_currentSequence < 0)
    {
        m_currentSequence = m_objectSequencesNames.size() - 1;
    }

    const char* secName = m_objectSequencesNames[m_currentSequence].c_str();
    object->playSequence(secName, true);
}

void GafFeatures::next_anim(cocos2d::Ref*)
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

void GafFeatures::prev_anim(cocos2d::Ref*)
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

void GafFeatures::restart(cocos2d::Ref*)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->getObjectAtIndex(0);
    object->stop();
    object->start();
}

void GafFeatures::playpause(cocos2d::Ref*)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->getObjectAtIndex(0);

    if (object->isAnimationRunning())
    {
        object->pauseAnimation();
    }
    else
    {
        object->resumeAnimation();
    }
}

int GafFeatures::maxFrameNumber()
{
    if (!m_objects || !m_objects->count())
    {
        return -1;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->getObjectAtIndex(0);
    return object->totalFrameCount();	
}

void GafFeatures::setFrameNumber(int aFrameNumber)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->getObjectAtIndex(0);
    object->setFrame(aFrameNumber);	
}

int GafFeatures::frameNumber()
{
    if (!m_objects || !m_objects->count())
    {
        return -1;
    }

    GAFAnimatedObject *object = (GAFAnimatedObject *)m_objects->getObjectAtIndex(0);
    return object->currentFrameIndex();
}

void GafFeatures::set1(cocos2d::Ref*)
{
    set(1);
}

void GafFeatures::set5(cocos2d::Ref*)
{
    set(5);
}

void GafFeatures::set10(cocos2d::Ref*)
{
    set(10);
}

void GafFeatures::set(int n)
{
    if (!m_objects)
    {
        m_objects = cocos2d::__Array::create();
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

void GafFeatures::addOne(cocos2d::Ref*)
{
    addObjectsToScene(1);
}

void GafFeatures::toggleReverse(cocos2d::Ref*)
{
    if (m_objects->count() == 0)
    {
        return;
    }
    GAFAnimatedObject *obj = (GAFAnimatedObject *)m_objects->getLastObject();
    obj->setReversed(!obj->isReversed());
}

void GafFeatures::removeOne(cocos2d::Ref*)
{
    removeFromScene(1);
}

void GafFeatures::cleanup(cocos2d::Ref*)
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
        GAFAnimatedObject *obj = (GAFAnimatedObject *) m_objects->getLastObject();
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

        cocos2d::log("Loading time [%f]\n", loadingTime);

        std::ostringstream ss;
        ss << m_files[m_anim_index] << " ";
        ss << loadingTime;

        //m_loadingTimeLabel->setString(ss.str().c_str());

        CC_SAFE_RETAIN(m_asset);
    }

    if (!m_objects)
    {
        m_objects = cocos2d::__Array::create();
        m_objects->retain();
    }

    const cocos2d::Size size = cocos2d::Director::getInstance()->getWinSizeInPixels();

    if (m_asset)
    {
        int initialCount = m_objects->count();

        for (int i = initialCount; i < initialCount + aCount; ++i)
        {
            GAFAnimatedObject *object = m_asset->createObject();
 
            object->setLocalZOrder(100 * i);

            addChild(object);
            
            float scaleFactor = cocos2d::Director::getInstance()->getContentScaleFactor();
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
            object->setLooped(true);
            object->start();

            object->setSequenceDelegate(this);
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
