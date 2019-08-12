#include "zcCore.h"

namespace zcRes
{

GfxIndex_DX12::~GfxIndex_DX12()
{

}

bool GfxIndex_DX12::Initialize()
{
	bool bUploadData	= maIndices.SizeMem() > 0;
	mResource.meState	= bUploadData ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(maIndices.SizeMem()),
					mResource.meState,
					nullptr,
					IID_PPV_ARGS(&mResource.mrResource));
	
	if( FAILED(hr) )
		return false;

	// Create view on Buffer resource
	mResource.mView.BufferLocation	= mResource.mrResource->GetGPUVirtualAddress();
	mResource.mView.SizeInBytes		= static_cast<UINT>(maIndices.SizeMem());
	mResource.mView.Format			= meIndiceFormat;

	zSetGfxResourceName(mResource.mrResource, mResID, nullptr);
	
	// Create resource update command
	if( bUploadData )
	{
		void* pUploadData = Lock();
		if( pUploadData )
		{
			zenMem::Copy( reinterpret_cast<zU8*>(pUploadData), maIndices.Data(), maIndices.SizeMem() );
			Unlock(zenGfx::zCommandList::GetFrameContext());
			return true;
		}
	}
	return false;
}

zU8* GfxIndex_DX12::Lock()
{
	zenAssert(mResource.mrUpload.Get() == nullptr);
	zU8* pLockData(nullptr);

	// Allocate temp memory to upload data to buffer
	UINT64 uUploadBufferSize		= 0;
	D3D12_RESOURCE_DESC BufferDesc	= mResource.mrResource->GetDesc();
	zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&BufferDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uUploadBufferSize);
	
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(uUploadBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&mResource.mrUpload));
	
	if( SUCCEEDED(hr) )
	{				
		zSetGfxResourceName(mResource.mrUpload, mResID, L"Index UploadData");
		hr			= mResource.mrUpload->Map(0, NULL, reinterpret_cast<void**>(&pLockData));	// Get cpu memory pointer
		pLockData	= SUCCEEDED(hr) ? pLockData : nullptr;
	}
	return pLockData;
}

void GfxIndex_DX12::Unlock(const zenGfx::zCommandList& _rContext)
{
	zenAssert(mResource.mrUpload.Get() != nullptr);
	mResource.mrUpload->Unmap(0, NULL);

	//! @todo 2 support partial updates
	zcRes::GfxIndexRef rBuffer = reinterpret_cast<zcRes::GfxIndex*>(this);
	zcGfx::CommandUpdateIndex_DX12::Add(_rContext, rBuffer, 0, maIndices.SizeMem());
	mResource.mrUpload = nullptr;
}

}