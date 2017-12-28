#include "zcCore.h"

#if THIRDPARTY_PIXEVENT && defined(USE_PIX)
#include <pix3.h>
#endif

namespace zcGfx 
{

//=================================================================================================
// DRAW COMMAND 
//=================================================================================================
zEngineRef<Command> CommandDraw_DX12::Add(const CommandListRef& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor)
{
	zEngineRef<Command> rCommand	= CommandDraw::Add(_rContext, _rRenderPass, _rMeshStrip, _uIndexFirst, _uIndexCount, _vScreenScissor );
	CommandDraw_DX12* pCommandDraw	= reinterpret_cast<CommandDraw_DX12*>( rCommand.GetSafe() );
	pCommandDraw->mrPSO				= PSO_DX12::GetAdd( _rRenderPass, _rMeshStrip );
	const zArrayDynamic<zcRes::GfxShaderResourceDescRef>& aTrackedResource = _rMeshStrip.HAL()->maTrackedResources;
	for( zUInt idx(0), count(aTrackedResource.Count()); idx<count; ++idx)
		_rContext->AddBarrierCheck(true, CommandList_DX12::BarrierCheck(aTrackedResource[idx].GetGpuBuffer(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	return rCommand;
}

void CommandDraw_DX12::Invoke(GPUContext& _Context)
{
	if( mrMeshStrip.IsValid() && mrPSO->IsCompiled() )
	{
		zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_DrawIndexed);		//! @todo 1 optim find cheaper method to count # drawcall, large memory/time overhead with large drawcount		
		_Context.UpdateState( *this );
		_Context.GetCommandList()->DrawIndexedInstanced(muIndexCount, 1, muIndexFirst, 0, 0);
	}
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command> CommandClearColor_DX12::Add(const CommandListRef& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTColor, const zVec4F& _vRGBA,  const zColorMask& _ColorMask, const zVec2S16& _vOrigin, const zVec2U16& _vDim)
{
	if( _rRTColor->GetTexture2D().IsValid() ) // Backbuffer doesn't have associated Texture2D
		_rContext->AddBarrierCheck(true, CommandList_DX12::BarrierCheck(&_rRTColor->GetTexture2D().HAL()->mResource, D3D12_RESOURCE_STATE_RENDER_TARGET));
	return CommandClearColor::Add(_rContext, _rRenderPass, _rRTColor, _vRGBA, _ColorMask, _vOrigin, _vDim);
}
void CommandClearColor_DX12::Invoke(GPUContext& _Context)
{	
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearColor);
	if( mrRTColor.IsValid() && mrRTColor.HAL()->mTargetColorView.IsValid() )
		_Context.GetCommandList()->ClearRenderTargetView(mrRTColor.HAL()->mTargetColorView.GetCpu(), mvColor.xyzw, 0, nullptr);
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command> CommandClearDepthStencil_DX12::Add(const CommandListRef& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTDepth, bool _bClearDepth, float _fDepthValue, bool _bClearStencil, zU8 _uStencilValue)
{
	_rContext->AddBarrierCheck(true, CommandList_DX12::BarrierCheck(&_rRTDepth->GetTexture2D().HAL()->mResource, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	return CommandClearDepthStencil::Add(_rContext, _rRenderPass, _rRTDepth, _bClearDepth, _fDepthValue, _bClearStencil, _uStencilValue);
}
void CommandClearDepthStencil_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearDepth);
	UINT ClearFlags  = mbClearDepth		? D3D12_CLEAR_FLAG_DEPTH	: 0;
	ClearFlags		|= mbClearStencil	? D3D12_CLEAR_FLAG_STENCIL	: 0; 
	if( mrRTDepthStencil.IsValid() && mrRTDepthStencil.HAL()->mTargetDepthView.IsValid() )
		_Context.GetCommandList()->ClearDepthStencilView( mrRTDepthStencil.HAL()->mTargetDepthView.GetCpu(), (D3D12_CLEAR_FLAGS)ClearFlags, mfDepthValue, muStencilValue, 0, nullptr);
}

//=================================================================================================
// DRAW COMMAND GPU SCOPED EVENT
//=================================================================================================
void CommandGPUScopedEvent_DX12::Invoke(GPUContext& _Context)
{
#if THIRDPARTY_PIXEVENT && defined(USE_PIX)
	if( meEventInfo == keEventStart )
	{
		UINT64 uColor = UINT64(0xFF*mvColor.r) | UINT64(0xFF*mvColor.g)<<8 | UINT64(0xFF*mvColor.b)<<16 | UINT64(0xFF*mvColor.a)<<24;
		PIXBeginEvent(_Context.GetCommandList().Get(), uColor, mzEventName);
	}
	else
		PIXEndEvent(_Context.GetCommandList().Get());
#endif
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command> CommandUpdateIndex_DX12::Add(const CommandListRef& _rContext, const zcRes::GfxIndexRef& _rIndex, zUInt _uOffset, zUInt _uSize)
{
	zenAssert(_rIndex.HAL()->mResource.mrUpload.Get() != nullptr);
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateIndex", sizeof(CommandUpdateIndex_DX12), 128, 128);
	auto pCommand					= zenNew(&sMemPool) CommandUpdateIndex_DX12;	
	pCommand->mrIndex				= _rIndex;
	pCommand->mrResourceUpload		= _rIndex.HAL()->mResource.mrUpload;
	pCommand->muOffset				= _uOffset;
	pCommand->muSize				= _uSize;
	pCommand->SetSortKeyGeneric(keGpuPipe_DataUpdate, _rIndex.GetResID().GetHashID());
	
	_rContext->AddCommand(pCommand);
	_rContext->AddBarrierCheck(true,	CommandList_DX12::BarrierCheck(&_rIndex.HAL()->mResource, D3D12_RESOURCE_STATE_COPY_DEST));
	_rContext->AddBarrierCheck(false,	CommandList_DX12::BarrierCheck(&_rIndex.HAL()->mResource, D3D12_RESOURCE_STATE_INDEX_BUFFER));	
	return pCommand;
}

void CommandUpdateIndex_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateIndex);	
	zcRes::GfxIndex_DX12* pIndexDX12 = mrIndex.HAL();
	zenAssert( pIndexDX12->mResource.meState & D3D12_RESOURCE_STATE_COPY_DEST );	// Should have been changed to proper stage by WantedBarrierState check
	_Context.GetCommandList()->CopyBufferRegion(pIndexDX12->mResource.mrResource.Get(), muOffset, mrResourceUpload.Get(), muOffset, muSize);
}


//=================================================================================================
// DRAW COMMAND UPDATE BUFFER
//=================================================================================================
zEngineRef<Command> CommandUpdateBuffer_DX12::Add(const CommandListRef& _rContext, const zcRes::GfxBufferRef& _rBuffer, zUInt _uOffset, zUInt _uSize)
{
	zenAssert(_rBuffer.HAL()->mResource.mrUpload.Get() != nullptr);
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateBuffer", sizeof(CommandUpdateBuffer_DX12), 128, 128);
	
	_uOffset						= 0; //! @todo 2 support partial updates
	_uSize							= zenMath::Min(_uSize, (zUInt)_rBuffer.HAL()->muElementCount*_rBuffer.HAL()->muElementStride - _uOffset);
	auto pCommand					= zenNew(&sMemPool) CommandUpdateBuffer_DX12;
	pCommand->mrBuffer				= _rBuffer;
	pCommand->mrResourceUpload		= _rBuffer.HAL()->mResource.mrUpload;
	pCommand->muOffset				= _uOffset;
	pCommand->muSize				= _uSize;
	pCommand->SetSortKeyGeneric(keGpuPipe_DataUpdate, _rBuffer.GetResID().GetHashID());
	
	_rContext->AddCommand(pCommand);
	_rContext->AddBarrierCheck(true,	CommandList_DX12::BarrierCheck(&_rBuffer.HAL()->mResource, D3D12_RESOURCE_STATE_COPY_DEST));
	_rContext->AddBarrierCheck(false,	CommandList_DX12::BarrierCheck(&_rBuffer.HAL()->mResource, D3D12_RESOURCE_STATE_GENERIC_READ));
	return pCommand;
}

void CommandUpdateBuffer_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateBuffer);	
	zcRes::GfxBuffer_DX12* pBufferDX12 = mrBuffer.HAL();
	zenAssert( pBufferDX12->mResource.meState & D3D12_RESOURCE_STATE_COPY_DEST );	// Should have been changed to proper stage by WantedBarrierState check
	_Context.GetCommandList()->CopyBufferRegion(pBufferDX12->mResource.mrResource.Get(), muOffset, mrResourceUpload.Get(), muOffset, muSize);
}

//=================================================================================================
// DRAW COMMAND UPDATE BUFFER
//=================================================================================================
zEngineRef<Command> CommandUpdateTexture_DX12::Add(const CommandListRef& _rContext, const zcRes::GfxTexture2DRef& _rTexture, zUInt _uSize )
{
	zenAssert(_rTexture.HAL()->mResource.mrUpload.Get() != nullptr);
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateTexture", sizeof(CommandUpdateTexture_DX12), 128, 128);
	auto pCommand					= zenNew(&sMemPool) CommandUpdateTexture_DX12;
	pCommand->mrTexture				= _rTexture;
	pCommand->mrResourceUpload		= _rTexture.HAL()->mResource.mrUpload;
	pCommand->muSize				= _uSize;
	pCommand->SetSortKeyGeneric(keGpuPipe_DataUpdate, _rTexture.GetResID().GetHashID());
	_rContext->AddCommand(pCommand);
	_rContext->AddBarrierCheck(true,	CommandList_DX12::BarrierCheck(&_rTexture.HAL()->mResource, D3D12_RESOURCE_STATE_COPY_DEST));
	_rContext->AddBarrierCheck(false,	CommandList_DX12::BarrierCheck(&_rTexture.HAL()->mResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	return pCommand;
}

void CommandUpdateTexture_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateTexture);	
	zcRes::GfxTexture2D_DX12* pTextureDX12 = mrTexture.HAL();
	zenAssert( pTextureDX12->mResource.meState & D3D12_RESOURCE_STATE_COPY_DEST );	// Should have been changed to proper stage by WantedBarrierState check

	//Send to transfer command
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layout;
    UINT64 RowSizesInBytes; UINT64 RequiredSize; UINT NumRows;	
    zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&pTextureDX12->mResource.mrResource->GetDesc(), 
		0, 1, 0, &Layout, &NumRows, &RowSizesInBytes, &RequiredSize);

	CD3DX12_TEXTURE_COPY_LOCATION Dst(pTextureDX12->mResource.mrResource.Get(), 0);
    CD3DX12_TEXTURE_COPY_LOCATION Src(mrResourceUpload.Get(), Layout);
    _Context.GetCommandList()->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command> CommandQueryEnd_DX12::Add(const CommandListRef& _rContext, QueryHeapRingbuffer_DX12* _pQueryHeap, zUInt _uQueryIndex, bool _bStartOfCmdList)
{
	zenAssert(_pQueryHeap != nullptr);
	static zenMem::zAllocatorPool sMemPool("Pool CommandQueryEnd", sizeof(CommandQueryEnd_DX12), 128, 128);
	auto pCommand			= zenNew(&sMemPool) CommandQueryEnd_DX12;	
	pCommand->mpQueryHeap	= _pQueryHeap;
	pCommand->muQueryIndex	= _uQueryIndex;
	pCommand->SetSortKeyGeneric(_bStartOfCmdList ? keGpuPipe_First : keGpuPipe_Last, _uQueryIndex);
	_rContext->AddCommand(pCommand);
	return pCommand;
}

void CommandQueryEnd_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_Query);	
	mpQueryHeap->QueryEnd(_Context, muQueryIndex);
}

}