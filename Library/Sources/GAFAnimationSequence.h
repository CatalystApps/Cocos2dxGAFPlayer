#pragma once

#ifndef __GAF_ANIMATION_SEQUENCE_H__
#define __GAF_ANIMATION_SEQUENCE_H__

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

#endif // __GAF_ANIMATION_SEQUENCE_H__
