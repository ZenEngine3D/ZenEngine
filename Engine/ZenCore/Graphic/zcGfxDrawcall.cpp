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
zEngineRef<Command> CommandDraw::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor)
{
	//! @todo perf switch this to 3x ring buffer with frame lifespan
	static zenMem::zAllocatorPool sMemPool("Pool CommandDraw", sizeof(CommandDraw_HAL), 1024, 1024 );
	auto pDrawcall					= zenNew(&sMemPool) CommandDraw_HAL;		
	pDrawcall->mrRenderPass			= _rRenderPass;
	pDrawcall->mrMeshStrip			= _rMeshStrip;
	pDrawcall->mvScreenScissor		= _vScreenScissor;
	pDrawcall->muIndexFirst			= _rMeshStrip->GetIndexFirst() + _uIndexFirst;
	pDrawcall->muIndexCount			= zenMath::Min(_rMeshStrip->GetIndexCount()-_uIndexFirst, _uIndexCount);
	pDrawcall->SetSortKeyDraw(_rRenderPass, sfCommandCount, _rMeshStrip);
	return pDrawcall;
}

//=================================================================================================
// DRAWCALL CLEAR COLOR
//=================================================================================================
zEngineRef<Command> CommandClearColor::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTColor, const zVec4F& _vRGBA, const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim )
{
	zenAssertMsg( !_rRTColor->IsDepth(), "Trying to clear a depth rendertarget as color.");
	static zenMem::zAllocatorPool sMemPool("Pool CommandClearColor", sizeof(CommandClearColor_HAL), 128, 128 );
	auto pCmdClearColor				= zenNew(&sMemPool) CommandClearColor_HAL;		
	pCmdClearColor->mrRTColor		= _rRTColor;
	pCmdClearColor->mvOrigin		= _vOrigin;
	pCmdClearColor->mvDim			= _vDim;
	pCmdClearColor->mvColor			= _vRGBA;
	pCmdClearColor->mColorMask		= _ColorMask;
	pCmdClearColor->SetSortKeyDataUpdate(_rRTColor.GetResID().GetHashID() );
	return pCmdClearColor;
}

//=================================================================================================
// DRAWCALL CLEAR DEPTH/STENCIL
//=================================================================================================
zEngineRef<Command> CommandClearDepthStencil::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTDepth, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	zenAssertMsg( _rRTDepth->IsDepth(), "Trying to clear a color rendertarget as depth.");
	static zenMem::zAllocatorPool sMemPool("Pool CommandClearDepth", sizeof(CommandClearDepthStencil_HAL), 128, 128 );
	auto pCmdClearDepthStencil					= zenNew(&sMemPool) CommandClearDepthStencil_HAL;		
	pCmdClearDepthStencil->mrRTDepthStencil		= _rRTDepth;
	pCmdClearDepthStencil->mbClearDepth			= _bClearDepth;
	pCmdClearDepthStencil->mfDepthValue			= _fDepthValue;
	pCmdClearDepthStencil->mbClearStencil		= _bClearStencil;
	pCmdClearDepthStencil->muStencilValue		= _uStencilValue;
	pCmdClearDepthStencil->SetSortKeyDataUpdate(_rRTDepth.GetResID().GetHashID());
	return pCmdClearDepthStencil;
}

}
