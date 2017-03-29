#include "zcCore.h"

zenPreventEmptyFileWarning

namespace zcGfx
{

GpuMemoryResStandard::GpuMemoryResStandard()
{
	mView = zcGfx::DescriptorSRV_UAV_CBV::Allocate();
}

GpuMemoryResStandard::~GpuMemoryResStandard()
{
	mView.Free();
}

/*
void* GpuBuffer::Lock(zUInt _uMipCount)
{
	zenAssert(mrUpload.Get() == nullptr);
	void*					pData			= nullptr;
	D3D12_RESOURCE_DESC		TextureDesc		= mrResource->GetDesc();    
	D3D12_HEAP_PROPERTIES	UploadHeap		= {D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1};
	muUploadOffset							= 0;
	zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&TextureDesc, 0, (UINT)_uMipCount, 0, nullptr, nullptr, nullptr, &muUploadSize);
	
	D3D12_RESOURCE_DESC UploadDesc			= {};
	UploadDesc.Dimension					= D3D12_RESOURCE_DIMENSION_BUFFER;
	UploadDesc.Alignment					= 0;
	UploadDesc.Width						= muUploadSize;
	UploadDesc.Height						= 1;
	UploadDesc.DepthOrArraySize				= 1;
	UploadDesc.MipLevels					= (UINT)_uMipCount;
	UploadDesc.Format						= DXGI_FORMAT_UNKNOWN;
	UploadDesc.SampleDesc					= {1,0};
	UploadDesc.Layout						= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	UploadDesc.Flags						= D3D12_RESOURCE_FLAG_NONE;

	// Create the GPU upload buffer.
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
						&UploadHeap,
						D3D12_HEAP_FLAG_NONE,
						&UploadDesc,
						D3D12_RESOURCE_STATE_GENERIC_READ,
						nullptr,
						IID_PPV_ARGS(&mrUpload));

	if( SUCCEEDED(hr) )
	{		
		//zSetGfxResourceName(mrUpload, mResID, L"UploadData");		//! @todo 0 figure out name
		hr = mrUpload->Map(0, NULL, &pData);
		if (FAILED(hr) )
			pData = nullptr;
	}

	return pData;
}

void GpuBuffer::Unlock(const zcRes::ResourceAnyRef& _rParentResource, const zenGfx::zScopedDrawlist& _rContext)
{
	zenAssert(mrUpload.Get() != nullptr);
	mrUpload->Unmap(0, NULL);
	zEngineRef<zcGfx::Command> rCommand	= zcGfx::CommandUpdateResourceBuffer_DX12::Create(_rParentResource, *this);
	mrUpload							= nullptr;
	muUploadSize						= 0;
	muUploadOffset						= 0;
	_rContext->AddCommand(rCommand.Get());
}
*/
}
