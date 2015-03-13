#pragma once

NS_GAF_BEGIN

class GAFResourcesInfo
{
public:
    enum class ResourceId : uint16_t
    {
        Texture = 0,
        Font,
        Sound, // this one for future usage
    };

    ResourceId id;
};

class GAFResourcesInfoTexture : public GAFResourcesInfo
{
public:
    GAFResourcesInfoTexture() { id = ResourceId::Texture; }
    GAFResourcesInfoTexture(std::string _source, float _csf)
    {
        id = ResourceId::Texture;
        source = _source;
        csf = _csf;
    }

    bool operator==(const GAFResourcesInfoTexture &other)
    {
        return source.compare(other.source) == 0 && fabs(csf - other.csf) < std::numeric_limits<float>::epsilon();
    }

    std::string source;
    float csf;
};

class GAFResourcesInfoFont : public GAFResourcesInfo
{
public:
    GAFResourcesInfoFont() { id = ResourceId::Font; }
    GAFResourcesInfoFont(std::string _name)
    {
        id = ResourceId::Font;
        name = _name;
    }

    bool operator==(const GAFResourcesInfoFont &other)
    {
        return name.compare(other.name) == 0;
    }

    std::string name;
};

NS_GAF_END