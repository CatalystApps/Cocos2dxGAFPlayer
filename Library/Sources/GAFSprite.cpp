#include "sprite_nodes/CCSprite.h"
#include "GAFSprite.h"

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__A__) ( (int)(__A__))
#endif
#include "GAFSpriteWithAlpha.h"
#include "shaders/CCShaderCache.h"
#include "sprite_nodes/CCSpriteBatchNode.h"

GAFSprite::GAFSprite()
:
_useSeparateBlendFunc(false),
_isLocator(false),
_blendEquation(-1),
_externalTransform(CCAffineTransformIdentity),
_childTransform(CCAffineTransformIdentity),
_atlasScale(1.0f)
{
	
}

void GAFSprite::setExternaTransform(const CCAffineTransform& transform)
{
	if (!CCAffineTransformEqualToTransform(_externalTransform, transform))
	{
		_externalTransform = transform;
		m_bTransformDirty = true;
		m_bInverseDirty = true;
	}
}

void GAFSprite::setAtlasScale(float scale)
{
	if (_atlasScale != scale)
	{
		_atlasScale = scale;
		m_bTransformDirty = true;
		m_bInverseDirty = true;
	}
}

CCAffineTransform GAFSprite::nodeToParentTransform(void)
{
	if (m_bTransformDirty)
	{
		CCAffineTransform t = _externalTransform;
		if (_atlasScale != 1.0f)
		{
			t = CCAffineTransformScale(t, _atlasScale, _atlasScale);
		}
		m_sTransform = CCAffineTransformTranslate(t, -m_obAnchorPointInPoints.x, -m_obAnchorPointInPoints.y);
		m_bTransformDirty = false;
	}
	return m_sTransform;		
}

void GAFSprite::draw(void)
{
	if (_isLocator)
	{
		return;
	}
	
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "GAFSprite - draw");
	
    CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
	
    CC_NODE_DRAW_SETUP();
	
    if (_useSeparateBlendFunc)
    {
        glBlendFuncSeparate(_blendFuncSeparate.src,      _blendFuncSeparate.dst,
                            _blendFuncSeparate.srcAlpha, _blendFuncSeparate.dstAlpha);
    }
    else
    {
        ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    }
    
    if (_blendEquation != -1)
    {
        glBlendEquation(_blendEquation);
    }
	
    if (m_pobTexture != NULL)
    {
        ccGLBindTexture2D( m_pobTexture->getName() );
    }
    else
    {
        ccGLBindTexture2D(0);
    }
    
    //
    // Attributes
    //
	
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
	setUniformsForFragmentShader();
	CHECK_GL_ERROR_DEBUG();
	
#define kQuadSize sizeof(m_sQuad.bl)
    long offset = (long)&m_sQuad;
	
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
	
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
	
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
    CHECK_GL_ERROR_DEBUG();	

    CC_INCREMENT_GL_DRAWS(1);
	
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "GAFSprite - draw");
}

void GAFSprite::invalidateTransformCache()
{
	m_bTransformDirty = true;
}

void GAFSprite::setUniformsForFragmentShader()
{
	
}

bool GAFSprite::initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated)
{
	if (CCSprite::initWithTexture(pTexture, rect, rotated))
	{
		setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
		return true;
	}
	else
	{
		return false;
	}
}

void GAFSprite::setTexture(CCTexture2D *texture)
{
    // If batchnode, then texture id should be the same
    CCAssert(! m_pobBatchNode || texture->getName() == m_pobBatchNode->getTexture()->getName(), "CCSprite: Batched sprites should use the same texture as the batchnode");
    // accept texture==nil as argument
    CCAssert( !texture || dynamic_cast<CCTexture2D*>(texture), "setTexture expects a CCTexture2D. Invalid argument");
    
    if (!m_pobBatchNode && m_pobTexture != texture)
    {
        CC_SAFE_RETAIN(texture);
        CC_SAFE_RELEASE(m_pobTexture);
        m_pobTexture = texture;
        updateBlendFunc();
    }
}