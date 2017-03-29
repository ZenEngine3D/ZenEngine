#include "zcCore.h"

namespace zcGfx
{

float Command::sfCommandCount = 0.f;

Command::Command()
{ 
	zenStaticAssert( sizeof(RenderStateSortID::Draw)		== sizeof(zU64)*2 );	//Error when setting bitfield, must match size of mSortKeyLo + mSortKeyHi
	zenStaticAssert( sizeof(RenderStateSortID::DataUpdate)	== sizeof(zU64)*2 );	
	zenStaticAssert( sizeof(RenderStateSortID::Compute)		== sizeof(zU64)*2 );	
	++sfCommandCount; 
}

void Command::ResetCommandCount()
{
	sfCommandCount = 0.f;
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command> CommandDraw::Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor)
{
	//! @todo perf switch this to 3x ring buffer with frame lifespan
	static zenMem::zAllocatorPool sMemPool("Pool CommandDraw", sizeof(CommandDraw_HAL), 1024, 1024 );
	auto pCommand					= zenNew(&sMemPool) CommandDraw_HAL;		
	pCommand->mrRenderPass			= _rRenderPass;
	pCommand->mrMeshStrip			= _rMeshStrip;
	pCommand->mvScreenScissor		= _vScreenScissor;
	pCommand->muIndexFirst			= _rMeshStrip->GetIndexFirst() + _uIndexFirst;
	pCommand->muIndexCount			= zenMath::Min(_rMeshStrip->GetIndexCount()-_uIndexFirst, _uIndexCount);
	pCommand->SetSortKeyDraw(_rRenderPass, sfCommandCount, _rMeshStrip);
	_rContext->AddCommand(pCommand);
	return pCommand;
}

//=================================================================================================
// DRAWCALL CLEAR COLOR
//=================================================================================================
zEngineRef<Command> CommandClearColor::Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTColor, const zVec4F& _vRGBA, const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim )
{
	zenAssertMsg( !_rRTColor->IsDepth(), "Trying to clear a depth rendertarget as color.");
	static zenMem::zAllocatorPool sMemPool("Pool CommandClearColor", sizeof(CommandClearColor_HAL), 128, 128 );
	auto pCommand					= zenNew(&sMemPool) CommandClearColor_HAL;		
	pCommand->mrRTColor				= _rRTColor;
	pCommand->mvOrigin				= _vOrigin;
	pCommand->mvDim					= _vDim;
	pCommand->mvColor				= _vRGBA;
	pCommand->mColorMask			= _ColorMask;
	pCommand->SetSortKeyDataUpdate(_rRTColor.GetResID().GetHashID() );
	_rContext->AddCommand(pCommand);
	return pCommand;
}

//=================================================================================================
// DRAWCALL CLEAR DEPTH/STENCIL
//=================================================================================================
zEngineRef<Command> CommandClearDepthStencil::Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTDepth, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	zenAssertMsg( _rRTDepth->IsDepth(), "Trying to clear a color rendertarget as depth.");
	static zenMem::zAllocatorPool sMemPool("Pool CommandClearDepth", sizeof(CommandClearDepthStencil_HAL), 128, 128 );
	auto pCommand					= zenNew(&sMemPool) CommandClearDepthStencil_HAL;		
	pCommand->mrRTDepthStencil		= _rRTDepth;
	pCommand->mbClearDepth			= _bClearDepth;
	pCommand->mfDepthValue			= _fDepthValue;
	pCommand->mbClearStencil		= _bClearStencil;
	pCommand->muStencilValue		= _uStencilValue;
	pCommand->SetSortKeyDataUpdate(_rRTDepth.GetResID().GetHashID());
	_rContext->AddCommand(pCommand);
	return pCommand;
}

}
