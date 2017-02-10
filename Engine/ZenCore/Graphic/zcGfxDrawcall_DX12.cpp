#include "zcCore.h"

namespace zcGfx 
{

//=================================================================================================
// DRAW COMMAND 
//=================================================================================================
void CommandDraw_DX12::Invoke(GPUContext& _Context)
{
	if( mrMeshStrip.IsValid() )
	{
		//==============================================================================================
		//! @todo 0 remove all of this hacked code
		//==============================================================================================
		static zUInt sLastFrameCount = 0xFFFF;
		if( sLastFrameCount != zcMgr::GfxRender.muFrameRendered )
		{
			const zcRes::GfxTarget2DRef& rBackbuffer = zcGfx::grWindowRender->GetBackbuffer();
		
			// Set necessary state.
			//m_commandList->SetGraphicsRootSignature(mRootSignature.Get());
			//m_commandList->SetPipelineState( mTmpPipelineState.Get() );
			zcMgr::GfxRender.mGpuContext[0].UpdateState( *this );

			ID3D12DescriptorHeap* ppHeaps[] = { zcGfx::DescriptorSRV_UAV_CBV::GetDescHeap().Get() };
			zcMgr::GfxRender.mGpuContext[0].GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

			UINT width(1280), height(800);
			CD3DX12_VIEWPORT m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
			CD3DX12_RECT m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	
			zcMgr::GfxRender.mGpuContext[0].GetCommandList()->SetGraphicsRootDescriptorTable(0, zcMgr::GfxRender.mrTmpTexture.HAL()->mTextureView.GetGpuHandle());
			zcMgr::GfxRender.mGpuContext[0].GetCommandList()->RSSetViewports(1, &m_viewport);
			zcMgr::GfxRender.mGpuContext[0].GetCommandList()->RSSetScissorRects(1, &m_scissorRect);

			//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
			//m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
			D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle = rBackbuffer.HAL()->mTargetColorView.GetCpuHandle();
			zcMgr::GfxRender.mGpuContext[0].GetCommandList()->OMSetRenderTargets(1, &CpuHandle, FALSE, nullptr);

			// Record commands.
			//const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
			//m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
			zcMgr::GfxRender.mGpuContext[0].GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			zcMgr::GfxRender.mGpuContext[0].GetCommandList()->IASetVertexBuffers(0, 1, &zcMgr::GfxRender.mTmpVertexBufferView);
			zcMgr::GfxRender.mGpuContext[0].GetCommandList()->DrawInstanced(3, 1, 0, 0);

			sLastFrameCount = zcMgr::GfxRender.muFrameRendered; // Here to render 1st triangle only
		}
		//==============================================================================================

		//zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_DrawIndexed);		//! @todo 1 optim find cheaper method to count # drawcall, large memory/time overhead with large drawcount
		//_Context.UpdateState(*this);
		//_Context.GetDeviceContext()->DrawIndexed(muIndexCount, muIndexFirst, mrMeshStrip.HAL()->muVertexFirst);
	}
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
void CommandClearColor_DX12::Invoke(GPUContext& _Context)
{	
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearColor);
	//_Context.GetDeviceContext()->ClearRenderTargetView( mrRTColor.HAL()->mpTargetColorView, mvColor.xyzw );	
}

//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
void CommandClearDepthStencil_DX12::Invoke(GPUContext& _Context)
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_ClearDepth);	
	//UINT ClearFlags  = mbClearDepth		? D3D11_CLEAR_DEPTH		: 0;
	//ClearFlags		|= mbClearStencil	? D3D11_CLEAR_STENCIL	: 0; 
	//_Context.GetDeviceContext()->ClearDepthStencilView( mrRTDepthStencil.HAL()->mpTargetDepthView, ClearFlags, mfDepthValue, muStencilValue );
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
