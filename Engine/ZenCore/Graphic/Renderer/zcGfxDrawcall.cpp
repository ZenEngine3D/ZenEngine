#include "zcCore.h"

namespace zcGfx
{

float Command::sfCommandCount = 0.f;

Command::Command()
{ 
	++sfCommandCount; 
}

void Command::ResetCommandCount()
{
	sfCommandCount = 0.f;
}

//=================================================================================================
// DRAWCALL
//=================================================================================================
zEngineRef<Command> CommandDraw::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor)
{
	//! @todo perf switch this to 3x ring buffer with frame lifespan
	static zenMem::zAllocatorPool sMemPool("Pool CommandDraw", sizeof(CommandDraw), 1024, 1024 );
	CommandDraw* pDrawcall			= zenNew(&sMemPool) CommandDraw;	
	pDrawcall->ConfigureBase( _rRenderPass, sfCommandCount, _rMeshStrip);
	pDrawcall->mbIsCommandDraw		= true;
	pDrawcall->mrRenderPass			= _rRenderPass;
	pDrawcall->mrMeshStrip			= _rMeshStrip;
	pDrawcall->mvScreenScissor		= _vScreenScissor;
	pDrawcall->muIndexFirst			= _rMeshStrip->GetProxy()->muIndexFirst + _uIndexFirst;
	pDrawcall->muIndexCount			= zenMath::Min(_rMeshStrip->GetProxy()->muIndexCount-_uIndexFirst, _uIndexCount);
	return pDrawcall;
}

//=================================================================================================
// DRAWCALL CLEAR COLOR
//=================================================================================================
zEngineRef<Command> CommandClearColor::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxRenderTargetRef& _rRTColor, const zVec4F& _vRGBA, const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim )
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandClearColor", sizeof(CommandClearColor), 128, 128 );
	CommandClearColor* pCmdClearColor		= zenNew(&sMemPool) CommandClearColor;	
	pCmdClearColor->ConfigureBase( _rRenderPass, sfCommandCount, 0, keGpuPipe_PreDrawCommand);
	pCmdClearColor->mrRenderPass	= _rRenderPass;
	pCmdClearColor->mrRTColor		= _rRTColor;
	pCmdClearColor->mvOrigin		= _vOrigin;
	pCmdClearColor->mvDim			= _vDim;
	pCmdClearColor->mvColor			= _vRGBA;
	pCmdClearColor->mColorMask		= _ColorMask;
	return pCmdClearColor;
}

void CommandClearColor::Invoke()
{	
	mrRTColor->Clear(mvColor); //! @todo Urgent Fully implement for all RTs
}

//=================================================================================================
// DRAWCALL CLEAR DEPTH/STENCIL
//=================================================================================================
zEngineRef<Command> CommandClearDepthStencil::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxRenderTargetRef& _rRTDepth, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandClearDepth", sizeof(CommandClearDepthStencil), 128, 128 );
	CommandClearDepthStencil* pCmdClearDepthStencil	= zenNew(&sMemPool) CommandClearDepthStencil;	
	pCmdClearDepthStencil->ConfigureBase( _rRenderPass, sfCommandCount, 0, keGpuPipe_PreDrawCommand);
	pCmdClearDepthStencil->mrRTDepthStencil	= _rRTDepth;
	pCmdClearDepthStencil->mbClearDepth		= _bClearDepth;
	pCmdClearDepthStencil->mfDepthValue		= _fDepthValue;
	pCmdClearDepthStencil->mbClearStencil	= _bClearStencil;
	pCmdClearDepthStencil->muStencilValue	= _uStencilValue;
	return pCmdClearDepthStencil;
}

void CommandClearDepthStencil::Invoke()
{
	mrRTDepthStencil->Clear(mfDepthValue, muStencilValue, mbClearDepth, mbClearStencil);
}


//=================================================================================================
// DRAWCALL EVENT START
//=================================================================================================
zEngineRef<Command> CommandEventStart::Create(const zcRes::GfxRenderPassRef& _rRenderPass/*, const zenPerf::zEventRef& _rEventGPU*/)
{
//	ZENAssert(_rEventGPU.IsValid());
	static zenMem::zAllocatorPool sMemPool("Pool CommandEventStart", sizeof(CommandEventStart), 128, 128);
	CommandEventStart* pCmdEventStart = zenNew(&sMemPool) CommandEventStart;
	pCmdEventStart->ConfigureBase(_rRenderPass, -999999.f, 0, keGpuPipe_PreDrawCommand);
//	pDrawcallEventStart->mrEventGPU = _rEventGPU;
	return pCmdEventStart;
}

void CommandEventStart::Invoke()
{
	//mrEventGPU->Start();
}

//=================================================================================================
// DRAWCALL EVENT STOP
//=================================================================================================
zEngineRef<Command> CommandEventStop::Create(const zcRes::GfxRenderPassRef& _rRenderPass/*, const zenPerf::zEventRef& _rEventGPU*/)
{
//	ZENAssert(_rEventGPU.IsValid());
	static zenMem::zAllocatorPool sMemPool("Pool CommandEventStop", sizeof(CommandEventStop), 128, 128);
	CommandEventStop* pCmdEventStop = zenNew(&sMemPool) CommandEventStop;
	pCmdEventStop->ConfigureBase(_rRenderPass, 999999.f, 0, keGpuPipe_PostDrawCommand);
//	pDrawcallEventStop->mrEventGPU = _rEventGPU;
	return pCmdEventStop;
}

void CommandEventStop::Invoke()
{
	//mrEventGPU->Stop();
}


}
