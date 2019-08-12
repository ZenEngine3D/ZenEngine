#pragma once 

namespace zcGfx
{

class GPUContext_Base
{
zenClassDeclareNoParent(GPUContext_Base);
public:
											GPUContext_Base();
											void Reset();
protected:
	zcRes::GfxRenderPassRef					mrRenderpass		= nullptr;
	zcRes::GfxViewRef						mrStateView			= nullptr;
	zcRes::GfxStateBlendRef					mrStateBlend		= nullptr;
	zcRes::GfxStateDepthStencilRef			mrStateDepthStencil	= nullptr;
	zcRes::GfxStateRasterRef				mrStateRaster		= nullptr;
};


} //namespace zcGfx

#include zenHeaderRenderer(zcGfxGPUContext)

namespace zcGfx
{

class GPUContext : public GPUContext_HAL
{
zenClassDeclare(GPUContext, GPUContext_HAL);
public:
	void Submit(const zArrayDyn<zEngineRef<zcGfx::Command>>& _aDrawcalls); //! @todo 1 remove this, collapse in RenderManager::Render
};

}

