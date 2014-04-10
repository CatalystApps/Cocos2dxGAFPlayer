#pragma once

struct Tags
{
    enum Enum
    {
        TagEnd = 0,
        TagDefineAtlas = 1,
        TagDefineAnimationMasks = 2,
        TagDefineAnimationObjects = 3,
        TagDefineAnimationFrames = 4,
        TagDefineNamedParts = 5,
        TagDefineSequences = 6,
    };

    static std::string toString(Enum e)
    {
        switch (e)
        {
        case Tags::TagEnd:
            return "TagEnd";
            break;
        case Tags::TagDefineAtlas:
            return "TagDefineAtlas";
            break;
        case Tags::TagDefineAnimationMasks:
            return "TagDefineAnimationMasks";
            break;
        case Tags::TagDefineAnimationObjects:
            return "TagDefineAnimationObjects";
            break;
        case Tags::TagDefineAnimationFrames:
            return "TagDefineAnimationFrames";
            break;
        case Tags::TagDefineNamedParts:
            return "TagDefineNamedParts";
            break;
        case Tags::TagDefineSequences:
            return "TagDefineSequences";
            break;
        default:
            return "";
            assert(false);
        }
    }
};