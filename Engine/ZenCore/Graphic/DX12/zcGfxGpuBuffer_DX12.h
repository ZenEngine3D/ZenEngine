#pragma once

namespace zcGfx
{

//! @todo 1 package these 3 members in 1 object with common methods for data upload
class GpuMemoryResource
{
zenClassDeclareNoParent(GpuMemoryResource);	
public:
	//void*							Lock(zUInt _uMipCount);
	//void							Unlock(const zcRes::ResourceAnyRef& _rParentResource, const zenGfx::zCommandContext& _rContext);												
	DirectXComRef<ID3D12Resource>	mrResource;				//!< DirectX memory mapping for this resource
	D3D12_RESOURCE_STATES			meState;				//!< Current GPU access to this resource (used for barrier updates)
	DirectXComRef<ID3D12Resource>	mrUpload;				//!< Upload memory allocated to stream-in a texture
	UINT64							muUploadSize	= 0;	//!< Memory amount of upload buffer
	UINT64							muUploadOffset	= 0;	//!< Memory offset of upload destination
};

//! @brief Standard Gpu resource (Struct buffer, ...)
class GpuMemoryResStandard : public GpuMemoryResource
{
zenClassDeclare(GpuMemoryResStandard, GpuMemoryResource);	
public:
									GpuMemoryResStandard();
									~GpuMemoryResStandard();
	DescriptorRangeSRV				mView;	
};

class GpuMemoryResIndex : public GpuMemoryResource
{
zenClassDeclare(GpuMemoryResIndex, GpuMemoryResource);	
public:
	D3D12_INDEX_BUFFER_VIEW			mView;
};

}
