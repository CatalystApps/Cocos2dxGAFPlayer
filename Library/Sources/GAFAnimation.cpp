#include "GAFPrecompiled.h"
#include "GAFAnimation.h"
#include "GAFAnimationSequence.h"
#include "GAFAsset.h"
#include "GAFAnimatedObject.h"

GAFAnimation::GAFAnimation()
:
_asset(NULL),
_currentFrameIndex(GAF_FIRST_FRAME_INDEX),
_sequenceDelegate(NULL),
m_animationPlaybackDelegate(NULL),
_totalFrameCount(0),
_isRunning(false),
_isReversed(false),
_isLooped(false)
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
    if (! _isReversed)
    {
        if (_currentFrameIndex < _currentSequenceStart)
        {
            _currentFrameIndex = _currentSequenceStart;
        }

        if (_sequenceDelegate && _asset)
        {
            const GAFAnimationSequence * seq = _asset->getSequenceByLastFrame(_currentFrameIndex);
            if (seq)
            {
                _sequenceDelegate->onFinishSequence(dynamic_cast<GAFAnimatedObject*>(this), seq->name);
            }
        }

        if (_currentFrameIndex >= _currentSequenceEnd)
        {
            if (_isLooped)
            {
                _currentFrameIndex = _currentSequenceStart;
                
                if (m_animationPlaybackDelegate)
                {
                    m_animationPlaybackDelegate->onAnimationStartedNextLoopDelegate(this);
                }
            }
            else
            {
                setAnimationRunning(false);

                if (m_animationPlaybackDelegate)
                {
                    m_animationPlaybackDelegate->onAnimationFinishedPlayDelegate(this);
                }

                return;
            }
        }

        processAnimation();

        ++_currentFrameIndex;
    }
    else
    {
        // If switched to reverse after final frame played
        if (_currentFrameIndex >= _currentSequenceEnd)
        {
            _currentFrameIndex = _currentSequenceEnd - 1;
        }

        if (_sequenceDelegate && _asset)
        {
            const GAFAnimationSequence * seq = _asset->getSequenceByFirstFrame(_currentFrameIndex + 1);
            if (seq)
            {
                _sequenceDelegate->onFinishSequence(dynamic_cast<GAFAnimatedObject*>(this), seq->name);
            }
        }

        if (_currentFrameIndex < _currentSequenceStart)
        {
            if (_isLooped)
            {
                _currentFrameIndex = _currentSequenceEnd - 1;

                if (m_animationPlaybackDelegate)
                {
                    m_animationPlaybackDelegate->onAnimationStartedNextLoopDelegate(this);
                }
            }
            else
            {
                setAnimationRunning(false);

                if (m_animationPlaybackDelegate)
                {
                    m_animationPlaybackDelegate->onAnimationFinishedPlayDelegate(this);
                }

                return;
            }
        }

        processAnimation();

        --_currentFrameIndex;
    }
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
        if (!_isReversed)
        {
            return _currentFrameIndex > _totalFrameCount;
        }
        else
        {
            return _currentFrameIndex < GAF_FIRST_FRAME_INDEX - 1;
        }
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
        setAnimationRunning(true);
    }
}

void GAFAnimation::pause()
{
    if (_isRunning)
    {
        setAnimationRunning(false);
    }
}

void GAFAnimation::resume()
{
    if (!_isRunning)
    {
        setAnimationRunning(true);
    }
}

void GAFAnimation::stop()
{
    if (_isRunning)
    {
        _currentFrameIndex = GAF_FIRST_FRAME_INDEX;
        setAnimationRunning(false);
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
        setAnimationRunning(false);
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
        setAnimationRunning(true);
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
bool GAFAnimation::isReversed() const
{
    return _isReversed;
}
void GAFAnimation::setReversed(bool reversed)
{
    _isReversed = reversed;
}

void GAFAnimation::setAnimationRunning(bool value)
{
    _isRunning = value;
}

bool GAFAnimation::getIsRunning() const
{
    return _isRunning;
}

void GAFAnimation::setAnimationPlaybackDelegate(GAFAnimationPlaybackDelegate* delegate)
{
    m_animationPlaybackDelegate = delegate;
}
