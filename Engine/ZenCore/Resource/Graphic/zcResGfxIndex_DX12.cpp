#include "zcCore.h"

//SF DX12

namespace zcRes
{

GfxIndex_DX12::~GfxIndex_DX12()
{

}

bool GfxIndex_DX12::Initialize()
{
	bool bUploadData	= maIndices.SizeMem() > 0;
	meResourceState		= bUploadData ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(maIndices.SizeMem()),
					meResourceState,
					nullptr,
					IID_PPV_ARGS(&mrResource));
	
	if( FAILED(hr) )
		return false;

	zSetGfxResourceName(mrResource, mResID, nullptr);
	
	// Create resource update command
	if( bUploadData )
	{
		void* pUploadData = Lock();
		if( pUploadData )
		{
			zenMem::Copy( reinterpret_cast<zU8*>(pUploadData), maIndices.First(), maIndices.SizeMem() );
			Unlock(zenGfx::zContext::GetFrameContext());
			return true;
		}
	}
	return false;
}

zU8* GfxIndex_DX12::Lock()
{
	zenAssert(mrResourceUpload.Get() == nullptr);

	// Allocate temp memory to upload data to buffer
	UINT64 uUploadBufferSize		= 0;
	D3D12_RESOURCE_DESC BufferDesc	= mrResource->GetDesc();
	zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&BufferDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uUploadBufferSize);
	
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(uUploadBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&mrResourceUpload));
	
	zSetGfxResourceName(mrResourceUpload, mResID, L"Index UploadData");

	if( SUCCEEDED(hr) )
	{		
		void* pData;		
		hr = mrResourceUpload->Map(0, NULL, &pData);	// Get cpu memory pointer
		if (SUCCEEDED(hr) )
			return reinterpret_cast<zU8*>(pData);
	}
	return nullptr;
}

void GfxIndex_DX12::Unlock(const zenGfx::zContext& _rContext)
{
	zenAssert(mrResourceUpload.Get() != nullptr);
	mrResourceUpload->Unmap(0, NULL);

	//! @todo 2 support partial updates
	zcRes::GfxIndexRef rBuffer					= reinterpret_cast<zcRes::GfxIndex*>(this);
	zEngineRef<zcGfx::Command> rCommand			= zcGfx::CommandUpdateIndex_DX12::Create(rBuffer, 0, maIndices.SizeMem());
	mrResourceUpload							= nullptr;

	_rContext->AddCommand(rCommand.Get());
}

}