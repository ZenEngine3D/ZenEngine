#include "zcCore.h"

namespace zcGfx 
{

//=================================================================================================
// DRAW COMMAND 
//=================================================================================================
zEngineRef<Command> CommandDraw_DX12::Create(const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst, zU32 _uIndexCount, const zVec4U16& _vScreenScissor)
{
	zEngineRef<Command> rCommand	= CommandDraw::Create( _rRenderPass, _rMeshStrip, _uIndexFirst, _uIndexCount, _vScreenScissor );
	CommandDraw_DX12* pCommandDraw	= reinterpret_cast<CommandDraw_DX12*>( rCommand.GetSafe() );
	pCommandDraw->mrPSO				= PSO_DX12::GetAdd( _rRenderPass, _rMeshStrip );
	return rCommand;
}

void CommandDraw_DX12::Invoke(GPUContext& _Context)
{
	if( mrMeshStrip.IsValid() && mrPSO->IsCompiled() )
	{
		zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_DrawIndexed);		//! @todo 1 optim find cheaper method to count # drawcall, large memory/time overhead with large drawcount		
		_Context.UpdateState( *this );

		//! @todo 0 remove all of this hacked code
		ID3D12DescriptorHeap* ppHeaps[] = { zcGfx::DescriptorSRV_UAV_CBV::GetDescHeap().Get() };
		zcMgr::GfxRender.mGpuContext[0].GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		zcMgr::GfxRender.mGpuContext[0].GetCommandList()->SetGraphicsRootDescriptorTable(0, zcMgr::GfxRender.mrTmpTexture.HAL()->mTextureView.GetGpuHandle());
		// Record commands.
// 		zcMgr::GfxRender.mGpuContext[0].GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
// 		zcMgr::GfxRender.mGpuContext[0].GetCommandList()->IASetVertexBuffers(0, 1, &zcMgr::GfxRender.mTmpVertexBufferView);

		_Context.GetCommandList()->DrawInstanced(3, 1, 0, 0);
	}
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
void CommandClearColor_DX12::Invoke(GPUContext& _Context)
{	
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearColor);
	if( mrRTColor.IsValid() && mrRTColor.HAL()->mTargetColorView.IsValid() )
		_Context.GetCommandList()->ClearRenderTargetView(mrRTColor.HAL()->mTargetColorView.GetCpuHandle(), mvColor.xyzw, 0, nullptr);
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
void CommandClearDepthStencil_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearDepth);
	UINT ClearFlags  = mbClearDepth		? D3D12_CLEAR_FLAG_DEPTH	: 0;
	ClearFlags		|= mbClearStencil	? D3D12_CLEAR_FLAG_STENCIL	: 0; 
	if( mrRTDepthStencil.IsValid() && mrRTDepthStencil.HAL()->mTargetDepthView.IsValid() )
		_Context.GetCommandList()->ClearDepthStencilView( mrRTDepthStencil.HAL()->mTargetDepthView.GetCpuHandle(), (D3D12_CLEAR_FLAGS)ClearFlags, mfDepthValue, muStencilValue, 0, nullptr);
}


//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command> CommandUpdateIndex_DX12::Create(const zcRes::GfxIndexRef& _rIndex, zUInt _uOffset, zUInt _uSize)
{
	zenAssert(_rIndex.HAL()->mrResourceUpload.Get() != nullptr);
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateIndex", sizeof(CommandUpdateIndex_DX12), 128, 128);
	auto pCmdUpdateIndex				= zenNew(&sMemPool) CommandUpdateIndex_DX12;	
	pCmdUpdateIndex->mrIndex			= _rIndex;
	pCmdUpdateIndex->mrResourceUpload	= _rIndex.HAL()->mrResourceUpload;
	pCmdUpdateIndex->muOffset			= _uOffset;
	pCmdUpdateIndex->muSize				= _uSize;
	pCmdUpdateIndex->SetSortKeyDataUpdate(_rIndex.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateIndex_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateIndex);	
	zcRes::GfxIndex_DX12* pIndexDX12 = mrIndex.HAL();
	
	//! @todo 3 clean use context, to append to a provided commandlist instead of using default device. 
	//! @todo 3 clean better management of barriers

	// Switch buffer Mode to copy destination
	if( (pIndexDX12->meResourceState & D3D12_RESOURCE_STATE_COPY_DEST) == 0 )
	{
		D3D12_RESOURCE_BARRIER BarrierDesc = {};
        BarrierDesc.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        BarrierDesc.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
        BarrierDesc.Transition.pResource	= pIndexDX12->mrResource.Get();
        BarrierDesc.Transition.StateBefore	= pIndexDX12->meResourceState;
        BarrierDesc.Transition.StateAfter	= D3D12_RESOURCE_STATE_COPY_DEST;
        BarrierDesc.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		pIndexDX12->meResourceState			= BarrierDesc.Transition.StateAfter;
		_Context.GetCommandList()->ResourceBarrier(1, &BarrierDesc);
	}

	//Send to transfer command
	_Context.GetCommandList()->CopyBufferRegion(pIndexDX12->mrResource.Get(), muOffset, mrResourceUpload.Get(), muOffset, muSize);
#if 0
	//commandList->CopyBufferRegion(m_sceneVertexBuffer.Get(), 0, sceneVertexBufferUpload.Get(), 0, vertexBufferSize);
	//commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_sceneVertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
#endif
}


//=================================================================================================
// DRAW COMMAND UPDATE BUFFER
//=================================================================================================
zEngineRef<Command>	CommandUpdateBuffer_DX12::Create(const zcRes::GfxBufferRef& _rBuffer, zUInt _uOffset, zUInt _uSize)
{
	zenAssert(_rBuffer.HAL()->mrResourceUpload.Get() != nullptr);
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateBuffer", sizeof(CommandUpdateBuffer_DX12), 128, 128);
	
	_uOffset								= 0; //! @todo 2 support partial updates
	_uSize									= zenMath::Min(_uSize, (zUInt)_rBuffer.HAL()->muElementCount*_rBuffer.HAL()->muElementStride - _uOffset);
	auto pCmdUpdateBuffer					= zenNew(&sMemPool) CommandUpdateBuffer_DX12;
	pCmdUpdateBuffer->mrBuffer				= _rBuffer;
	pCmdUpdateBuffer->mrResourceUpload		= _rBuffer.HAL()->mrResourceUpload;
	pCmdUpdateBuffer->muOffset				= _uOffset;
	pCmdUpdateBuffer->muSize				= _uSize;
	pCmdUpdateBuffer->SetSortKeyDataUpdate(_rBuffer.GetResID().GetHashID());
	return pCmdUpdateBuffer;
}

void CommandUpdateBuffer_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateBuffer);	
	zcRes::GfxBuffer_DX12* pBufferDX12 = mrBuffer.HAL();

	//! @todo 3 clean use context, to append to a provided commandlist instead of using default device. 
	//! @todo 3 clean better management of barriers

	// Switch buffer Mode to copy destination
	if( (pBufferDX12->meResourceState & D3D12_RESOURCE_STATE_COPY_DEST) == 0 )
	{
		D3D12_RESOURCE_BARRIER BarrierDesc = {};
        BarrierDesc.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        BarrierDesc.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
        BarrierDesc.Transition.pResource	= pBufferDX12->mrResource.Get();
        BarrierDesc.Transition.StateBefore	= pBufferDX12->meResourceState;
        BarrierDesc.Transition.StateAfter	= D3D12_RESOURCE_STATE_COPY_DEST;
        BarrierDesc.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;        
		pBufferDX12->meResourceState		= BarrierDesc.Transition.StateAfter;
		_Context.GetCommandList()->ResourceBarrier(1, &BarrierDesc);
	}

	//Send to transfer command
	_Context.GetCommandList()->CopyBufferRegion(pBufferDX12->mrResource.Get(), muOffset, mrResourceUpload.Get(), muOffset, muSize);
}

//=================================================================================================
// DRAW COMMAND UPDATE BUFFER
//=================================================================================================
zEngineRef<Command>	CommandUpdateTexture_DX12::Create(const zcRes::GfxTexture2DRef& _rTexture, zUInt _uSize )
{
	zenAssert(_rTexture.HAL()->mrResourceUpload.Get() != nullptr);
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateTexture", sizeof(CommandUpdateTexture_DX12), 128, 128);
	auto pCmdUpdateTexture					= zenNew(&sMemPool) CommandUpdateTexture_DX12;
	pCmdUpdateTexture->mrTexture			= _rTexture;
	pCmdUpdateTexture->mrResourceUpload		= _rTexture.HAL()->mrResourceUpload;
	pCmdUpdateTexture->muSize				= _uSize;
	pCmdUpdateTexture->SetSortKeyDataUpdate(_rTexture.GetResID().GetHashID());
	return pCmdUpdateTexture;
}

void CommandUpdateTexture_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateTexture);	
	zcRes::GfxTexture2D_DX12* pTextureDX12 = mrTexture.HAL();

	// Switch buffer Mode to copy destination
	if( (pTextureDX12->meResourceState & D3D12_RESOURCE_STATE_COPY_DEST) == 0 )
	{
		D3D12_RESOURCE_BARRIER BarrierDesc = {};
        BarrierDesc.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        BarrierDesc.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
        BarrierDesc.Transition.pResource	= pTextureDX12->mrResource.Get();
        BarrierDesc.Transition.StateBefore	= pTextureDX12->meResourceState;
        BarrierDesc.Transition.StateAfter	= D3D12_RESOURCE_STATE_COPY_DEST;
        BarrierDesc.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES; 		
		_Context.GetCommandList()->ResourceBarrier(1, &BarrierDesc);
		pTextureDX12->meResourceState		= BarrierDesc.Transition.StateAfter;
	}
	
	//Send to transfer command
	//zcMgr::GfxRender.m_commandList->CopyBufferRegion(pTextureDX12->mrResource.Get(), 0, mrResourceUpload.Get(), 0, muSize);
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layout;
    UINT64 RowSizesInBytes; UINT64 RequiredSize; UINT NumRows;	
    zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&pTextureDX12->mrResource->GetDesc(), 
		0, 1, 0, &Layout, &NumRows, &RowSizesInBytes, &RequiredSize);

	CD3DX12_TEXTURE_COPY_LOCATION Dst(pTextureDX12->mrResource.Get(), 0);
    CD3DX12_TEXTURE_COPY_LOCATION Src(mrResourceUpload.Get(), Layout);
    _Context.GetCommandList()->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
	//UpdateSubresources( zcMgr::GfxRender.m_commandList.Get(), mrResource.Get(), rTextureUploadHeap.Get(), 0, 0, 1, &textureData);

	//! @todo 1 better management of barrier (look at cmdlist to know if we're last and should switch back)
	//Temp reset resource barrier to read
	{
		D3D12_RESOURCE_BARRIER BarrierDesc = {};
        BarrierDesc.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        BarrierDesc.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
        BarrierDesc.Transition.pResource	= pTextureDX12->mrResource.Get();
        BarrierDesc.Transition.StateBefore	= pTextureDX12->meResourceState;
        BarrierDesc.Transition.StateAfter	= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        BarrierDesc.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;        
		_Context.GetCommandList()->ResourceBarrier(1, &BarrierDesc);
		pTextureDX12->meResourceState		= BarrierDesc.Transition.StateAfter;
	}
}

}
