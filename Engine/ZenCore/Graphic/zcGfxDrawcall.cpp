#include "zcCore.h"

namespace zcGfx
{

float Command::sfCommandCount = 0.f;

Command::Command()
{ 
	zenStaticAssert( sizeof(RenderStateSortID::Draw)	== sizeof(zU64)*2 );	//Error when setting bitfield, must match size of mSortKeyLo + mSortKeyHi
	zenStaticAssert( sizeof(RenderStateSortID::Generic)	== sizeof(zU64)*2 );	
	zenStaticAssert( sizeof(RenderStateSortID::Compute)	== sizeof(zU64)*2 );	
	++sfCommandCount; 
}

void Command::ResetCommandsize()
{
	sfCommandCount = 0.f;
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command> CommandDraw::Add(const CommandListRef& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor)
{
	//! @todo perf switch this to 3x ring buffer with frame lifespan
	auto pCommand				= zenMem::NewPool<CommandDraw_HAL>();
	pCommand->mrRenderPass		= _rRenderPass;
	pCommand->mrMeshStrip		= _rMeshStrip;
	pCommand->mvScreenScissor	= _vScreenScissor;
	pCommand->muIndexFirst		= _rMeshStrip->GetIndexFirst() + _uIndexFirst;
	pCommand->muIndexCount		= zenMath::Min(_rMeshStrip->GetIndexsize()-_uIndexFirst, _uIndexCount);
	pCommand->SetSortKeyDraw(_rRenderPass, sfCommandCount, _rMeshStrip);
	_rContext->AddCommand(pCommand);
	return pCommand;
}

//=================================================================================================
// DRAWCALL CLEAR COLOR
//=================================================================================================
zEngineRef<Command> CommandClearColor::Add(const CommandListRef& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTColor, const zVec4F& _vRGBA, const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim )
{
	zenAssertMsg( !_rRTColor->IsDepth(), "Trying to clear a depth rendertarget as color.");
	auto pCommand				= zenMem::NewPool<CommandClearColor_HAL>();
	pCommand->mrRTColor			= _rRTColor;
	pCommand->mvOrigin			= _vOrigin;
	pCommand->mvDim				= _vDim;
	pCommand->mvColor			= _vRGBA;
	pCommand->mColorMask		= _ColorMask;
	pCommand->SetSortKeyGeneric(keGpuPipe_DataUpdate, _rRTColor.GetResID().GetHashID() );
	_rContext->AddCommand(pCommand);
	return pCommand;
}

//=================================================================================================
// DRAWCALL CLEAR DEPTH/STENCIL
//=================================================================================================
zEngineRef<Command> CommandClearDepthStencil::Add(const CommandListRef& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTDepth, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	zenAssertMsg( _rRTDepth->IsDepth(), "Trying to clear a color rendertarget as depth.");
	auto pCommand				= zenMem::NewPool<CommandClearDepthStencil_HAL>();
	pCommand->mrRTDepthStencil	= _rRTDepth;
	pCommand->mbClearDepth		= _bClearDepth;
	pCommand->mfDepthValue		= _fDepthValue;
	pCommand->mbClearStencil	= _bClearStencil;
	pCommand->muStencilValue	= _uStencilValue;
	pCommand->SetSortKeyGeneric(keGpuPipe_DataUpdate, _rRTDepth.GetResID().GetHashID());
	_rContext->AddCommand(pCommand);
	return pCommand;
}

//=================================================================================================
// DRAWCALL GPU SCOPED EVENT
//=================================================================================================
zEngineRef<Command> CommandGPUScopedEvent::Add(const CommandListRef& _rContext, const char* _zEventName, EnumEventInfo _eEventInfo, const zVec4F& _vColor, bool _bCopyEventName)
{	
	auto pCommand			= zenMem::NewPool<CommandGPUScopedEvent_HAL>();
	pCommand->mzEventName	= _zEventName;
	pCommand->meEventInfo	= _eEventInfo;
	pCommand->mvColor		= _vColor;
	if( _bCopyEventName )
	{
		pCommand->mzEventNameCopy	= _zEventName;
		pCommand->mzEventName		= &pCommand->mzEventNameCopy[0];
	}
	
	pCommand->SetSortKeyGeneric(_eEventInfo == keEventStart ? keGpuPipe_First : keGpuPipe_Last, 0);
	_rContext->AddCommand(pCommand);
	return pCommand;
}



}
