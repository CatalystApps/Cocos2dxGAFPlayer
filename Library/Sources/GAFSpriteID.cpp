#include "GAFPrecompiled.h"
#include "GAFSpriteID.h"
#include "GAFCollections.h"

#include "GAFAnimatedObject.h"

GAFSpriteID GAFSpriteID::IDNONE = { nullptr, ::IDNONE };

bool GAFSpriteID::operator==(const GAFSpriteID& oth) const
{
    return oth.owner == owner && oth.id == id;
}

bool GAFSpriteID::operator!=(const GAFSpriteID& oth) const
{
    return !(*this == oth);
}

GAFSprite* GAFSpriteID::getSprite() const
{
    if (*this == GAFSpriteID::IDNONE)
    {
        return nullptr;
    }

    return owner->getSprite(id);
}

GAFAnimatedObject* GAFSpriteID::getAnimatedObject() const
{
    if (*this == GAFSpriteID::IDNONE)
    {
        return nullptr;
    }

    return owner->getSubObject(id);
}
