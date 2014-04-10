#pragma once

#ifndef __GAF_TEXTURE_ATLAS___
#define __GAF_TEXTURE_ATLAS___


namespace cocos2d
{
    class CCImage;
    class CCArray;
    class CCTexture2D;
}
using namespace cocos2d;

class GAFTextureAtlasElement;
class GAFTextureLoadDelegate;

class GAFTextureAtlas
{
public:

    struct AtlasInfo
    {
        struct Source
        {
            std::string source;
            float csf;
        };

        typedef std::vector<Source> Sources_t;

        Sources_t m_sources;
        unsigned int id;
    };

    typedef std::vector<AtlasInfo> AtlasInfos_t;
    typedef std::map<unsigned int, GAFTextureAtlasElement*> Elements_t;

    GAFTextureAtlas();
private:

    float           m_scale;
    AtlasInfos_t    m_atlasInfos;
    Elements_t      m_elements;

    bool           _loaded;
    CCArray      *  _images;
    CCArray      *  _textures;
public:
    ~GAFTextureAtlas();

    void                    pushAtlasInfo(const AtlasInfo& ai);
    void                    pushElement(unsigned int idx, GAFTextureAtlasElement* el);

    void                    loadImages(const std::string& dir, GAFTextureLoadDelegate* delegate);

    inline bool     loaded() const
    {
        return _loaded;
    }

    void          setScale(float val);
    float         getScale() const;

    CCImage     * image();
    CCArray     * images();

    CCTexture2D * texture();
    CCArray     * textures();

    const Elements_t& getElements() const;
};

#endif // __GAF_TEXTURE_ATLAS___
