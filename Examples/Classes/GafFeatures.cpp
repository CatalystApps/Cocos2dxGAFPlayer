#include "GafFeatures.h"

#include <audio/include/SimpleAudioEngine.h>
#include "audio/include/AudioEngine.h"

#include <iostream>

#if defined(__APPLE__) && defined(SEARCH_ALL_GAF_FILES)
#include <dirent.h>
#include <unistd.h>
#endif // __APPLE__ && SEARCH_ALL_GAF_FILES

#ifdef WIN32
#include <windows.h>
#ifdef SEARCH_ALL_GAF_FILES
#include <codecvt>
#endif // SEARCH_ALL_GAF_FILES

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


cocos2d::Point centerScreenPosition(const cocos2d::Size& screenSize)
{
    return cocos2d::Point(screenSize.width / 2, screenSize.height / 2);
}

GafFeatures::GafFeatures()
    :
    m_asset(nullptr),
    m_objects(nullptr),
    m_currentSequence(0),
	m_loadingTimeLabel(nullptr)
{
    m_touchlistener = cocos2d::EventListenerTouchAllAtOnce::create();

    m_touchlistener->onTouchesBegan = CC_CALLBACK_2(cocos2d::Layer::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchlistener, this);
}
GafFeatures::~GafFeatures()
{
    CC_SAFE_RELEASE(m_asset);
    CC_SAFE_RELEASE(m_objects);
    CC_SAFE_RELEASE(m_playButton);
    CC_SAFE_RELEASE(m_pauseButton);
    CC_SAFE_RELEASE(m_nextSequence);
    CC_SAFE_RELEASE(m_prevSequence);
    CC_SAFE_RELEASE(m_sequenceName);
    CC_SAFE_RELEASE(m_vramStat);

    _eventDispatcher->removeEventListener(m_touchlistener);
}

cocos2d::Scene* GafFeatures::scene()
{
    cocos2d::Scene *scene = cocos2d::Scene::create();
    GafFeatures *layer = GafFeatures::create();
    scene->addChild(layer);
    return scene;
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

void GafFeatures::setupMenuItems()
{
    cocos2d::Size wsize = cocos2d::Director::getInstance()->getWinSize();
    cocos2d::Vector<cocos2d::MenuItem*> items;
    
    using namespace std::placeholders;
    
    const float Y = 0.1f;
    
    items.pushBack(addButton("buttons/previous_animation_button@2x.png", "buttons/previous_animation_button_pressed@2x.png", cocos2d::Point(0.0f, Y),
                             [this](cocos2d::Ref* obj)
                             {
                                 GafFeatures::prev_anim(obj);
                                 m_playButton->setVisible(false);
                                 m_pauseButton->setVisible(true);
                             }));
    
                   
    items.pushBack(addButton("buttons/previous_frame_button@2x.png", "buttons/previous_frame_button_pressed@2x.png", cocos2d::Point(0.1f, Y), std::bind(&GafFeatures::prevFrame, this, _1)));
    
    m_playButton = addButton("buttons/play_button@2x.png", "buttons/play_button_pressed@2x.png", cocos2d::Point(0.22f, Y),
                             [this](cocos2d::Ref* obj)
                             {
                                 m_playButton->setVisible(true);
                                 m_pauseButton->setVisible(false);
                                 this->playpause(obj);
                             });
    
    m_pauseButton = addButton("buttons/pause_button@2x.png", "buttons/pause_button_pressed@2x.png", cocos2d::Point(0.22f, Y),
                              [this](cocos2d::Ref* obj)
                              {
                                  m_pauseButton->setVisible(false);
                                  m_playButton->setVisible(true);
                                  this->playpause(obj);
                              });
    
    items.pushBack(addButton("buttons/next_frame_button@2x.png", "buttons/next_frame_button_pressed@2x.png", cocos2d::Point(0.34f, Y), std::bind(&GafFeatures::nextFrame, this, _1)));
    
    items.pushBack(addButton("buttons/next_animation_button@2x.png", "buttons/next_animation_button_pressed@2x.png", cocos2d::Point(0.44f, Y),
                             [this](cocos2d::Ref* obj)
                             {
                                 m_pauseButton->setVisible(true);
                                 m_playButton->setVisible(false);

                                 GafFeatures::next_anim(obj);
                             }));
    
    items.pushBack(addButton("buttons/restart_button@2x.png", "buttons/restart_button_pressed@2x.png", cocos2d::Point(0.57f, Y), std::bind(&GafFeatures::restart, this, _1)));
    
    items.pushBack(addButton("buttons/white_bg_button@2x.png", "buttons/white_bg_button_pressed@2x.png", cocos2d::Point(0.67f, Y),
                             [this](cocos2d::Ref* ob)
                             {
                                 static int mode = 0;
                                 mode ++;
                                 
                                 if (mode == 3) mode = 0;
                                 switch (mode)
                                 {
                                     case 0:
                                         this->gray(ob);
                                         break;
                                     case 1:
                                         this->white(ob);
                                         break;
                                     case 2:
                                         this->black(ob);
                                         break;
                                     default:
                                         break;
                                 }
                             }
                             ));
    
    m_nextSequence = addButton("buttons/right_arrow@2x.png", "buttons/right_arrow_pressed@2x.png", cocos2d::Point(0.40f, 0.93f), std::bind(&GafFeatures::nextSequence, this, _1));
    
    m_prevSequence = addButton("buttons/left_arrow@2x.png", "buttons/left_arrow_pressed@2x.png", cocos2d::Point(0.05f, 0.93f), std::bind(&GafFeatures::prevSequence, this, _1));
    
    
    
    m_playButton->retain();
    m_pauseButton->retain();
    m_nextSequence->retain();
    m_prevSequence->retain();
    
    m_playButton->setVisible(false);
    items.pushBack(m_pauseButton);
    items.pushBack(m_playButton);
    items.pushBack(m_nextSequence);
    items.pushBack(m_prevSequence);
    
    m_sequenceName = cocos2d::Label::createWithSystemFont("---", "System", 24);
    m_sequenceName->retain();
    
    cocos2d::Menu* pMenu = cocos2d::Menu::createWithArray(items);

    pMenu->setPosition(cocos2d::Point(wsize.width / 2.f - m_pauseButton->getPositionX(), 0));

    cocos2d::MenuItemLabel* labelMenuItem = cocos2d::MenuItemLabel::create(m_sequenceName);
    labelMenuItem->setAnchorPoint(cocos2d::Vec2(0.f, 0.5f));
    cocos2d::Point labelPos = cocos2d::Point( m_prevSequence->getPositionX() + m_prevSequence->getContentSize().width / 2.f, m_prevSequence->getPositionY());
    labelMenuItem->setPosition(labelPos);
    
    pMenu->addChild(labelMenuItem);
    
    addChild(pMenu, 10000);
    
    cocos2d::Sprite* gafLogo = cocos2d::Sprite::create("buttons/gaf_logo@2x.png");
    gafLogo->setAnchorPoint(cocos2d::Point(1.f, 1.f));
    gafLogo->setScale(0.5f * cocos2d::Director::getInstance()->getContentScaleFactor());
    gafLogo->setPosition(cocos2d::Vec2(wsize.width * 0.93f, wsize.height * 0.93f));
    
    addChild(gafLogo, 10000);
    
    m_vramStat = cocos2d::Label::createWithSystemFont("VRAM: XXX", "System", 14);
    m_vramStat->setAnchorPoint(cocos2d::Point(0.f, 0.f));
    m_vramStat->setPosition(cocos2d::Vec2(10, wsize.height * 0.1f));
    m_vramStat->retain();
    
    addChild(m_vramStat);
}

cocos2d::MenuItemImage* GafFeatures::addButton(const std::string &buttonName, const std::string &buttonPressedName, const cocos2d::Point &pos, const cocos2d::ccMenuCallback& clb)
{
    cocos2d::Size size = cocos2d::Director::getInstance()->getWinSize();
    
    cocos2d::MenuItemImage* item = cocos2d::MenuItemImage::create(buttonName, buttonPressedName, "", clb);
    
    item->setPosition(cocos2d::Point(size.width * pos.x, size.height * pos.y));
    item->setScale(cocos2d::Director::getInstance()->getContentScaleFactor());
    
    return item;
}

bool GafFeatures::init()
{
    setupMenuItems();
    gray(nullptr);
    
    generateGafFilesList();
    
    m_anim_index = 0;
    
    addObjectsToScene();
    
    return true;
}

void GafFeatures::generateGafFilesList()
{
#ifdef SEARCH_ALL_GAF_FILES
#if defined(WIN32)
    std::wstring start_path = L"";
#elif defined(__APPLE__)
    char *dir = getcwd(NULL, 0);
    std::string start_path = dir;
    start_path.append("/../../.."); // Supress Appname.app/Content/Resources which is default directory
    chdir(start_path.c_str()); // Mandatory action
#endif // Platform
    searchGafFilesInDirectory(start_path);
#else // SEARCH_ALL_GAF_FILES
    m_files.push_back("SoundsExample_Tank/SoundsExample_Tank.gaf");
    m_files.push_back("cut_the_hope/cut_the_hope.gaf");
    m_files.push_back("biggreen/biggreen.gaf");
    m_files.push_back("bird_bezneba/bird_bezneba.gaf");
    m_files.push_back("christmas2013_julia2/christmas2013_julia2.gaf");
    m_files.push_back("fireman_2014_sound/fireman_2014_sound.gaf");
    m_files.push_back("fairy2/fairy2.gaf");
    m_files.push_back("firemen/firemen.gaf");
    m_files.push_back("impiretank_05_oneplace/impiretank_05_oneplace.gaf");
    m_files.push_back("myshopsgame4/myshopsgame4.gaf");
    m_files.push_back("peacock_feb3_natasha/peacock_feb3_natasha.gaf");
    m_files.push_back("tiger/tiger.gaf");
#endif // SEARCH_ALL_GAF_FILES
}

#ifdef SEARCH_ALL_GAF_FILES
#ifdef WIN32
void GafFeatures::searchGafFilesInDirectory(std::string& path)
{
    std::wstring wpath = path;
    std::wstring files = wpath + L"*.*";
    const std::wstring gaf_extension = L".gaf";

    WIN32_FIND_DATA wfd;
    HANDLE search = FindFirstFileEx(files.c_str(), FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
    bool ret = true;
    if (search != INVALID_HANDLE_VALUE)
    {
        BOOL find = true;
        while (find)
        {
            //. ..
            if (wfd.cFileName[0] != '.')
            {
                std::wstring temp = wpath + wfd.cFileName;
                
                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    temp += '/';
                    searchGafFilesInDirectory(temp);
                }
                else if (temp.rfind(gaf_extension) == temp.length() - gaf_extension.length())
                {
                    const std::wstring resources_dir = L"Resources/";
                    int pos = temp.rfind(resources_dir);
                    std::wstring path_to_append = temp.substr(pos + resources_dir.length());

                    typedef std::codecvt_utf8<wchar_t> convert_type;
                    std::wstring_convert<convert_type, wchar_t> converter;

                    std::string converted_path = converter.to_bytes(path_to_append);
                    m_files.push_back(converted_path);
                }
            }
            find = FindNextFile(search, &wfd);
        }
        if (path.empty())
        {
            FindClose(search);
        }
    }
}
#endif // WIN32

#ifdef __APPLE__
void GafFeatures::searchGafFilesInDirectory(std::string& path)
{
    const char* gaf_extension = ".gaf";

    DIR* directory = opendir(path.c_str());
    
    if (directory)
    {
        struct dirent* hFile;
        while ((hFile = readdir(directory)) != NULL)
        {
            if (hFile->d_name[0] == '.')
            {
                continue;
            }
            
            std::string child_entry = path + "/";
            child_entry.append(hFile->d_name);
            
            if (hFile->d_type == DT_DIR)
            {
                searchGafFilesInDirectory(child_entry);
            }
            else if(strstr(hFile->d_name, gaf_extension))
            {
                m_files.push_back(child_entry);
            }
        }
        closedir(directory);
    }
    
}
#endif // __APPLE__
#endif // SEARCH_ALL_GAF_FILES

void GafFeatures::enableSequenceControllers( bool value )
{
    m_prevSequence->setVisible(value);
    m_nextSequence->setVisible(value);
    m_sequenceName->setVisible(value);
    
    if (value)
    {
        m_sequenceName->setString(m_objectSequencesNames[m_currentSequence]);
    }
}

void GafFeatures::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFObject * node = (GAFObject*)m_objects->getObjectAtIndex(0);
    cocos2d::Touch * pTouch = touches[0];
    cocos2d::Point pt = pTouch->getLocation();
    node->setPosition(pt.x, pt.y);
}

void GafFeatures::nextSequence( cocos2d::Ref* )
{
    GAFObject *object = (GAFObject *)m_objects->getObjectAtIndex(0);

    m_currentSequence++;

    if (m_currentSequence >= m_objectSequencesNames.size())
    {
        m_currentSequence = 0;
    }

    const char* secName = m_objectSequencesNames[m_currentSequence].c_str();
    object->playSequence(secName, true);
    
    m_sequenceName->setString(secName);
}

void GafFeatures::prevSequence( cocos2d::Ref* )
{
    GAFObject *object = (GAFObject *)m_objects->getObjectAtIndex(0);

    m_currentSequence--;

    if (m_currentSequence < 0)
    {
        m_currentSequence = m_objectSequencesNames.size() - 1;
    }

    const char* secName = m_objectSequencesNames[m_currentSequence].c_str();
    object->playSequence(secName, true);
    
    m_sequenceName->setString(secName);
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
    addObjectsToScene();
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

    addObjectsToScene();
}

void GafFeatures::restart(cocos2d::Ref*)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFObject *object = (GAFObject *)m_objects->getObjectAtIndex(0);
    //object->stop(true);
    //object->start(true);
}

void GafFeatures::playpause(cocos2d::Ref*)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFObject *object = (GAFObject *)m_objects->getObjectAtIndex(0);

    if (object->getIsAnimationRunning())
    {
        object->setAnimationRunning(false, true);
    }
    else
    {
        object->setAnimationRunning(true, true);
    }
}

int GafFeatures::maxFrameNumber()
{
    if (!m_objects || !m_objects->count())
    {
        return -1;
    }

    GAFObject *object = (GAFObject *)m_objects->getObjectAtIndex(0);
    return object->getTotalFrameCount();
}

void GafFeatures::setFrameNumber(int aFrameNumber)
{
    if (!m_objects || !m_objects->count())
    {
        return;
    }

    GAFObject *object = (GAFObject *)m_objects->getObjectAtIndex(0);
    object->setFrame(aFrameNumber);	
}

int GafFeatures::frameNumber()
{
    if (!m_objects || !m_objects->count())
    {
        return -1;
    }

    GAFObject *object = (GAFObject *)m_objects->getObjectAtIndex(0);
    return object->getCurrentFrameIndex();
}

void GafFeatures::toggleReverse(cocos2d::Ref*)
{
    if (m_objects->count() == 0)
    {
        return;
    }
    GAFObject *obj = (GAFObject *)m_objects->getLastObject();
    obj->setReversed(!obj->isReversed());
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
        GAFObject *obj = (GAFObject *) m_objects->getLastObject();
        removeChild(obj);
        m_objects->removeLastObject();
    }
}

void GafFeatures::addObjectsToScene()
{
    using namespace std::placeholders;
    
    if (!m_asset)
    {
        StartCounter();
        m_asset = GAFAsset::create(m_files[m_anim_index], nullptr);
        double loadingTime = GetCounter();

        cocos2d::log("Loading time [%f]\n", loadingTime);

        std::ostringstream ss;
        ss << m_files[m_anim_index] << " ";
        ss << loadingTime;

        //m_loadingTimeLabel->setString(ss.str().c_str());
        
        ss.str("");
        
		ss << "VRAM: ";
        ss << m_asset->getTextureManager()->getMemoryConsumptionStat();
        ss << " bytes";
        
        m_vramStat->setString(ss.str());
        
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
        m_asset->setSoundDelegate(std::bind(&GafFeatures::onSoundEvent, this, _1, _2, _3));
        GAFObject *object = m_asset->createObject();
        
        object->setLocalZOrder(0);
        addChild(object);
        
        float scaleFactor = cocos2d::Director::getInstance()->getContentScaleFactor();
        object->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        object->setPosition(centerScreenPosition(size / scaleFactor));
        object->setLocator(true);
        
        m_objects->addObject(object);
        
        m_objectSequencesNames.clear();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
        
        for(auto& effect : m_musicEffects)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(effect.second.c_str());
        }
#endif
        m_musicEffects.clear();
        
        const AnimationSequences_t& secDictionary = m_asset->getRootTimeline()->getAnimationSequences(); // TODO: only root timeline (temporary workaround)
        if (!secDictionary.empty())
        {
            for (AnimationSequences_t::const_iterator i = secDictionary.begin(), e = secDictionary.end(); i != e; ++i)
            {
                const std::string& seqName = i->first;
                std::string::size_type pos = seqName.find("__audio:");
                if (pos != std::string::npos)
                {
                    std::string effectName = seqName.substr(pos + 8) + ".wav";
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(effectName.c_str());
#endif
                    m_musicEffects[i->second.startFrameNo] = std::move(effectName);
                }
                else
                {
                    m_objectSequencesNames.push_back(i->first);
                }
            }
        }
        
        enableSequenceControllers(!m_objectSequencesNames.empty());
        
        // will work only if animation has a sequence
        object->playSequence("walk", true);
        object->setLooped(true, true);
        object->start();
        
        object->setSequenceDelegate(std::bind(&GafFeatures::onFinishSequence, this, _1, _2));
        object->setFramePlayedDelegate(std::bind(&GafFeatures::onFramePlayed, this, _1, _2));
        
    }
}

void GafFeatures::onFinishSequence( GAFObject * object, const std::string& sequenceName )
{
    //! This function will be triggered once a sequence completed
}

void GafFeatures::onFramePlayed(GAFObject *object, uint32_t frame)
{
    MusicEffects_t::const_iterator it = m_musicEffects.find(frame);
    
    if (it != m_musicEffects.end())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(it->second.c_str());
#endif
    }
}

//! path parameter could be changed
void GafFeatures::onTexturePreLoad(std::string* path)
{
    CCLOG("Loading texture %s", path->c_str());
}

void GafFeatures::onSoundEvent(GAFSoundInfo* sound, int32_t repeat, GAFSoundInfo::SyncEvent syncEvent)
{
    std::string path = m_asset->getGAFFileName();
    int slashPos = path.find_last_of("/");
    path = path.substr(0, slashPos + 1);
    path.append(sound->source);

    cocos2d::experimental::AudioEngine::play2d(path, repeat == -1);
}