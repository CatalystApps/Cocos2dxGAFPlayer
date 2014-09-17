#pragma once

class GAFAnimatedObject;
class GAFSprite;

class GAFSpriteID
{
public:

    static GAFSpriteID IDNONE;

    GAFAnimatedObject* owner;
    uint32_t           id;

    bool operator==(const GAFSpriteID& oth) const;
    bool operator!=(const GAFSpriteID& oth) const;

    GAFSprite* getSprite() const;
    GAFAnimatedObject* getAnimatedObject() const;
};
