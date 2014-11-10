#pragma once

#include "GAFObject.h"
#include "GAFTextData.h"

NS_GAF_BEGIN

class GAFTextField : public GAFObject
{


public:
    cocos2d::Label *m_label;

    GAFTextField();
    virtual ~GAFTextField();

    void initWithTextData(const GAFTextData *data);

};

NS_GAF_END