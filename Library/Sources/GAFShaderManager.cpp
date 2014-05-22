#include "GAFPrecompiled.h"
#include "GAFShaderManager.h"


cocos2d::GLProgram * GAFShaderManager::createWithFragmentFilename(const char * vertexSource, const char * fragmentFilename, cocos2d::GLProgram * p)
{
    if (!vertexSource || !fragmentFilename)
    {
        return NULL;
    }
    std::string fp = cocos2d::FileUtils::getInstance()->fullPathForFilename(fragmentFilename);

    std::string fileContent = cocos2d::FileUtils::getInstance()->getStringFromFile(fp);

    if (fileContent.empty())
    {
        CCLOGERROR("Cannot load fragment shader with name %s", fragmentFilename);
        return NULL;
    }
    cocos2d::GLProgram * res;

    if (p)
    {
        res = p;
    }
    else
    {
        res = new cocos2d::GLProgram();
    }

    if (!res)
    {
        return NULL;
    }
    
    if (!res->initWithByteArrays(vertexSource, fileContent.c_str()))
    {
        CC_SAFE_RELEASE(res);
        return NULL;
    }

    if (!p)
    {
        res->autorelease();
    }

    return res;
}

void GAFShaderManager::handleEnterBackground()
{
    // Stub yet
}
