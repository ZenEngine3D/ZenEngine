#include "zcCore.h"
#if THIRDPARTY_PIXEVENT && defined(USE_PIX)
#include <pix3.h>
#endif

namespace zcGfx
{

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
void CommandDraw_DX11::Invoke(zcGfx::GPUContext& _Context)
{
	if( mrMeshStrip.IsValid() )
	{
		zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_DrawIndexed);		//! @todo 1 optim find cheaper method to count # drawcall, large memory/time overhead with large drawcount
		_Context.UpdateState(*this);
		_Context.GetDeviceContext()->DrawIndexed(muIndexCount, muIndexFirst, 0);
	}
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
void CommandClearColor_DX11::Invoke(zcGfx::GPUContext& _Context)
{	
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearColor);
	_Context.GetDeviceContext()->ClearRenderTargetView( mrRTColor.HAL()->mpTargetColorView, mvColor.xyzw );	
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
void CommandClearDepthStencil_DX11::Invoke(zcGfx::GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearDepth);	
	UINT ClearFlags  = mbClearDepth		? D3D11_CLEAR_DEPTH		: 0;
	ClearFlags		|= mbClearStencil	? D3D11_CLEAR_STENCIL	: 0; 
	_Context.GetDeviceContext()->ClearDepthStencilView( mrRTDepthStencil.HAL()->mpTargetDepthView, ClearFlags, mfDepthValue, muStencilValue );
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<zcGfx::Command> CommandUpdateIndex_DX11::Create(const zcRes::GfxIndexRef& _rIndex, zU8* _pData, zUInt _uOffset, zUInt _uSize)
{
	//static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateIndex", sizeof(CommandUpdateIndex_DX11), 128, 128);
	auto* pCmdUpdateIndex		= zenNewPool CommandUpdateIndex_DX11;
	
	_uOffset					= 0;//zenMath::Min(_uOffset, (zUInt)muIndiceCount); //! @todo Urgent support partial updates
	_uSize						= zenMath::Min(_uSize, (zUInt)_rIndex.HAL()->maIndices.SizeMem() - _uOffset);

	pCmdUpdateIndex->mrIndex	= _rIndex;
	pCmdUpdateIndex->mpData		= _pData;
	pCmdUpdateIndex->muOffset	= _uOffset;
	pCmdUpdateIndex->muSize		= _uSize;
	pCmdUpdateIndex->SetSortKeyGeneric(keGpuPipe_DataUpdate, _rIndex.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateIndex_DX11::Invoke(zcGfx::GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateIndex);
	zcRes::GfxIndex_DX11* pIndexDX11 = mrIndex.HAL();
	//mrIndex->Update(mpData, muOffset, muSize);	
	zenAssert(pIndexDX11->mpIndiceBuffer);	
	D3D11_MAPPED_SUBRESOURCE mapRes;
	
	HRESULT result = _Context.GetDeviceContext()->Map(pIndexDX11->mpIndiceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	if( result == S_OK )
	{
		memcpy((zU8*)mapRes.pData, mpData, muSize); 
		_Context.GetDeviceContext()->Unmap(pIndexDX11->mpIndiceBuffer, 0);
	}
	zenDelArrayNullptr( mpData );
}

//=================================================================================================
// DRAW COMMAND UPDATE BUFFER
//=================================================================================================
zEngineRef<zcGfx::Command> CommandUpdateBuffer_DX11::Create(const zcRes::GfxBufferRef& _rBuffer, zU8* _pUpdateData, zUInt _uOffset, zUInt _uSize)
{
	//static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateBuffer", sizeof(CommandUpdateBuffer_DX11), 128, 128);
	auto pCmdUpdateIndex			= zenNewPool CommandUpdateBuffer_DX11;
	_uOffset						= 0; //! @todo 2 support partial updates
	_uSize							= zenMath::Min(_uSize, (zUInt)_rBuffer.HAL()->muElementCount*_rBuffer.HAL()->muElementStride - _uOffset);

	pCmdUpdateIndex->mrBuffer		= _rBuffer;
	pCmdUpdateIndex->mpUpdateData	= _pUpdateData;
	pCmdUpdateIndex->muOffset		= _uOffset;
	pCmdUpdateIndex->muSize			= _uSize;
	pCmdUpdateIndex->SetSortKeyGeneric(keGpuPipe_DataUpdate, _rBuffer.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateBuffer_DX11::Invoke(zcGfx::GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateBuffer);	
	//! @todo optim select proper write type
	D3D11_MAPPED_SUBRESOURCE mapRes;	
	HRESULT result = _Context.GetDeviceContext()->Map(mrBuffer.HAL()->mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	if( result == S_OK )
	{
		memcpy((zU8*)mapRes.pData, mpUpdateData, muSize); 
		_Context.GetDeviceContext()->Unmap(mrBuffer.HAL()->mpBuffer, 0);
	}
	zenDelArrayNullptr(mpUpdateData);
	
}

//=================================================================================================
// DRAW COMMAND GPU SCOPED EVENT
//=================================================================================================
void CommandGPUScopedEvent_DX11::Invoke(GPUContext& _Context)
{
	if( meEventInfo == keEventStart )
		zcMgr::GfxRender.NamedEventBegin(mzEventName);
	else
		zcMgr::GfxRender.NamedEventEnd();
/*
 	if( mbDX11ProfilerDetected )
 	{
 		WCHAR zEventName[64];
 		mbstowcs_s(nullptr, zEventName, zName.mzName, zenArrayCount(zEventName));
 		mDX11pPerf->BeginEvent(zEventName);
 	}*/ 
#if 0 && THIRDPARTY_PIXEVENT && defined(USE_PIX)
 	if( meEventInfo == keEventStart )
 	{
 		UINT64 uColor = UINT64(0xFF*mvColor.r) | UINT64(0xFF*mvColor.g)<<8 | UINT64(0xFF*mvColor.b)<<16 | UINT64(0xFF*mvColor.a)<<24;
 		PIXBeginEvent(_Context.GetDeviceContext(), uColor, mzEventName);
 	}
 	else
 		PIXEndEvent(_Context.GetDeviceContext());
#endif
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command> CommandQueryEnd_DX11::Add(const CommandListRef& _rContext, ID3D11Query* _pQuery, bool _bStartOfCmdList)
{
	//static zenMem::zAllocatorPool sMemPool("Pool CommandQueryEnd", sizeof(CommandQueryEnd_DX11), 128, 128);
	auto pCommand			= zenNewPool CommandQueryEnd_DX11;	
	pCommand->mpQuery		= _pQuery;
	pCommand->SetSortKeyGeneric(_bStartOfCmdList ? keGpuPipe_First : keGpuPipe_Last, (zU64)_pQuery);
	_rContext->AddCommand(pCommand);
	return pCommand;
}

void CommandQueryEnd_DX11::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_Query);	
	_Context.GetDeviceContext()->End(mpQuery);
}
}
