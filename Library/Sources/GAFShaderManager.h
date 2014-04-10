#pragma once

#ifndef __GAF_SHADER_MANAGER__
#define __GAF_SHADER_MANAGER__

namespace cocos2d
{
    class CCGLProgram;
}

class GAFShaderManager
{
public:
    static CCGLProgram * createWithFragmentFilename(const char * vertexSource, const char * fragmentFilename, CCGLProgram * p = 0);
    static void handleEnterBackground();

}; // GAFShaderManager

#endif // __GAF_SHADER_MANAGER__