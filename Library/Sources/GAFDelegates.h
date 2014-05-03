#pragma once

class GAFAnimatedObject;
class GAFSprite;
class GAFAnimation;

class GAFSequenceDelegate
{
public:
    virtual void onFinishSequence(GAFAnimatedObject * object, const std::string& sequenceName) = 0;
};

class GAFAnimationPlaybackDelegate
{
public:
    virtual void onAnimationFinishedPlayDelegate(GAFAnimation* animation) { }
    virtual void onAnimationStartedNextLoopDelegate(GAFAnimation* animation) { }
};

class GAFTextureLoadDelegate
{
public:

    virtual void onTexturePreLoad(std::string& path) = 0;
};


/// @class GAFFramePlayedDelegate
/// You can get notification when particular frame of any GAFAnimatedObject is played.
/// To do this you have to inherit GAFFramePlayedDelegate and call setFramePlayedDelegate
/// method of your GAFAnimatedObject

class GAFFramePlayedDelegate
{
public:
    /// Callback function, called by GAF.
    /// @param object - selected animated object
    /// @param frame - frame number that is just played

    virtual void onFramePlayed(GAFAnimatedObject * object, int frame) = 0;
};

class GAFAnimatedObjectControlDelegate
{
public:
    virtual void onFrameDisplayed(GAFAnimatedObject * object, const GAFSprite * subobject) = 0;
};