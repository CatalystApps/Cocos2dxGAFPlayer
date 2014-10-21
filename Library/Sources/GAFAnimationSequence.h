#pragma once

NS_GAF_BEGIN

class GAFAnimationSequence
{
public:
    std::string name;
    int startFrameNo;
    int endFrameNo;
    inline int length() const
    {
        return endFrameNo - startFrameNo;
    }
};

NS_GAF_END
