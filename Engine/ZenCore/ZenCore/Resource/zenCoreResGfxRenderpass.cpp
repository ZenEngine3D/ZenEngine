#include "zcCore.h"

namespace zen { namespace zenGfx {

zScopedDrawlist zScopedDrawlist::Create(const zStringHash32& _zContextName, const zScopedDrawlist& _rParent, const zenRes::zGfxRenderPass& _rRenderState)
{
	static zenMem::zAllocatorPool sPoolContext("zcGfx::DrawlistContext", sizeof(zcGfx::ScopedDrawlist), 512, 128);	
	zScopedDrawlist rContext;
	rContext = zenNew(&sPoolContext) zcGfx::ScopedDrawlist(_zContextName, _rParent.Get(), _rRenderState);
	return rContext;
}

zScopedDrawlist zScopedDrawlist::Create(const zStringHash32& _zContextName, const zenRes::zGfxRenderPass& _rRenderState)
{
	return Create(_zContextName, zScopedDrawlist(), _rRenderState);
}

const zScopedDrawlist& zScopedDrawlist::GetFrameContext()
{
	return zcMgr::GfxRender.GetFrameContext();
}

void zScopedDrawlist::Submit()
{
	GetSafe()->Submit();
}

//! @todo Urgent move elsewhere, not a resource per see
zCommand::zCommand()
{
}

zCommand::zCommand(const zCommand& _Copy)
{
	*this = _Copy;
}

zCommand::zCommand(zcGfx::Command* _pDrawcall)
{
	*this = _pDrawcall;
}

void zCommand::DrawMesh(const zScopedDrawlist& _rContext, float _fPriority, const zenRes::zGfxMeshStrip& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor )
{
	const zcRes::GfxRenderPassRef& rRenderpass	= _rContext->GetRenderpass();
	zenAssert(rRenderpass.IsValid());
	zenAssert(_rMeshStrip.IsValid());
	zcGfx::CommandDraw_HAL::Add(_rContext, rRenderpass, _rMeshStrip, _uIndexFirst, _uIndexCount, _vScreenScissor);	
}

void zCommand::DrawMesh(const zScopedDrawlist& _rContext, float _fPriority, const zenRes::zGfxMesh& _rMesh, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor )
{
	const zcRes::GfxRenderPassRef& rRenderpass	= _rContext->GetRenderpass();
	zenAssert(rRenderpass.IsValid());
	zenAssert(_rMesh.IsValid());

	auto aMeshStrips = _rMesh->GetMeshStrips();
	for(zUInt idx(0), count(aMeshStrips.Count()); idx<count; ++idx)
	{		
		zcRes::GfxMeshStripRef rMeshStrip = aMeshStrips[idx];
		zcGfx::CommandDraw_HAL::Add(_rContext, rRenderpass, rMeshStrip, _uIndexFirst, _uIndexCount, _vScreenScissor);			
	}
}

void zCommand::ClearColor(const zScopedDrawlist& _rContext, const zenRes::zGfxTarget2D& _rRTColor, const zVec4F& _vRGBA, const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim )
{
	const zcRes::GfxRenderPassRef& rRenderpass	= _rContext->GetRenderpass();
	zenAssert(rRenderpass.IsValid());
	zenAssert(!_rRTColor->IsDepth());		
	zcGfx::CommandClearColor_HAL::Add(_rContext, rRenderpass, _rRTColor, _vRGBA, _ColorMask, _vOrigin, _vDim);
}

void zCommand::ClearDepthStencil(const zScopedDrawlist& _rContext, const zenRes::zGfxTarget2D& _rRTDepthStencil, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	const zcRes::GfxRenderPassRef& rRenderpass	= _rContext->GetRenderpass();
	zenAssert(rRenderpass.IsValid() && _rRTDepthStencil.IsValid());
	zenAssert(_rRTDepthStencil->IsDepth());
	zcGfx::CommandClearDepthStencil_HAL::Add(_rContext, rRenderpass, _rRTDepthStencil, _bClearDepth, _fDepthValue, _bClearStencil, _uStencilValue).GetSafe();		
}

zCommand& zCommand::operator=(const zCommand& _Copy)
{	
	Super::operator=(_Copy.Get());
	return *this;
}

zCommand& zCommand::operator=(zcGfx::Command* _pCopy)
{	
	Super::operator=(_pCopy);
	return *this;
}

bool zCommand::operator>(const zenGfx::zCommand& _Cmp)const
{
	return	(Get()->mSortId.mSortKeyHi > _Cmp->mSortId.mSortKeyHi) ||
			(Get()->mSortId.mSortKeyHi == _Cmp->mSortId.mSortKeyHi && Get()->mSortId.mSortKeyLo > _Cmp->mSortId.mSortKeyLo);
}

bool zCommand::operator>=(const zenGfx::zCommand& _Cmp)const
{
	return	(Get()->mSortId.mSortKeyHi > _Cmp->mSortId.mSortKeyHi) ||
			(Get()->mSortId.mSortKeyHi == _Cmp->mSortId.mSortKeyHi && Get()->mSortId.mSortKeyLo >= _Cmp->mSortId.mSortKeyLo);
}

}} // namespace zen, zenGfx

namespace zen { namespace zenRes {	

//=================================================================================================
// GFX RenderPass
//=================================================================================================
const zGfxTarget2D& zGfxRenderPass::GetColorTarget(zUInt _uIndex)const
{
	return Get()->mrStateView->GetColorTarget(_uIndex);
}
const zGfxTarget2D&	zGfxRenderPass::GetDepthTarget()const
{
	return Get()->mrStateView->GetDepthTarget();
}

zGfxRenderPass zGfxRenderPass::Create(const zString& _zStageName, zU8 _uPassPriority, const ConfigColorRT& _RTColor, const ConfigDepthRT& _RTDepth, const zGfxStateRaster& _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
{
	return zcExp::CreateGfxRenderpass(_zStageName, _uPassPriority, _RTColor, _RTDepth, _rStateRaster, _vDim, _vOrigin);
}

zGfxRenderPass zGfxRenderPass::Create(const zString& _zStageName, zU8 _uPassPriority, const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aRTColors, const ConfigDepthRT& _RTDepth, const zGfxStateRaster& _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
{
	return zcExp::CreateGfxRenderpass(_zStageName, _uPassPriority, _aRTColors, _RTDepth, _rStateRaster, _vDim, _vOrigin);
}


}} // namespace zen, zenRes
