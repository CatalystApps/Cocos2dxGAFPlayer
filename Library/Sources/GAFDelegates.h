#pragma once

class GAFAnimatedObject;
class GAFSprite;
class GAFAnimation;
class GAFObject;

typedef std::function<void (GAFAnimatedObject * object, const std::string& sequenceName)>   GAFSequenceDelegate_t;
typedef std::function<void(GAFObject* obj)>                                                 GAFAnimationFinishedPlayDelegate_t;
typedef std::function<void(GAFObject* obj)>                                                 GAFAnimationStartedNextLoopDelegate_t;
typedef std::function<void(std::string*)>                                                   GAFTextureLoadDelegate_t;
typedef std::function<void(GAFObject* obj, uint32_t frame)>                                 GAFFramePlayedDelegate_t;
typedef std::function<void(GAFAnimatedObject * object, const GAFSprite * subobject)>        GAFAnimatedObjectControlDelegate_t;