#include "zcCore.h"

namespace zcGfx
{

zEngineRef<Drawcall> Drawcall::Create( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip, const zVec4U16& _vScreenScissor)
{
	//! @todo switch this to 3x ring buffer with frame lifespan
	static zenMem::zAllocatorPool sMemPool("Pool Drawcall", sizeof(Drawcall), 1024, 1024 );
	Drawcall* pDrawcall						= zenNew(&sMemPool) Drawcall;	
	pDrawcall->ConfigureBase( _rRenderPass, _fPriority, _rMeshStrip);
	pDrawcall->mrRenderPass					= _rRenderPass;
	pDrawcall->mrMeshStrip					= _rMeshStrip;
	pDrawcall->mvScreenScissor				= _vScreenScissor;
	return pDrawcall;
}

zEngineRef<Drawcall> DrawcallClearColor::Create( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxRenderTargetRef& _rRTColor, const zVec4F& _vRGBA, const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim )
{
	static zenMem::zAllocatorPool sMemPool("Pool Drawcall Clear Color", sizeof(DrawcallClearColor), 128, 128 );
	DrawcallClearColor* pDrawcallClearColor	= zenNew(&sMemPool) DrawcallClearColor;	
	pDrawcallClearColor->ConfigureBase( _rRenderPass, _fPriority, 0, keGpuPipe_PreDrawCommand);
	pDrawcallClearColor->mrRenderPass				= _rRenderPass;
	pDrawcallClearColor->mrMeshStrip				= nullptr;
	pDrawcallClearColor->mrRTColor					= _rRTColor;
	pDrawcallClearColor->mvOrigin					= _vOrigin;
	pDrawcallClearColor->mvDim						= _vDim;
	pDrawcallClearColor->mvColor					= _vRGBA;
	pDrawcallClearColor->mColorMask					= _ColorMask;
	return pDrawcallClearColor;
}

void DrawcallClearColor::Invoke()
{	
	mrRTColor->Clear(mvColor); //! @todo Urgent Fully implement
}

zEngineRef<Drawcall> DrawcallClearDepthStencil::Create( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxRenderTargetRef& _rRTDepth, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	static zenMem::zAllocatorPool sMemPool("Pool Drawcall Clear Color", sizeof(DrawcallClearDepthStencil), 128, 128 );
	DrawcallClearDepthStencil* pDrawcallClearDepthStencil	= zenNew(&sMemPool) DrawcallClearDepthStencil;	
	pDrawcallClearDepthStencil->ConfigureBase( _rRenderPass, _fPriority, 0, keGpuPipe_PreDrawCommand);
	pDrawcallClearDepthStencil->mrRTDepthStencil	= _rRTDepth;
	pDrawcallClearDepthStencil->mbClearDepth		= _bClearDepth;
	pDrawcallClearDepthStencil->mfDepthValue		= _fDepthValue;
	pDrawcallClearDepthStencil->mbClearStencil		= _bClearStencil;
	pDrawcallClearDepthStencil->muStencilValue		= _uStencilValue;
	return pDrawcallClearDepthStencil;
}

void DrawcallClearDepthStencil::Invoke()
{
	mrRTDepthStencil->Clear(mfDepthValue, muStencilValue, mbClearDepth, mbClearStencil);
}

}
