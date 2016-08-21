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
// DRAWCALL
//=================================================================================================
zEngineRef<Command> CommandDraw::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor)
{
	//! @todo perf switch this to 3x ring buffer with frame lifespan
	static zenMem::zAllocatorPool sMemPool("Pool CommandDraw", sizeof(CommandDraw), 1024, 1024 );
	CommandDraw* pDrawcall			= zenNew(&sMemPool) CommandDraw;		
	pDrawcall->mbIsCommandDraw		= true;
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
	static zenMem::zAllocatorPool sMemPool("Pool CommandClearColor", sizeof(CommandClearColor), 128, 128 );
	CommandClearColor* pCmdClearColor	= zenNew(&sMemPool) CommandClearColor;		
	pCmdClearColor->mrRTColor			= _rRTColor;
	pCmdClearColor->mvOrigin			= _vOrigin;
	pCmdClearColor->mvDim				= _vDim;
	pCmdClearColor->mvColor				= _vRGBA;
	pCmdClearColor->mColorMask			= _ColorMask;
	pCmdClearColor->SetSortKeyDataUpdate(_rRTColor.GetResID().GetHashID() );
	return pCmdClearColor;
}

void CommandClearColor::Invoke()
{	
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearColor);
	mrRTColor->Clear(mvColor); //! @todo Urgent Fully implement for all RTs
}

//=================================================================================================
// DRAWCALL CLEAR DEPTH/STENCIL
//=================================================================================================
zEngineRef<Command> CommandClearDepthStencil::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTDepth, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandClearDepth", sizeof(CommandClearDepthStencil), 128, 128 );
	CommandClearDepthStencil* pCmdClearDepthStencil	= zenNew(&sMemPool) CommandClearDepthStencil;		
	pCmdClearDepthStencil->mrRTDepthStencil	= _rRTDepth;
	pCmdClearDepthStencil->mbClearDepth		= _bClearDepth;
	pCmdClearDepthStencil->mfDepthValue		= _fDepthValue;
	pCmdClearDepthStencil->mbClearStencil	= _bClearStencil;
	pCmdClearDepthStencil->muStencilValue	= _uStencilValue;
	pCmdClearDepthStencil->SetSortKeyDataUpdate(_rRTDepth.GetResID().GetHashID());
	return pCmdClearDepthStencil;
}

void CommandClearDepthStencil::Invoke()
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearDepth);
	mrRTDepthStencil->Clear(mfDepthValue, muStencilValue, mbClearDepth, mbClearStencil);
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command>	CommandUpdateIndex::Create(const zcRes::GfxIndexRef& _rIndex, zU8* _pData, zUInt _uOffset, zUInt _uSize)
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateIndex", sizeof(CommandUpdateIndex), 128, 128);
	CommandUpdateIndex* pCmdUpdateIndex = zenNew(&sMemPool) CommandUpdateIndex;
	pCmdUpdateIndex->mrIndex			= _rIndex;
	pCmdUpdateIndex->mpData				= _pData;
	pCmdUpdateIndex->muOffset			= _uOffset;
	pCmdUpdateIndex->muSize				= _uSize;
	pCmdUpdateIndex->SetSortKeyDataUpdate(_rIndex.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateIndex::Invoke()
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateIndex);
	mrIndex->Update(mpData, muOffset, muSize);
	zenDelArray( mpData );
	mpData = nullptr;
}

//=================================================================================================
// DRAW COMMAND UPDATE VERTEX BUFFER
//=================================================================================================
zEngineRef<Command>	CommandUpdateVertex::Create(const zcRes::GfxVertexRef& _rVertex, zU8* _pData, zUInt _uOffset, zUInt _uSize)
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateVertex", sizeof(CommandUpdateVertex), 128, 128);
	CommandUpdateVertex* pCmdUpdateIndex = zenNew(&sMemPool) CommandUpdateVertex;
	pCmdUpdateIndex->mrVertex			= _rVertex;
	pCmdUpdateIndex->mpData				= _pData;
	pCmdUpdateIndex->muOffset			= _uOffset;
	pCmdUpdateIndex->muSize				= _uSize;
	pCmdUpdateIndex->SetSortKeyDataUpdate(_rVertex.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateVertex::Invoke()
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateVertex);
	mrVertex->Update(mpData, muOffset, muSize);
	zenDelArray( mpData );
	mpData = nullptr;
}

}
