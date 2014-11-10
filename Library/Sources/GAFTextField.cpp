#include "GAFPrecompiled.h"
#include "GAFTextField.h"

NS_GAF_BEGIN

GAFTextField::GAFTextField()
{
    m_charType = GAFCharacterType::TextField;
}

GAFTextField::~GAFTextField()
{

}

// TODO: Factory
void GAFTextField::initWithTextData(GAFTextData const* data)
{
    m_label = cocos2d::Label::create(data->m_text, data->m_textFormat.m_font, data->m_textFormat.m_size);
    
    m_label->setDimensions(data->m_width, data->m_height);
    //m_label->setTextColor(data->m_textFormat.m_color); // NOT WORKING
    m_label->setColor(cocos2d::Color3B(data->m_textFormat.m_color));
    m_label->setHorizontalAlignment(data->m_textFormat.getTextAlignForCocos());


    addChild(m_label);
}

NS_GAF_END