#pragma once

#ifndef __GAF_SHADER_MANAGER__
#define __GAF_SHADER_MANAGER__

class GAFShaderManager
{
public:
    static cocos2d::GLProgram * createWithFragmentFilename(const char * vertexSource, const char * fragmentFilename, cocos2d::GLProgram * p = 0);
    static void handleEnterBackground();

}; // GAFShaderManager

#endif // __GAF_SHADER_MANAGER__