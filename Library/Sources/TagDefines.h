#pragma once

NS_GAF_BEGIN

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
        TagDefineTextFields = 7,
        TagDefineAtlas2 = 8,
        TagDefineStage = 9,
        TagDefineAnimationObjects2 = 10,
        TagDefineAnimationMasks2 = 11,
        TagDefineAnimationFrames2 = 12,
        TagDefineTimeline = 13
    };

    static std::string toString(Enum e)
    {
        switch (e)
        {
        case Tags::TagEnd:
            return "TagEnd";

        case Tags::TagDefineAtlas:
            return "TagDefineAtlas";

        case Tags::TagDefineAnimationMasks:
            return "TagDefineAnimationMasks";

        case Tags::TagDefineAnimationObjects:
            return "TagDefineAnimationObjects";

        case Tags::TagDefineAnimationFrames:
            return "TagDefineAnimationFrames";

        case Tags::TagDefineNamedParts:
            return "TagDefineNamedParts";

        case Tags::TagDefineSequences:
            return "TagDefineSequences";

        case Tags::TagDefineStage:
            return "TagDefineStage";

        case Tags::TagDefineAtlas2:
            return "TagDefineAtlas2";

        case Tags::TagDefineTimeline:
            return "TagDefineTimeline";

        case Tags::TagDefineTextFields:
            return "TagDefineTextFields";

        case Tags::TagDefineAnimationMasks2:
            return "TagDefineAnimationMasks2";

        case Tags::TagDefineAnimationObjects2:
            return "TagDefineAnimationObjects2";

        case Tags::TagDefineAnimationFrames2:
            return "TagDefineAnimationFrames2";

        default:
            return "This tag is not yet implemented";
        }
    }
};

NS_GAF_END