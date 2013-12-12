#include "GAFFilterData.h"

GAFBlurFilterData * GAFBlurFilterData::create(float bX, float bY)
{
	GAFBlurFilterData * ret = new GAFBlurFilterData();
	if (ret)
	{
		ret->blurSize.width = bX;
		ret->blurSize.height = bY;
		ret->autorelease();
		return ret;
	}
	return NULL;
}

GAFBlurFilterData::GAFBlurFilterData()
{
	
}
