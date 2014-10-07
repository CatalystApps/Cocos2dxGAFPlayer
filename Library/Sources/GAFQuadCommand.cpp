#include "GAFPrecompiled.h"
#include "GAFQuadCommand.h"

USING_NS_CC;

namespace gaf
{

GAFQuadCommand::GAFQuadCommand()
{}

GAFQuadCommand::~GAFQuadCommand()
{}

void GAFQuadCommand::init(float globalOrder, GLuint textureID, GLProgramState* glProgramState, BlendFunc blendType, V3F_C4B_T2F_Quad* quad, ssize_t quadCount, const Mat4 &mv, uint32_t materialId)
{
    CCASSERT(glProgramState, "Invalid GLProgramState");
    CCASSERT(glProgramState->getVertexAttribsFlags() == 0, "No custom attributes are supported in QuadCommand");

    _globalOrder = globalOrder;
    _quadsCount = quadCount;
    _quads = quad;
    _mv = mv;

    if (_textureID != textureID || _blendType.src != blendType.src || _blendType.dst != blendType.dst || _glProgramState != glProgramState || _materialID != materialId) 
    {
        _textureID = textureID;
        _blendType = blendType;
        _glProgramState = glProgramState;
        _materialID = materialId;
    }
}

}