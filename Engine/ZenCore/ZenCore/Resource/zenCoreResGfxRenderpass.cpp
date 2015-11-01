#include "zcCore.h"

namespace zen { namespace zenRes {

//! @todo Urgent move elsewhere, not a resource per see
zGfxDrawcall::zGfxDrawcall(const zGfxDrawcall& _Copy)
{
	*this = _Copy;
}

zGfxDrawcall::zGfxDrawcall()
{
}

zGfxDrawcall::zGfxDrawcall(zcGfx::Drawcall* _pDrawcall)
{
	*this = _pDrawcall;
}

zGfxDrawcall zGfxDrawcall::DrawMesh(const zGfxRenderPass& _rRenderPass, float _fPriority, const zGfxMeshStrip& _rMeshStrip )
{
	zcRes::GfxRenderPassRef rRenderpass	= _rRenderPass;
	zcRes::GfxMeshStripRef rMeshStrip	= _rMeshStrip;
	zGfxDrawcall rDrawcall				= zcGfx::Drawcall::Create(rRenderpass, _fPriority, rMeshStrip ).GetSafe();	
	return rDrawcall;
}

zGfxDrawcall zGfxDrawcall::ClearColor( const zGfxRenderPass& _rRenderPass, const float& _fPriority, const zGfxRenderTarget& _rRTColor, const zVec4F& _vRGBA, const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim )
{
	zcRes::GfxRenderPassRef rRenderpass	= _rRenderPass;
	zcRes::GfxRenderTargetRef rRTColor	= _rRTColor;
	ZENAssert(!rRTColor->IsDepth());
	zGfxDrawcall rDrawcall = zcGfx::DrawcallClearColor::Create(rRenderpass, _fPriority, rRTColor, _vRGBA, _ColorMask, _vOrigin, _vDim).GetSafe();	
	return rDrawcall;
}

zGfxDrawcall zGfxDrawcall::ClearDepthStencil(const zGfxRenderPass& _rRenderPass, float _fPriority, const zGfxRenderTarget& _rRTDepthStencil, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	zcRes::GfxRenderPassRef rRenderpass			= _rRenderPass;
	zcRes::GfxRenderTargetRef rRTDepthStencil	= _rRTDepthStencil;
	ZENAssert(rRTDepthStencil->IsDepth());
	zGfxDrawcall rDrawcall = zcGfx::DrawcallClearDepthStencil::Create(rRenderpass, _fPriority, rRTDepthStencil, _bClearDepth, _fDepthValue, _bClearStencil, _uStencilValue).GetSafe();	
	return rDrawcall;
}
void zGfxDrawcall::Submit(zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcalls)
{
	zcMgr::GfxRender.Render( _aDrawcalls );
}

zGfxDrawcall& zGfxDrawcall::operator=(const zGfxDrawcall& _Copy)
{	
	Super::operator=(_Copy.Get());
	return *this;
}

zGfxDrawcall& zGfxDrawcall::operator=(zcGfx::Drawcall* _pCopy)
{	
	Super::operator=(_pCopy);
	return *this;
}

//=================================================================================================
// GFX RenderPass
//=================================================================================================
zGfxRenderPass zGfxRenderPass::Create(const zString& _zStageName, zU8 _uPassPriority, const ConfigColorRT& _RTColor, const ConfigDepthRT& _RTDepth, const zGfxStateRasterizer& _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
{
	return zcExp::CreateGfxRenderpass(_zStageName, _uPassPriority, _RTColor, _RTDepth, _rStateRaster, _vDim, _vOrigin);
}

zGfxRenderPass zGfxRenderPass::Create(const zString& _zStageName, zU8 _uPassPriority, const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aRTColors, const ConfigDepthRT& _RTDepth, const zGfxStateRasterizer& _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
{
	return zcExp::CreateGfxRenderpass(_zStageName, _uPassPriority, _aRTColors, _RTDepth, _rStateRaster, _vDim, _vOrigin);
}

}} // namespace zen, zenRes
