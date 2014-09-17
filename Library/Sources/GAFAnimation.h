#pragma once

#ifndef __GAF_ANIMATION__
#define __GAF_ANIMATION__

#include "GAFDelegates.h"

class GAFAsset;
class GAFTimeline;

enum AnimSetSequenceHint
{
    ASSH_CONTINUE,
    ASSH_RESTART
};

enum
{
    GAF_FIRST_FRAME_INDEX = 0
};

class GAFAnimation
{
public:
    ~GAFAnimation();
    bool init(GAFAsset * anAnimationData, GAFTimeline* tl);

    virtual void processAnimation();
    virtual void start();
    virtual void pauseAnimation();
    virtual void resumeAnimation();
    virtual void stop();
    virtual void step();
    /// @returns true if the animation is finished, otherwise NO	
    bool isDone() const;
    bool isAnimationRunning() const;
    bool isLooped() const;
    void setLooped(bool looped);
    bool isReversed() const;
    void setReversed(bool reversed);
    uint32_t totalFrameCount() const;
    uint32_t currentFrameIndex() const;

    bool setFrame(uint32_t index);

    /// Plays specified frame and then stops	
    bool gotoAndStop(const std::string& frameLabel);
    bool gotoAndStop(uint32_t frameNumber);

    /// Plays animation from specified frame	
    bool gotoAndPlay(const std::string& frameLabel);
    bool gotoAndPlay(uint32_t frameNumber);


    int getStartFrame(const std::string& frameLabel);
    int getEndFrame(const std::string& frameLabel);

    /// Plays animation sequence with specified name
    /// @param name a sequence name
    /// @param looped if true - sequence should play in cycle
    /// @param resume if true - animation will be played immediately, if false - playback will be paused after the first frame is shown
    /// @param hint specific animation playback parameters

    bool playSequence(const std::string& name, bool looped = false, bool resume = true, AnimSetSequenceHint hint = ASSH_RESTART);
    void clearSequence();

    /// @note do not forget to call setSequenceDelegate(NULL) before deleting your subscriber
    void setSequenceDelegate(GAFSequenceDelegate * delegate);

    /// @note do not forget to call setAnimationPlaybackDelegate(NULL) before deleting your subscriber
    void setAnimationPlaybackDelegate(GAFAnimationPlaybackDelegate* delegate);

    bool hasSequences() const;
protected:
    GAFAnimation();
protected:
    GAFAsset * m_asset;
    GAFTimeline* m_timeline;
    uint32_t m_currentFrameIndex;

    void    setAnimationRunning(bool value);
    bool    getIsRunning() const;

private:
    GAFSequenceDelegate * _sequenceDelegate;
    GAFAnimationPlaybackDelegate* m_animationPlaybackDelegate;
    uint32_t m_totalFrameCount;
    uint32_t m_currentSequenceStart;
    uint32_t m_currentSequenceEnd;
    bool m_isRunning;
    bool m_isLooped;
    bool m_isReversed;
};


#endif // __GAF_ANIMATION__
