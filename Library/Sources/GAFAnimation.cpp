#include "GAFPrecompiled.h"
#include "GAFAnimation.h"
#include "GAFAnimationSequence.h"
#include "GAFAsset.h"
#include "GAFAnimatedObject.h"

GAFAnimation::GAFAnimation()
:
m_asset(NULL),
_currentFrameIndex(GAF_FIRST_FRAME_INDEX),
_sequenceDelegate(NULL),
m_animationPlaybackDelegate(NULL),
m_totalFrameCount(0),
m_isRunning(false),
m_isReversed(false),
m_isLooped(false)
{

}

GAFAnimation::~GAFAnimation()
{
    CC_SAFE_RELEASE(m_asset);
}

void GAFAnimation::setSequenceDelegate(GAFSequenceDelegate * delegate)
{
    _sequenceDelegate = delegate;
}


bool GAFAnimation::init(GAFAsset * anAssetData)
{
    CCAssert(anAssetData, "anAssetData data should not be nil");
    if (m_asset != anAssetData)
    {
        CC_SAFE_RELEASE(m_asset);
        m_asset = anAssetData;
        CC_SAFE_RETAIN(m_asset);
    }
    m_currentSequenceStart = _currentFrameIndex = GAF_FIRST_FRAME_INDEX;
    m_currentSequenceEnd = m_totalFrameCount = anAssetData->getAnimationFramesCount();
    return true;
}

void GAFAnimation::clearSequence()
{
    m_currentSequenceStart = GAF_FIRST_FRAME_INDEX;
    m_currentSequenceEnd = m_totalFrameCount;
}

void GAFAnimation::step()
{
    if (! m_isReversed)
    {
        if (_currentFrameIndex < m_currentSequenceStart)
        {
            _currentFrameIndex = m_currentSequenceStart;
        }

        if (_sequenceDelegate && m_asset)
        {
            const GAFAnimationSequence * seq = m_asset->getSequenceByLastFrame(_currentFrameIndex);
            if (seq)
            {
                _sequenceDelegate->onFinishSequence(dynamic_cast<GAFAnimatedObject*>(this), seq->name);
            }
        }

        if (_currentFrameIndex >= m_currentSequenceEnd)
        {
            if (m_isLooped)
            {
                _currentFrameIndex = m_currentSequenceStart;
                
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
        if (_currentFrameIndex >= m_currentSequenceEnd)
        {
            _currentFrameIndex = m_currentSequenceEnd - 1;
        }

        if (_sequenceDelegate && m_asset)
        {
            const GAFAnimationSequence * seq = m_asset->getSequenceByFirstFrame(_currentFrameIndex + 1);
            if (seq)
            {
                _sequenceDelegate->onFinishSequence(dynamic_cast<GAFAnimatedObject*>(this), seq->name);
            }
        }

        if (_currentFrameIndex < m_currentSequenceStart)
        {
            if (m_isLooped)
            {
                _currentFrameIndex = m_currentSequenceEnd - 1;

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
    if (m_isLooped)
    {
        return false;
    }
    else
    {
        if (!m_isReversed)
        {
            return _currentFrameIndex > m_totalFrameCount;
        }
        else
        {
            return _currentFrameIndex < GAF_FIRST_FRAME_INDEX - 1;
        }
    }
}

int GAFAnimation::totalFrameCount() const
{
    return m_totalFrameCount;
}

int GAFAnimation::currentFrameIndex() const
{
    return _currentFrameIndex;
}

bool GAFAnimation::isAnimationRunning() const
{
    return m_isRunning;
}

bool GAFAnimation::isLooped() const
{
    return m_isLooped;
}

void GAFAnimation::setLooped(bool looped)
{
    m_isLooped = looped;
}

void GAFAnimation::start()
{
    if (!m_isRunning)
    {
        _currentFrameIndex = GAF_FIRST_FRAME_INDEX;
        setAnimationRunning(true);
    }
}

void GAFAnimation::pauseAnimation()
{
    if (m_isRunning)
    {
        setAnimationRunning(false);
    }
}

void GAFAnimation::resumeAnimation()
{
    if (!m_isRunning)
    {
        setAnimationRunning(true);
    }
}

void GAFAnimation::stop()
{
    if (m_isRunning)
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
    if (aFrameIndex >= 0 && aFrameIndex < m_totalFrameCount)
    {
        _currentFrameIndex = aFrameIndex;
        processAnimation();
        return true;
    }
    return false;
}

int GAFAnimation::getStartFrame(const char * sequence_name)
{
    if (!m_asset)
    {
        return -1;
    }
    const GAFAnimationSequence * seq = m_asset->getSequence(sequence_name);
    if (seq)
    {
        return seq->startFrameNo;
    }
    return -1;
}

int GAFAnimation::getEndFrame(const char * sequence_name)
{
    if (!m_asset)
    {
        return -1;
    }
    const GAFAnimationSequence * seq = m_asset->getSequence(sequence_name);
    if (seq)
    {
        return seq->endFrameNo;
    }
    return -1;
}

bool GAFAnimation::playSequence(const char * name, bool looped, bool _resume, AnimSetSequenceHint hint)
{
    if (!m_asset)
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
    m_currentSequenceStart = s;
    m_currentSequenceEnd = e;

    if (_currentFrameIndex < m_currentSequenceStart || _currentFrameIndex >m_currentSequenceEnd)
    {
        _currentFrameIndex = m_currentSequenceStart;
    }
    else
    {
        if (hint == ASSH_RESTART)
        {
            _currentFrameIndex = m_currentSequenceStart;
        }
        else
        {
            // new hints may appear
        }
    }
    setLooped(looped);
    if (_resume)
    {
        resumeAnimation();
    }
    else
    {
        stop();
    }
    return true;
}
bool GAFAnimation::hasSequences() const
{
    return !m_asset->getAnimationSequences().empty();
}
bool GAFAnimation::isReversed() const
{
    return m_isReversed;
}
void GAFAnimation::setReversed(bool reversed)
{
    m_isReversed = reversed;
}

void GAFAnimation::setAnimationRunning(bool value)
{
    m_isRunning = value;
}

bool GAFAnimation::getIsRunning() const
{
    return m_isRunning;
}

void GAFAnimation::setAnimationPlaybackDelegate(GAFAnimationPlaybackDelegate* delegate)
{
    m_animationPlaybackDelegate = delegate;
}
