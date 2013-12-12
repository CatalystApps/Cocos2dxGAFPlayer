#include "GAFShaderManager.h"
#include "GAFData.h"

#include "platform/CCFileUtils.h"
#include "shaders/CCGLProgram.h"


CCGLProgram * GAFShaderManager::createWithFragmentFilename(const char * vertexSource, const char * fragmentFilename, CCGLProgram * p)
{
	if (!vertexSource || !fragmentFilename)
	{
		return NULL;
	}
	std::string fp = CCFileUtils::sharedFileUtils()->fullPathForFilename(fragmentFilename);
	GAFData data;
	data.delete_data = true;
	data.ptr = CCFileUtils::sharedFileUtils()->getFileData(fp.c_str(), "r", &data.size);
	if (!data.ptr)
	{
		CCLOGERROR("Cannot load fragment shader with name %s", fragmentFilename);
		return NULL;
	}
	CCGLProgram * res;
	
	if (p)
	{
		res = p;
	}
	else
	{
		res = new CCGLProgram();
	}
	
	if (!res)
	{
		return NULL;
	}
	if (data.size)
	{
		data.ptr[data.size - 1] = 0;
	}
	
	if (!res->initWithVertexShaderByteArray(vertexSource, (const char *)data.ptr))
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