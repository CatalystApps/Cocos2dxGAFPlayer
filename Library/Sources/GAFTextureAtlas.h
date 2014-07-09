#pragma once

#ifndef __GAF_TEXTURE_ATLAS___
#define __GAF_TEXTURE_ATLAS___

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
    cocos2d::__Array      *  _images;
    cocos2d::__Array      *  _textures;
public:
    ~GAFTextureAtlas();

    void                    pushAtlasInfo(const AtlasInfo& ai);
    void                    pushElement(unsigned int idx, GAFTextureAtlasElement* el);

    void                    loadImages(const std::string& dir, GAFTextureLoadDelegate* delegate, cocos2d::ZipFile* bundle = nullptr);

    inline bool     loaded() const
    {
        return _loaded;
    }

    void          setScale(float val);
    float         getScale() const;

    cocos2d::Image     * image();
    cocos2d::__Array     * images();

    cocos2d::Texture2D * texture();
    cocos2d::__Array     * textures();

    const Elements_t& getElements() const;
};

#endif // __GAF_TEXTURE_ATLAS___
