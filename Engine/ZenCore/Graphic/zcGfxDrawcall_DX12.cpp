#include "zcCore.h"

namespace zcGfx
{
//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command>	CommandUpdateIndexDX12::Create(const zcRes::GfxIndexRef& _rIndex, zUInt _uOffset, zUInt _uSize)
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateIndex", sizeof(CommandUpdateIndexDX12), 128, 128);
	CommandUpdateIndexDX12* pCmdUpdateIndex	= zenNew(&sMemPool) CommandUpdateIndexDX12;
	zenAssert(_rIndex.HAL()->mrLockData.Get() != nullptr);

	pCmdUpdateIndex->mrIndex					= _rIndex;
	pCmdUpdateIndex->mrUpdateData				= _rIndex.HAL()->mrLockData;
	pCmdUpdateIndex->muOffset					= _uOffset;
	pCmdUpdateIndex->muSize						= _uSize;
	pCmdUpdateIndex->SetSortKeyDataUpdate(_rIndex.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateIndexDX12::Invoke()
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateIndex);	
	zcRes::GfxIndexHAL_DX12* pIndexDX12 = mrIndex.HAL();

	//! @todo 3 clean use context, to append to a provided commandlist instead of using default device. 
	//! @todo 3 clean better management of barriers

	// Switch buffer Mode to copy destination
	if( (pIndexDX12->meBufferState & D3D12_RESOURCE_STATE_COPY_DEST) != 0 )
	{
		D3D12_RESOURCE_BARRIER BarrierDesc = {};
        BarrierDesc.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        BarrierDesc.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
        BarrierDesc.Transition.pResource	= pIndexDX12->mrBuffer.Get();
        BarrierDesc.Transition.StateBefore	= pIndexDX12->meBufferState;
        BarrierDesc.Transition.StateAfter	= D3D12_RESOURCE_STATE_COPY_DEST;
        BarrierDesc.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;        
		zcMgr::GfxRender.m_commandList->ResourceBarrier(1, &BarrierDesc);
	}
	pIndexDX12->meBufferState = D3D12_RESOURCE_STATE_COPY_DEST;

	//Send to transfer command
	zcMgr::GfxRender.m_commandList->CopyBufferRegion(pIndexDX12->mrBuffer.Get(), muOffset, mrUpdateData.Get(), muOffset, muSize);
#if 0
	//commandList->CopyBufferRegion(m_sceneVertexBuffer.Get(), 0, sceneVertexBufferUpload.Get(), 0, vertexBufferSize);
	//commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_sceneVertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
#endif
}


//=================================================================================================
// DRAW COMMAND UPDATE BUFFER
//=================================================================================================
zEngineRef<Command>	CommandUpdateBufferDX12::Create(const zcRes::GfxBufferRef& _rBuffer, zUInt _uOffset, zUInt _uSize)
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateBuffer", sizeof(CommandUpdateBufferDX12), 128, 128);
	CommandUpdateBufferDX12* pCmdUpdateIndex	= zenNew(&sMemPool) CommandUpdateBufferDX12;
	zenAssert(_rBuffer.HAL()->mrLockData.Get() != nullptr);

	_uOffset		= 0; //! @todo 2 support partial updates
	_uSize			= zenMath::Min(_uSize, (zUInt)_rBuffer.HAL()->muElementCount*_rBuffer.HAL()->muElementStride - _uOffset);

	pCmdUpdateIndex->mrBuffer					= _rBuffer;
	pCmdUpdateIndex->mrUpdateData				= _rBuffer.HAL()->mrLockData;
	pCmdUpdateIndex->muOffset					= _uOffset;
	pCmdUpdateIndex->muSize						= _uSize;
	pCmdUpdateIndex->SetSortKeyDataUpdate(_rBuffer.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateBufferDX12::Invoke()
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateBuffer);	
	zcRes::GfxBufferHAL_DX12* pBufferDX12 = mrBuffer.HAL();

	//! @todo 3 clean use context, to append to a provided commandlist instead of using default device. 
	//! @todo 3 clean better management of barriers

	// Switch buffer Mode to copy destination
	if( (pBufferDX12->meBufferState & D3D12_RESOURCE_STATE_COPY_DEST) != 0 )
	{
		D3D12_RESOURCE_BARRIER BarrierDesc = {};
        BarrierDesc.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        BarrierDesc.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
        BarrierDesc.Transition.pResource	= pBufferDX12->mrBuffer.Get();
        BarrierDesc.Transition.StateBefore	= pBufferDX12->meBufferState;
        BarrierDesc.Transition.StateAfter	= D3D12_RESOURCE_STATE_COPY_DEST;
        BarrierDesc.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;        
		zcMgr::GfxRender.m_commandList->ResourceBarrier(1, &BarrierDesc);
	}
	pBufferDX12->meBufferState = D3D12_RESOURCE_STATE_COPY_DEST;

	//Send to transfer command
	zcMgr::GfxRender.m_commandList->CopyBufferRegion(pBufferDX12->mrBuffer.Get(), muOffset, mrUpdateData.Get(), muOffset, muSize);
#if 0
	//commandList->CopyBufferRegion(m_sceneVertexBuffer.Get(), 0, sceneVertexBufferUpload.Get(), 0, vertexBufferSize);
	//commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_sceneVertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
#endif
}

}
