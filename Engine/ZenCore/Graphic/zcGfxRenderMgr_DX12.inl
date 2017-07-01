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

}
