#pragma once

namespace zcGfx
{

DescriptorRangeSRV ManagerRender_DX12::GetDescriptorSRV(zUInt _uCount)
{
	return mDescriptorHeapSRV.Allocate(_uCount);
}

DescriptorRangeRTV ManagerRender_DX12::GetDescriptorRTV(zUInt _uCount)
{
	return mDescriptorHeapRTV.Allocate(_uCount);
}

DescriptorRangeDSV ManagerRender_DX12::GetDescriptorDSV(zUInt _uCount)
{
	return mDescriptorHeapDSV.Allocate(_uCount);
}

DescriptorRangeSampler ManagerRender_DX12::GetDescriptorSampler(zUInt _uCount)
{
	return mDescriptorHeapSampler.Allocate(_uCount);
}

DescriptorRangeSRV ManagerRender_DX12::GetFrameDescriptorSRV(zUInt _uCount)
{
	zenAssert( _uCount > 0 );
	zUInt uIndex = muFrameDescriptorIndex.fetch_add(_uCount);
	zenAssertMsg( uIndex+_uCount <= muFrameDescriptorCount, "Ran out of resource descriptors for this frame");
	return DescriptorRangeSRV(maFrameDescriptorSRV[muFrameRendered%kuFrameBufferCount], uIndex, _uCount);
}


void QueryHeapRingbuffer_DX12::QueryStart(GPUContext& _Context,  zU64 _uQueryIndex)
{
	_Context.GetCommandList()->BeginQuery(mrDXQueryHeap.Get(), meQueryType, (UINT)(_uQueryIndex%muQueryCount));			
}

void QueryHeapRingbuffer_DX12::QueryEnd(GPUContext& _Context, zU64 _uQueryIndex)
{
	_Context.GetCommandList()->EndQuery(mrDXQueryHeap.Get(), meQueryType, (UINT)(_uQueryIndex%muQueryCount));
}

bool QueryHeapRingbuffer_DX12::IsQueryReady(zU64 _uQueryIndex)
{
	if( _uQueryIndex >= muIndexFence )
		muIndexFence = mrDXFence->GetCompletedValue();
	return _uQueryIndex < muIndexFence;
}

template<typename TResultType> 
const TResultType& QueryHeapRingbuffer_DX12::QueryResult(zU64 _uQueryIndex) 
{
	zenAssert(sizeof(TResultType)==muQueryDataSize);
	zenAssert(IsQueryReady(_uQueryIndex));
	
	//----------------------------------------------------------------------------------------------
	// If query data hasn't been read back from GPU yet, do it
	//----------------------------------------------------------------------------------------------
	if( _uQueryIndex >= muIndexResolved )
	{
		D3D12_RANGE ReadRange			= {};
		D3D12_RANGE EmptyRange			= {};
		zU8* pDataRead					= nullptr;
		ReadRange.Begin					= 0;
		ReadRange.End					= muQueryCount*muQueryDataSize;
		HRESULT hr						= mrDXQueryResources->Map(0, &ReadRange, reinterpret_cast<void**>(&pDataRead));
		if( hr == S_OK )
		{		
			const UINT uStart				= (UINT)(muIndexResolved % muQueryCount);
			const UINT uResolveCount		= (UINT)(muIndexFence - muIndexResolved);
			const UINT uResolveCountToEnd	= (UINT)zenMath::Min((zU64)uResolveCount, muQueryCount-uStart);
			zenMem::Copy(&maResultData[uStart*muQueryDataSize], &pDataRead[uStart*muQueryDataSize], uResolveCountToEnd*muQueryDataSize );
			if( uResolveCount != uResolveCountToEnd )
				zenMem::Copy(&maResultData[0], &pDataRead[0], (uResolveCount-uResolveCountToEnd)*muQueryDataSize );
			mrDXQueryResources->Unmap(0, &EmptyRange);
			muIndexResolved					= muIndexFence;
		}
	}
	
	// Return result
	return *reinterpret_cast<TResultType*>(&maResultData[(_uQueryIndex%muQueryCount)*muQueryDataSize]);
}

}
