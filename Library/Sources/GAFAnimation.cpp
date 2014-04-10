#include "GAFPrecompiled.h"
#include "GAFAnimation.h"
#include "GAFAnimationSequence.h"
#include "GAFAsset.h"
#include "GAFAnimatedObject.h"

GAFAnimation::GAFAnimation()
:
_asset(NULL),
_isRunning(false),
_currentFrameIndex(GAF_FIRST_FRAME_INDEX),
_totalFrameCount(0),
_sequenceDelegate(NULL)
{

}

GAFAnimation::~GAFAnimation()
{
    CC_SAFE_RELEASE(_asset);
}

void GAFAnimation::setSequenceDelegate(GAFSequenceDelegate * delegate)
{
    _sequenceDelegate = delegate;
}


bool GAFAnimation::init(GAFAsset * anAssetData)
{
    CCAssert(anAssetData, "anAssetData data should not be nil");
    if (_asset != anAssetData)
    {
        CC_SAFE_RELEASE(_asset);
        _asset = anAssetData;
        CC_SAFE_RETAIN(_asset);
    }
    _currentSequenceStart = _currentFrameIndex = GAF_FIRST_FRAME_INDEX;
    _currentSequenceEnd = _totalFrameCount = anAssetData->animationFramesCount();
    return true;
}

void GAFAnimation::clearSequence()
{
    _currentSequenceStart = GAF_FIRST_FRAME_INDEX;
    _currentSequenceEnd = _totalFrameCount;
}

void GAFAnimation::step()
{
    if (_sequenceDelegate && _asset)
    {
        const GAFAnimationSequence * seq = _asset->getSequenceByLastFrame(_currentFrameIndex);
        if (seq)
        {
            _sequenceDelegate->onFinishSequence(dynamic_cast<GAFAnimatedObject*>(this), seq->name);
        }
    }

    if (_isLooped)
    {
        if (_currentFrameIndex >= _currentSequenceEnd)
        {
            _currentFrameIndex = _currentSequenceStart;
        }
    }
    else
    {
        if (_currentFrameIndex >= _currentSequenceEnd)
        {
            _isRunning = false;
            return;
        }
    }

    processAnimation();

    ++_currentFrameIndex;
}

void GAFAnimation::processAnimation()
{

}

bool GAFAnimation::isDone() const
{
    if (_isLooped)
    {
        return false;
    }
    else
    {
        return _currentFrameIndex >= _totalFrameCount;
    }
}

int GAFAnimation::totalFrameCount() const
{
    return _totalFrameCount;
}

int GAFAnimation::currentFrameIndex() const
{
    return _currentFrameIndex;
}

bool GAFAnimation::isAnimationRunning() const
{
    return _isRunning;
}

bool GAFAnimation::isLooped() const
{
    return _isLooped;
}

void GAFAnimation::setLooped(bool looped)
{
    _isLooped = looped;
}

void GAFAnimation::start()
{
    if (!_isRunning)
    {
        _currentFrameIndex = GAF_FIRST_FRAME_INDEX;
        _isRunning = true;
    }
}

void GAFAnimation::pause()
{
    if (_isRunning)
    {
        _isRunning = false;
    }
}

void GAFAnimation::resume()
{
    if (!_isRunning)
    {
        _isRunning = true;
    }
}

void GAFAnimation::stop()
{
    if (_isRunning)
    {
        _currentFrameIndex = GAF_FIRST_FRAME_INDEX;
        _isRunning = false;
    }
}

bool GAFAnimation::gotoAndStop(const char * frameLabel)
{
    int f = getStartFrame(frameLabel);
    if (-1 == f)
    {
        return false;
    }
    return gotoAndStop(f);
}

bool GAFAnimation::gotoAndStop(int frameNumber)
{
    if (setFrame(frameNumber))
    {
        _isRunning = false;
        return true;
    }
    return false;
}

bool GAFAnimation::gotoAndPlay(const char * frameLabel)
{
    int f = getStartFrame(frameLabel);
    if (-1 == f)
    {
        return false;
    }
    return gotoAndPlay(f);
}

bool GAFAnimation::gotoAndPlay(int frameNumber)
{
    if (setFrame(frameNumber))
    {
        _isRunning = true;
        return true;
    }
    return false;
}


bool GAFAnimation::setFrame(int aFrameIndex)
{
    if (aFrameIndex >= 0 && aFrameIndex < _totalFrameCount)
    {
        _currentFrameIndex = aFrameIndex;
        processAnimation();
        return true;
    }
    return false;
}

int GAFAnimation::getStartFrame(const char * sequence_name)
{
    if (!_asset)
    {
        return -1;
    }
    const GAFAnimationSequence * seq = _asset->getSequence(sequence_name);
    if (seq)
    {
        return seq->startFrameNo;
    }
    return -1;
}

int GAFAnimation::getEndFrame(const char * sequence_name)
{
    if (!_asset)
    {
        return -1;
    }
    const GAFAnimationSequence * seq = _asset->getSequence(sequence_name);
    if (seq)
    {
        return seq->endFrameNo;
    }
    return -1;
}

bool GAFAnimation::playSequence(const char * name, bool looped, bool _resume, AnimSetSequenceHint hint)
{
    if (!_asset)
    {
        return false;
    }
    if (!name)
    {
        return false;
    }
    int s = getStartFrame(name);
    int e = getEndFrame(name);
    if (-1 == s || -1 == e)
    {
        return false;
    }
    _currentSequenceStart = s;
    _currentSequenceEnd = e;

    if (_currentFrameIndex < _currentSequenceStart || _currentFrameIndex >_currentSequenceEnd)
    {
        _currentFrameIndex = _currentSequenceStart;
    }
    else
    {
        if (hint == ASSH_RESTART)
        {
            _currentFrameIndex = _currentSequenceStart;
        }
        else
        {
            // new hints may appear
        }
    }
    setLooped(looped);
    if (_resume)
    {
        resume();
    }
    else
    {
        stop();
    }
    return true;
}
bool GAFAnimation::hasSequences() const
{
    return !_asset->getAnimationSequences().empty();
}