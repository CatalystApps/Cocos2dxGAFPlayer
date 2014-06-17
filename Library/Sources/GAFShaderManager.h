#pragma once

#ifndef __GAF_SHADER_MANAGER__
#define __GAF_SHADER_MANAGER__

namespace cocos2d
{
    class CCGLProgram;
}


static const char * kAlphaFragmentShaderFilename = "Shaders/pcShader_PositionTextureAlpha_frag.fs";
static const char * kAlphaFragmentShaderFilename_noCTX = "Shaders/pcShader_PositionTextureAlpha_frag_noCTX.fs";

static const char * kGAFSpriteWithAlphaShaderProgramCache_noCTX = "kGAFSpriteWithAlphaShaderProgramCache_noCTX";
static const char * kGAFSpriteWithAlphaShaderProgramCacheKey = "kGAFSpriteWithAlphaShaderProgramCache";

#define CHECK_CTX_IDENTITY 0

class GAFShaderManager
{
private:
    static GAFShaderManager* m_instance;

public:
    cocos2d::CCGLProgram * createWithFragmentFilename(const char * vertexSource, const char * fragmentFilename, cocos2d::CCGLProgram * p = 0);
    void handleEnterBackground();
    void handleEnterForeground();

    static GAFShaderManager* getInstance();
}; // GAFShaderManager

#endif // __GAF_SHADER_MANAGER__