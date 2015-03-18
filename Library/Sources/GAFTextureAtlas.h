#pragma once

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
public:
    ~GAFTextureAtlas();

    void    pushAtlasInfo(const AtlasInfo& ai);
    void    pushElement(unsigned int idx, GAFTextureAtlasElement* el);

    void    setScale(float val);
    float   getScale() const;

    const Elements_t& getElements() const;
    const AtlasInfos_t& getAtlasInfos() const;
};

NS_GAF_END