#include "GAFSubobjectState.h"
#include "GAFFilterData.h"
#include "GAFAsset.h"

#include "cocoa/CCDictionary.h"

#include <vector>
#include <algorithm>
#include <sstream>

static const char * kStateName = "st";
static const char * kColorName = "c";
static const char * kMaskName = "m";
static const char * kEffectsName = "e";
static const char * kEffectTypeName = "t";
static const char * kXName = "x";
static const char * kYName = "y";
static const char * kGAFBlurFilterName = "Fblur";

using namespace std;

#define USE_PLAIN_SPLIT 1

#if USE_PLAIN_SPLIT
static void split(const string& s, const string& delim, std::vector<string>& result)
{
    if (delim.empty())
    {
        result.push_back(s);
        return;
    }

    string::const_iterator substart = s.begin(), subend;
    while (true)
    {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        string temp(substart, subend);
        if (!temp.empty())
        {
            result.push_back(temp);
        }
        if (subend == s.end())
        {
            break;
        }
        substart = subend + delim.size();
    }
}

#else

struct tokens : std::ctype<char>
{
    tokens() : std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table()
    {
        typedef std::ctype<char> cctype;
        static const cctype::mask *const_rc = cctype::classic_table();

        static cctype::mask rc[cctype::table_size];
        std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

        rc[','] = std::ctype_base::space;
        return &rc[0];
    }
};

static void split(const string& s, const string& delim, std::vector<std::string>& out)
{
    std::stringstream ss(s);
    ss.imbue(std::locale(std::locale(), new tokens()));

    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;

    out.assign(begin, end);
}
#endif

GAFSubobjectState::GAFSubobjectState()
:
_filters(NULL)
{

}

GAFSubobjectState::~GAFSubobjectState()
{
    CC_SAFE_RELEASE(_filters);
}

GAFSubobjectState * GAFSubobjectState::createWithStateDictionary(CCDictionary * dict, const char * objectId)
{
    GAFSubobjectState * ret = new GAFSubobjectState();
    if (ret && ret->initWithStateDictionary(dict, objectId))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

GAFSubobjectState * GAFSubobjectState::createEmptyWithObjectId(const char * objectId)
{
    GAFSubobjectState * ret = new GAFSubobjectState();
    if (ret && ret->initEmptyWinthObjectId(objectId))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool GAFSubobjectState::initEmptyWinthObjectId(const char * objId)
{
    objectId = objId;
    zIndex = 0;
    affineTransform = CCAffineTransformMake(1, 0, 0, 1, 0, 0);
    _colorOffsets[0] = _colorOffsets[1] = _colorOffsets[2] = _colorOffsets[3] = _colorMults[GAFCTI_A] = 0;
    _colorMults[GAFCTI_R] = _colorMults[GAFCTI_G] = _colorMults[GAFCTI_B] = 1;
    return true;
}

bool GAFSubobjectState::initWithStateDictionary(CCDictionary * dict, const char * objId)
{
    if (!objId)
    {
        CCLOGERROR("Error while creating GAFSubobjectState. Invalid objectId");
        return false;
    }
    objectId = objId;
    CCString * state = (CCString*)dict->objectForKey(kStateName);
    if (!state)
    {
        CCLOGERROR("Error while creating GAFSubobjectState. State state is missing");
        return false;
    }
    std::string str(state->getCString());
    int s = str.find('{');
    int e = str.find('}');
    if (s == std::string::npos || e == std::string::npos || e < s)
    {
        CCLOGERROR("Error while creating GAFSubobjectState. Initial state data '%s'", str.c_str());
        return false;
    }
    std::string temp = str.substr(0, s - 1);
    zIndex = atoi(temp.c_str());
    temp = str.substr(e + 2, str.length() - e - 2);
    _colorMults[GAFCTI_A] = static_cast<float>(atof(temp.c_str()));
    temp = str.substr(s + 1, e - s - 1);
#if USE_PLAIN_SPLIT
    vector<string> transform;
    transform.reserve(6);
    split(temp, ",", transform);
#else
    vector<string> transform;
    transform.reserve(6);
    split(temp, ",", transform);
#endif
    if (6 != transform.size())
    {
        CCLOGERROR("Error while creating GAFSubobjectState. Initial transform data '%s'", temp.c_str());
        return false;
    }

    affineTransform = CCAffineTransformMake(
        static_cast<float>(atof(transform[0].c_str())),
        static_cast<float>(atof(transform[1].c_str())),
        static_cast<float>(atof(transform[2].c_str())),
        static_cast<float>(atof(transform[3].c_str())),
        static_cast<float>(atof(transform[4].c_str())),
        static_cast<float>(atof(transform[5].c_str()))
        );

    CCString * color = (CCString*)dict->objectForKey(kColorName);
    if (color)
    {
        std::string c(color->getCString());
#if USE_PLAIN_SPLIT
        vector<string> color;
        color.reserve(7);
        split(c, ",", color);
#else
        vector<string> color;
        color.reserve(7);
        split(c, ",", color);
#endif
        if (7 != color.size())
        {
            CCLOGERROR("Error while creating GAFSubobjectState. Invalid color data '%s'", c.c_str());
            return false;
        }
        _colorOffsets[GAFCTI_A] = static_cast<float>(atof(color[0].c_str()));

        _colorMults[GAFCTI_R] = static_cast<float>(atof(color[1].c_str()));
        _colorOffsets[GAFCTI_R] = static_cast<float>(atof(color[2].c_str()));

        _colorMults[GAFCTI_G] = static_cast<float>(atof(color[3].c_str()));
        _colorOffsets[GAFCTI_G] = static_cast<float>(atof(color[4].c_str()));

        _colorMults[GAFCTI_B] = static_cast<float>(atof(color[5].c_str()));
        _colorOffsets[GAFCTI_B] = static_cast<float>(atof(color[6].c_str()));
    }
    else
    {
        _colorOffsets[0] = _colorOffsets[1] = _colorOffsets[2] = _colorOffsets[3] = 0;
        _colorMults[GAFCTI_R] = _colorMults[GAFCTI_G] = _colorMults[GAFCTI_B] = 1;
    }

    CCString * mask = (CCString*)dict->objectForKey(kMaskName);
    if (mask)
    {
        maskObjectId = mask->getCString();
    }
    CCArray * effects = (CCArray*)dict->objectForKey(kEffectsName);
    CC_SAFE_DELETE(_filters);
    if (effects)
    {
        _filters = CCDictionary::create();
        _filters->retain();
        for (unsigned int i = 0; i < effects->count(); ++i)
        {
            CCDictionary * dict = (CCDictionary*)effects->objectAtIndex(i);
            CCString * type = (CCString*)dict->objectForKey(kEffectTypeName);
            if (type)
            {
                if (0 == strcmp(type->getCString(), kGAFBlurFilterName))
                {
                    CCString * x = (CCString*)dict->objectForKey(kXName);
                    CCString * y = (CCString*)dict->objectForKey(kYName);
                    if (x && y)
                    {
                        float bx = static_cast<float>(atof(x->getCString()));
                        float by = static_cast<float>(atof(y->getCString()));
                        GAFBlurFilterData * blurFilter = GAFBlurFilterData::create(bx, by);
                        _filters->setObject(blurFilter, kGAFBlurFilterName);
                    }
                }
            }
            else
            {
                CCLOGWARN("Can not determine effect type, skipping it");
            }
        }
    }
    return true;
}

