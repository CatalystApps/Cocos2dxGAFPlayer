#pragma once

namespace gaf
{

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

}