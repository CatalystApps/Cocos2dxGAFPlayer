#include "GAFPrecompiled.h"
#include "TagDefineTextField.h"

#include "GAFAsset.h"
#include "GAFStream.h"
#include "GAFTimeline.h"
#include "GAFTextData.h"
#include "PrimitiveDeserializer.h"

NS_GAF_BEGIN

void TagDefineTextField::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    (void)asset;
    unsigned int count = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int objectIdRef = in->readU32();

        GAFTextData* textData = new GAFTextData();

        PrimitiveDeserializer::deserialize(in, &textData->m_pivot);
        textData->m_width = in->readFloat();
        textData->m_height = in->readFloat();
        in->readString(&textData->m_text);
        textData->m_isEmbedFonts = in->readUByte() ? true : false;
        textData->m_isMultiline = in->readUByte() ? true : false;
        textData->m_isWordWrap = in->readUByte() ? true : false;
        textData->m_hasRestrict = in->readUByte() ? true : false;
        if (textData->m_hasRestrict)
        {
            in->readString(&textData->m_restrict);
        }
        textData->m_isEditable = in->readUByte() ? true : false;
        textData->m_isSelectable = in->readUByte() ? true : false;
        textData->m_displayAsPassword = in->readUByte() ? true : false;
        textData->m_maxChars = in->readU32();
        
        // Text Format
        {
            GAFTextData::TextFormat format;
            uint32_t align = in->readU32();
            format.m_align = static_cast<GAFTextData::TextFormat::TextAlign>(align);
            format.m_blockIndent = in->readU32();
            format.m_isBold = in->readUByte() ? true : false;
            format.m_isBullet = in->readUByte() ? true : false;

            unsigned int clr = in->readU32();
            PrimitiveDeserializer::translateColor(format.m_color, clr);

            in->readString(&format.m_font);
            format.m_indent = in->readU32();
            format.m_isItalic = in->readUByte() ? true : false;
            format.m_useKerning = in->readUByte() ? true : false;
            format.m_leading = in->readU32();
            format.m_leftMargin = in->readU32();
            format.m_letterSpacing = in->readFloat();
            format.m_rightMargin = in->readU32();
            format.m_size = in->readU32();

            uint32_t tabsCount = in->readU32();
            for (uint32_t j = 0; j < tabsCount; ++j)
            {
                uint32_t tabValue = in->readU32();
                format.m_tabStops.push_back(tabValue);
            }
            
            in->readString(&format.m_target);
            format.m_isUnderline = in->readUByte() ? true : false;
            in->readString(&format.m_url);

            textData->m_textFormat = format;
        }

        timeline->pushTextData(objectIdRef, textData);
    }
}

NS_GAF_END