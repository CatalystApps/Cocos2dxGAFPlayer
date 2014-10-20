#pragma once

#define GAF_FIRST_FRAME_INDEX 0

#include "GAFDelegates.h"

NS_GAF_BEGIN

class GAFAsset;

enum AnimSetSequenceHint
{
    ASSH_CONTINUE,
    ASSH_RESTART
};


class GAFAnimation
{
public:
    ~GAFAnimation();
    bool init(GAFAsset * anAnimationData);

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
    int totalFrameCount() const;
    int currentFrameIndex() const;

    bool setFrame(int index);

    /// Plays specified frame and then stops	
    bool gotoAndStop(const char * frameLabel);
    bool gotoAndStop(int frameNumber);

    /// Plays animation from specified frame	
    bool gotoAndPlay(const char * frameLabel);
    bool gotoAndPlay(int frameNumber);


    int getStartFrame(const char * frameLabel);
    int getEndFrame(const char * frameLabel);

    /// Plays animation sequence with specified name
    /// @param name a sequence name
    /// @param looped if true - sequence should play in cycle
    /// @param resume if true - animation will be played immediately, if false - playback will be paused after the first frame is shown
    /// @param hint specific animation playback parameters

    bool playSequence(const char * name, bool looped = false, bool resume = true, AnimSetSequenceHint hint = ASSH_RESTART);
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
    size_t _currentFrameIndex;

    void    setAnimationRunning(bool value);
    bool    getIsRunning() const;

private:
    GAFSequenceDelegate * _sequenceDelegate;
    GAFAnimationPlaybackDelegate* m_animationPlaybackDelegate;
    size_t m_totalFrameCount;
    size_t m_currentSequenceStart;
    size_t m_currentSequenceEnd;
    bool m_isRunning;
    bool m_isLooped;
    bool m_isReversed;
};

NS_GAF_END