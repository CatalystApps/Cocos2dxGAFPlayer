#pragma once

#include "GAFDelegates.h"

NS_GAF_BEGIN

class GAFTextureAtlasElement;

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

	static bool compareAtlasesById(const AtlasInfo& ai1, const AtlasInfo& ai2);

    typedef std::vector<AtlasInfo> AtlasInfos_t;
    typedef std::map<unsigned int, GAFTextureAtlasElement*> Elements_t;

    GAFTextureAtlas();
private:

    float           m_scale;
    AtlasInfos_t    m_atlasInfos;
    Elements_t      m_elements;

    bool           m_loaded;
    cocos2d::__Array      *  m_images;
    cocos2d::__Array      *  m_textures;
    
    uint32_t        m_memoryConsumption;
public:
    ~GAFTextureAtlas();

    void                    pushAtlasInfo(const AtlasInfo& ai);
    void                    pushElement(unsigned int idx, GAFTextureAtlasElement* el);

    void                    loadImages(const std::string& dir, GAFTextureLoadDelegate_t delegate, cocos2d::ZipFile* bundle = nullptr);

    inline bool     loaded() const
    {
        return m_loaded;
    }

    void          setScale(float val);
    float         getScale() const;

    cocos2d::Image     * image();
    cocos2d::__Array     * images();

    cocos2d::Texture2D * texture();
    cocos2d::__Array     * textures();

    const Elements_t& getElements() const;
	const AtlasInfos_t& getAtlasInfos() const;
    
    //! All CCImages will be freed at the first call of textures()
    //! So there is only VRAM occupation
    //! In case you don't want to release CCImages at textures(), remove releasing ones and
    //! after calling getMemoryConsumptionStat you will need x2 a given number: RAM + VRAM
    uint32_t     getMemoryConsumptionStat() const;
};

NS_GAF_END