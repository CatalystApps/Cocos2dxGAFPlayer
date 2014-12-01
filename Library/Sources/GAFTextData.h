#pragma once

NS_GAF_BEGIN

class GAFTextData
{
    friend class TagDefineTextField;

public:
    class TextFormat
    {
    public:
        enum class TextAlign : uint32_t
        {
            Left = 0,
            Right,
            Center,
            Justify,
            Start,
            End            
        };

        TextAlign m_align;
        cocos2d::TextHAlignment getTextAlignForCocos() const;

        float m_letterSpacing; // !! unused

        cocos2d::Color4B m_color;

        uint32_t m_blockIndent; // !! unused
        uint32_t m_indent; // !! unused
        uint32_t m_leading; // !! unused
        uint32_t m_leftMargin; // !! unused
        uint32_t m_rightMargin; // !! unused
        uint32_t m_size;

        std::vector<uint32_t> m_tabStops; // !! unused

        bool m_isBold; // !! unused
        bool m_isItalic; // !! unused
        bool m_isUnderline; // !! unused
        bool m_isBullet; // !! unused
        bool m_useKerning; // !! unused

        std::string m_font;
        std::string m_target; // !! unused
        std::string m_url; // !! unused
    };

    cocos2d::Point m_pivot;

    float m_width;
    float m_height;

    std::string m_text;
    std::string m_restrict; // !! unused

    bool m_isEmbedFonts; // !! unused
    bool m_isMultiline; // !! unused
    bool m_isWordWrap; // !! unused
    bool m_hasRestrict; // !! unused
    bool m_isEditable; // !! unused
    bool m_isSelectable; // !! unused
    bool m_displayAsPassword; // !! unused

    uint32_t m_maxChars; // !! unused

    TextFormat m_textFormat;

};

NS_GAF_END