#pragma once

NS_GAF_BEGIN

class GAFSprite;
class GAFObject;

typedef std::function<void(GAFObject* object, const std::string& sequenceName)>    GAFSequenceDelegate_t;
typedef std::function<void(GAFObject* obj)>                                        GAFAnimationFinishedPlayDelegate_t;
typedef std::function<void(GAFObject* obj)>                                        GAFAnimationStartedNextLoopDelegate_t;
typedef std::function<void(std::string*)>                                          GAFTextureLoadDelegate_t;
typedef std::function<void(GAFObject* obj, uint32_t frame)>                        GAFFramePlayedDelegate_t;
typedef std::function<void(GAFObject* object, const GAFSprite * subobject)>        GAFObjectControlDelegate_t;

NS_GAF_END