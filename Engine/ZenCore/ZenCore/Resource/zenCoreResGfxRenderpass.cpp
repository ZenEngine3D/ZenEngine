#include "zcCore.h"

namespace zen { namespace zenGfx {

zContext zContext::Create(const zStringHash32& _zContextName, const zContext& _rParent, const zenRes::zGfxRenderPass& _rRenderState)
{
	static zenMem::zAllocatorPool sPoolContext("zcGfx::DrawContext", sizeof(zcGfx::DrawContext), 512, 128);	
	zContext rContext;
	rContext = zenNew(&sPoolContext) zcGfx::DrawContext(_zContextName, _rParent.Get(), _rRenderState);
	return rContext;
}

zContext zContext::Create(const zStringHash32& _zContextName, const zenRes::zGfxRenderPass& _rRenderState)
{
	return Create(_zContextName, zContext(), _rRenderState);
}

void zContext::Submit()
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

void zCommand::DrawMesh(const zContext& _rContext, float _fPriority, const zenRes::zGfxMeshStrip& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor )
{
	const zcRes::GfxRenderPassRef& rRenderpass	= _rContext->GetRenderpass(); //! @todo urgent cleanup
	zcRes::GfxMeshStripRef rMeshStrip			= _rMeshStrip;
	ZENAssert(rRenderpass.IsValid());
	ZENAssert(rMeshStrip.IsValid());

	zEngineRef<zcGfx::Command> rCommand			= zcGfx::CommandDraw::Create(rRenderpass, rMeshStrip, _uIndexFirst, _uIndexCount, _vScreenScissor).GetSafe();	
	_rContext->AddCommand(rCommand.Get());
}

void zCommand::DrawMesh(const zContext& _rContext, float _fPriority, const zenRes::zGfxMesh& _rMesh, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor )
{
	const zcRes::GfxRenderPassRef& rRenderpass	= _rContext->GetRenderpass(); //! @todo urgent cleanup
	zcRes::GfxMeshRef rMesh						= _rMesh;
	ZENAssert(rRenderpass.IsValid());
	ZENAssert(rMesh.IsValid());

	auto aMeshStrips = rMesh->GetMeshStrips();
	for(int idx(0), count(aMeshStrips.Count()); idx<count; ++idx)
	{		
		zcRes::GfxMeshStripRef rMeshStrip			= aMeshStrips[idx];
		zEngineRef<zcGfx::Command> rCommand			= zcGfx::CommandDraw::Create(rRenderpass, rMeshStrip, _uIndexFirst, _uIndexCount, _vScreenScissor).GetSafe();	
		_rContext->AddCommand(rCommand.Get());
	}
}

void zCommand::ClearColor(const zContext& _rContext, const zenRes::zGfxRenderTarget& _rRTColor, const zVec4F& _vRGBA, const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim )
{
	const zcRes::GfxRenderPassRef& rRenderpass	= _rContext->GetRenderpass(); //! @todo urgent cleanup
	zcRes::GfxRenderTargetRef rRTColor			= _rRTColor;
	ZENAssert(rRenderpass.IsValid());
	ZENAssert(!rRTColor->IsDepth());
		
	zEngineRef<zcGfx::Command> rCommand			= zcGfx::CommandClearColor::Create(rRenderpass, rRTColor, _vRGBA, _ColorMask, _vOrigin, _vDim);
	_rContext->AddCommand(rCommand);
}

void zCommand::ClearDepthStencil(const zContext& _rContext, const zenRes::zGfxRenderTarget& _rRTDepthStencil, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	const zcRes::GfxRenderPassRef& rRenderpass	= _rContext->GetRenderpass(); //! @todo urgent cleanup
	zcRes::GfxRenderTargetRef rRTDepthStencil	= _rRTDepthStencil;
	ZENAssert(rRenderpass.IsValid());
	ZENAssert(rRTDepthStencil->IsDepth());

	zEngineRef<zcGfx::Command> rCommand			= zcGfx::CommandClearDepthStencil::Create(rRenderpass, rRTDepthStencil, _bClearDepth, _fDepthValue, _bClearStencil, _uStencilValue).GetSafe();	
	_rContext->AddCommand(rCommand);
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
zGfxRenderPass zGfxRenderPass::Create(const zString& _zStageName, zU8 _uPassPriority, const ConfigColorRT& _RTColor, const ConfigDepthRT& _RTDepth, const zGfxStateRasterizer& _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
{
	return zcExp::CreateGfxRenderpass(_zStageName, _uPassPriority, _RTColor, _RTDepth, _rStateRaster, _vDim, _vOrigin);
}

zGfxRenderPass zGfxRenderPass::Create(const zString& _zStageName, zU8 _uPassPriority, const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aRTColors, const ConfigDepthRT& _RTDepth, const zGfxStateRasterizer& _rStateRaster, const zVec2U16& _vDim, const zVec2S16& _vOrigin)
{
	return zcExp::CreateGfxRenderpass(_zStageName, _uPassPriority, _aRTColors, _RTDepth, _rStateRaster, _vDim, _vOrigin);
}


}} // namespace zen, zenRes
