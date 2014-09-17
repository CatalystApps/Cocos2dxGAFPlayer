#pragma once

class GAFAnimationSequence
{
public:
    std::string name;
    uint32_t startFrameNo;
    uint32_t endFrameNo;
    inline uint32_t length() const
    {
        assert(endFrameNo > startFrameNo);
        return endFrameNo - startFrameNo;
    }
};
