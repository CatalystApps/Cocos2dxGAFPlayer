#pragma once

NS_GAF_BEGIN

/** Command used to render one or more Quads */
class GAFQuadCommand : public cocos2d::QuadCommand
{
public:
    GAFQuadCommand();
    ~GAFQuadCommand();

    void init(float globalOrder, GLuint texutreID, cocos2d::GLProgramState* shader, cocos2d::BlendFunc blendType, cocos2d::V3F_C4B_T2F_Quad* quads, ssize_t quadCount,
        const cocos2d::Mat4& mv, uint32_t materialId);
};

NS_GAF_END