#include "GAFAsset.h"
#include "GAFData.h"
#include "CCJSONConverter.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"
#include "GAFAnimationFrame.h"
#include "GAFSubobjectState.h"
#include "GAFInteractionObject.h"
#include "GAFActionObject.h"
#include "GAFAnimationSequence.h"
#include "GAFAnimatedObject.h"
#include "platform/CCFileUtils.h"
#include "cocoa/CCInteger.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCDictionary.h"

static const char *  kAnimationFrameCountKey = "animationFrameCount";
static const char *  kAnimationConfigFramesKey = "animationConfigFrames";
static const char *  kAnimationObjectsKey = "animationObjects";
static const char *  kAnimationMasksKey = "animationMasks";
static const char *  kAnimationNamedPartsKey = "namedParts";
static const char *  kInteractionObjectsKey = "interactionObject";
static const char *  kStandObjectsKey = "standObjects";
static const char *  kTextureAtlasKey = "textureAtlas";
static const char *  kAnimationSequencesKey = "animationSequences";
static const char *  kVersionKey = "version";

static const char *  kFrameNumberKey = "frameNumber";
static const char *  kFrameStateKey = "state";

static const char *  kGAFAnimationSequenceIdKey = "id";
static const char *  kGAFAnimationSequenceStartFrameNoKey = "startFrameNo";
static const char *  kGAFAnimationSequenceEndFrameNoKey = "endFrameNo";

static const char *  kAtlasScaleKey = "scale";
static const char *  kAtlasInfoKey  = "atlases";

static float _currentDeviceScale      = 1.0f;
static bool  _makeExactScaleForObject = false;
static int   _desiredCsf = 1;

int GAFAsset::desiredCsf()
{
	return _desiredCsf;
}

void GAFAsset::setDesiredCsf(int csf)
{
	_desiredCsf = csf;
}

GAFAnimatedObject * GAFAsset::createObject()
{
	if (!_textureAtlas)
	{
		return 0;
	}
	return GAFAnimatedObject::create(this);
}

GAFAnimatedObject * GAFAsset::createObjectAndRun(bool looped)
{
	GAFAnimatedObject * res = createObject();
	if (res)
	{
		res->setLooped(looped);
		res->start();
	}
	return res;
}


GAFAsset * GAFAsset::create(const std::string& jsonPath)
{
    GAFAsset *pRet = new GAFAsset();
    if (pRet && pRet->initWithImageData(jsonPath.c_str()))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}


GAFAsset::GAFAsset()
:
_textureAtlas(NULL),
_objects(NULL),
_masks(NULL),
_interactionObjects(NULL),
_standObjects(NULL),
_animationFrames(NULL),
_animationSequences(NULL),
_namedParts(NULL)
{
}

GAFAsset::~GAFAsset()
{
	CC_SAFE_RELEASE(_textureAtlas);
	CC_SAFE_RELEASE(_objects);
	CC_SAFE_RELEASE(_masks);
	CC_SAFE_RELEASE(_interactionObjects);
	CC_SAFE_RELEASE(_standObjects);
	CC_SAFE_RELEASE(_animationFrames);
	CC_SAFE_RELEASE(_animationSequences);
	CC_SAFE_RELEASE(_namedParts);	
}

bool GAFAsset::isAssetVersionPlayable(const char * version)
{
	return true;
}

float GAFAsset::atlasScaleFromAtlasConfig(CCDictionary * anAtlasConfigDictionary)
{
	if (!anAtlasConfigDictionary)
	{
		return 0;
	}
	CCNumber * scale = (CCNumber * )anAtlasConfigDictionary->objectForKey(kAtlasScaleKey);	
	return scale->getDoubleValue();
}

bool GAFAsset::initWithImageData(const std::string& jsonPath)
{
		
	GAFData aConfigData;
	std::string fp = CCFileUtils::sharedFileUtils()->fullPathForFilename(jsonPath.c_str());
	
	aConfigData.delete_data = true;
	aConfigData.ptr = CCFileUtils::sharedFileUtils()->getFileData(fp.c_str(), "rb", &aConfigData.size);
	if (!aConfigData.ptr)
	{
		CCLOGERROR("Can not get data from json file : %s", jsonPath.c_str());
		return NULL;
	}

	if (!aConfigData.getBytes())
	{
		CCLOGWARN("can not init GAFAsset - invalid anImageData");
		return false;
	}
	
	CCDictionary* configDictionary = CCJSONConverter::sharedConverter()->dictionaryFrom( (const char *)aConfigData.getBytes());
	
	CCString *versionNode               = (CCString*)configDictionary->objectForKey(kVersionKey);
	
	if (!isAssetVersionPlayable(versionNode->getCString()))
	{
		return false;
	}
	CCArray *animationConfigFrames      = (CCArray *)configDictionary->objectForKey(kAnimationConfigFramesKey);
	CCArray *interactionObjectNodes     = (CCArray *)configDictionary->objectForKey(kInteractionObjectsKey);
	CCArray *standObjectsNodes          = (CCArray *)configDictionary->objectForKey(kStandObjectsKey);
	CCArray *textureAtlasNode           = (CCArray *)configDictionary->objectForKey(kTextureAtlasKey);
	CCArray *animationSequences         = (CCArray *)configDictionary->objectForKey(kAnimationSequencesKey);
	
	CCDictionary *objectNodes           = (CCDictionary *)configDictionary->objectForKey(kAnimationObjectsKey);
	CCDictionary *masksNodes            = (CCDictionary *)configDictionary->objectForKey(kAnimationMasksKey);
	CCDictionary *namedPartsNodes       = (CCDictionary *)configDictionary->objectForKey(kAnimationNamedPartsKey);

	
	if (!animationConfigFrames || !textureAtlasNode|| !objectNodes)
	{
		CCLOGERROR("Error while creating GAFAsset. Required subnodes in dictionary are missing.");
		return false;
	}
	
	CC_SAFE_RELEASE(_textureAtlas);
	
	if (!textureAtlasNode->count())
	{
		return false;
	}

	CCDictionary * atlasDictionary = (CCDictionary *)textureAtlasNode->objectAtIndex(0);
	float atlasScale = atlasScaleFromAtlasConfig(atlasDictionary);
	for (int i = 1; i < textureAtlasNode->count(); ++i)
	{
		CCDictionary * a = (CCDictionary *)textureAtlasNode->objectAtIndex(i);
		float as = atlasScaleFromAtlasConfig(a);
		if ( fabs(atlasScale - _currentDeviceScale) > fabs(as - _currentDeviceScale))
		{
			atlasDictionary = a;
			atlasScale = as;
		}
	}
	
	_usedAtlasContentScaleFactor = atlasScale;
	CCArray * atlasesInfo = (CCArray *)atlasDictionary->objectForKey(kAtlasInfoKey);
	if (!atlasesInfo)
	{
		CCLOGERROR("Error while creating GAFAsset.atlasesInfo subnode is missing in atlasDictionary.");
		return false;
	}
	
	_textureAtlas = GAFTextureAtlas::create(fp.c_str(), atlasDictionary);
	if (!_textureAtlas)
	{
		CCLOGERROR("Failed to initialize GAFAsset. GAFTextureAtlas could not be created.");
		return false;
	}
	CC_SAFE_RETAIN(_textureAtlas);
	
	if (_objects != objectNodes)
	{
		CC_SAFE_RELEASE(_objects);
		_objects = objectNodes;
		CC_SAFE_RETAIN(_objects);
	}
	
	if (_masks != masksNodes)
	{
		CC_SAFE_RELEASE(_masks);
		_masks	  = masksNodes;
		CC_SAFE_RETAIN(_masks);
	}
	
	if (_namedParts != namedPartsNodes)
	{
		CC_SAFE_RELEASE(_namedParts);
		_namedParts	  = namedPartsNodes;
		CC_SAFE_RETAIN(_namedParts);
	}
	
	if (interactionObjectNodes)
	{
		CC_SAFE_RELEASE(_interactionObjects);
		_interactionObjects = CCArray::create();
		CC_SAFE_RETAIN(_interactionObjects);
		
		for (int i = 0; i < interactionObjectNodes->count(); ++i)
		{
			CCDictionary * dict = (CCDictionary*)interactionObjectNodes->objectAtIndex(i);
			
			GAFInteractionObject * interObject = GAFInteractionObject::create(dict);
			if (interObject)
			{
				_interactionObjects->addObject(interObject);
			}
		}
	}
	
	if (standObjectsNodes)
	{
		CC_SAFE_RELEASE(_standObjects);
		_standObjects = CCArray::create();
		CC_SAFE_RETAIN(_standObjects);
		
		for (int i = 0; i < standObjectsNodes->count(); ++i)
		{
			CCDictionary * dict = (CCDictionary*)standObjectsNodes->objectAtIndex(i);
			
			GAFActionObject * interObject = GAFActionObject::create(dict);
			if (interObject)
			{
				_standObjects->addObject(interObject);
			}
		}
	}
	
	loadFramesFromConfigDictionary(configDictionary);
	
	if (animationSequences)
	{
		loadAnimationSequences(animationSequences);
	}

	configDictionary->removeAllObjects();  //free memory
	return true;
}

CCArray * GAFAsset::objectStatesFromConfigFrame(CCDictionary * configFrame)
{
	if (!configFrame)
	{
		return NULL;
	}
	CCDictionary * stateNodes = (CCDictionary *) configFrame->objectForKey(kFrameStateKey);
	if (!stateNodes)
	{
		return NULL;
	}
    CCArray *states = CCArray::createWithCapacity(stateNodes->count());
	
	CCDictElement* pElement = NULL;
    CCDICT_FOREACH(stateNodes, pElement)
    {
		CCDictionary * stateDictionary = (CCDictionary *)pElement->getObject();
		GAFSubobjectState * state = GAFSubobjectState::createWithStateDictionary(stateDictionary, pElement->getStrKey());
		if (state)
		{
			states->addObject(state);
		}
		else
		{
			CCLOGINFO("GAFSubobjectState cannot be created. Ignoring.");
		}
	}
    return states;
}

void GAFAsset::loadAnimationSequences(CCArray * aSequencesNodes)
{
	CC_SAFE_RELEASE(_animationSequences);
	_animationSequences = CCDictionary::create();
	for (int i = 0; i < aSequencesNodes->count(); ++i)
	{
		CCDictionary * sequenceNode = (CCDictionary *)aSequencesNodes->objectAtIndex(i);
		CCString  * sequenceId   = (CCString *)  sequenceNode->objectForKey(kGAFAnimationSequenceIdKey);
		CCInteger * startFrameNo = (CCInteger *) sequenceNode->objectForKey(kGAFAnimationSequenceStartFrameNoKey);
		CCInteger * endFrameNo   = (CCInteger *) sequenceNode->objectForKey(kGAFAnimationSequenceEndFrameNoKey);
		if (sequenceId && startFrameNo && endFrameNo)
		{
			GAFAnimationSequence *sequence = new GAFAnimationSequence();
			sequence->name         = sequenceId->getCString();
			sequence->startFrameNo = startFrameNo->getValue();
			sequence->endFrameNo   = endFrameNo->getValue();
			_animationSequences->setObject(sequence, sequenceId->getCString());
			sequence->release();
		}
		else
		{
			CCLOGERROR("Error while creating GAFAsset. aSequencesNodes cannot be parsed.");
		}
	}
	CC_SAFE_RETAIN(_animationSequences);
}

void GAFAsset::loadFramesFromConfigDictionary(CCDictionary * aConfigDictionary)
{
	CCAssert(_objects, "objects != NULL");
	CCDictionary * currentStates =  CCDictionary::create();
	
	CC_SAFE_RELEASE(_animationFrames);
	_animationFrames = CCArray::create();
	CC_SAFE_RETAIN(_animationFrames);
	
	CCArray *animationConfigFrames = (CCArray *)aConfigDictionary->objectForKey(kAnimationConfigFramesKey);
	
	CCDictElement* pElement = NULL;
    CCDICT_FOREACH(_objects, pElement)
    {
		const char * key = pElement->getStrKey();
		GAFSubobjectState *state = GAFSubobjectState::createEmptyWithObjectId(key);
		currentStates->setObject(state, key);
    } 
	CCInteger * configFrameCountInt = (CCInteger *)aConfigDictionary->objectForKey(kAnimationFrameCountKey);
	int configFrameCount = configFrameCountInt ? configFrameCountInt->getValue() : 0;
	int configFrameIndex = 0;
	for (int index = 0; index < configFrameCount; ++index)
	{
		if (configFrameIndex < animationConfigFrames->count())
		{
			CCDictionary *configFrame = (CCDictionary *)animationConfigFrames->objectAtIndex(configFrameIndex);
			CCInteger * configFrameNoInt = (CCInteger *) configFrame->objectForKey(kFrameNumberKey);
			int configFrameNo = configFrameNoInt ? configFrameNoInt->getValue() : -1;
			if (configFrameNo - 1 == index)
			{
				CCArray * newStates = objectStatesFromConfigFrame(configFrame);
				for (int j = 0; j < newStates->count(); ++j)
				{
					GAFSubobjectState * state = (GAFSubobjectState *)newStates->objectAtIndex(j);
					currentStates->setObject(state, state->objectId.c_str());
				}
				++configFrameNo;
				++configFrameIndex;
			}
		}
		GAFAnimationFrame * frame = new GAFAnimationFrame();
		CCDictElement* s = NULL;
		CCArray * allValues = CCArray::create();
		CCDICT_FOREACH(currentStates, s)
		{
			allValues->addObject(s->getObject());
		}
		frame->setObjectStates(allValues);
		_animationFrames->addObject(frame);
		frame->release();
	}
}

CCDictionary * GAFAsset::objects()
{
	return _objects;
}

CCDictionary   * GAFAsset::masks()
{
	return _masks;
}

CCDictionary   * GAFAsset::namedParts()
{
	return _namedParts;
}

CCArray        * GAFAsset::animationFrames()
{
	return _animationFrames;
}


GAFTextureAtlas * GAFAsset::textureAtlas()
{
	return _textureAtlas;
}

CCDictionary * GAFAsset::animationSequences()
{
	return _animationSequences;
}

int GAFAsset::animationFramesCount() const
{
	if (_animationFrames)
	{
		return _animationFrames->count();
	}
	else
	{
		return 0;
	}
}

GAFAnimationSequence * GAFAsset::getSequence(const char * name)
{
	if (!_animationSequences || !name)
	{
		return NULL;
	}
	return (GAFAnimationSequence *) _animationSequences->objectForKey(name);	
}

GAFAnimationSequence * GAFAsset::getSequenceByLastFrame(int frame)
{
	if (!_animationSequences)
	{
		return NULL;
	}
	CCDictElement* s = NULL;
	CCDICT_FOREACH(_animationSequences, s)
	{
		GAFAnimationSequence * seq = (GAFAnimationSequence*) s->getObject();
		if (seq->endFrameNo == frame)
		{
			return seq;
		}
	}
	return NULL;
}
