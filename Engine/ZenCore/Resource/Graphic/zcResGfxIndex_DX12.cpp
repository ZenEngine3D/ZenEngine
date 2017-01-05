#include "zcCore.h"

//SF DX12

namespace zcRes
{

GfxIndexHAL_DX12::~GfxIndexHAL_DX12()
{

}

bool GfxIndexHAL_DX12::Initialize()
{
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(maIndices.SizeMem()),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&mrBuffer));
	
	// Create resource update command
	if( SUCCEEDED(hr) )
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

zU8* GfxIndexHAL_DX12::Lock()
{
	zenAssert(mrLockData.Get() == nullptr);

	// Allocate temp memory to upload data to buffer
	UINT64 uUploadBufferSize		= 0;
	D3D12_RESOURCE_DESC BufferDesc	= mrBuffer->GetDesc();
	zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&BufferDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uUploadBufferSize);
	
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(uUploadBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&mrLockData));
	
	if( SUCCEEDED(hr) )
	{		
		void* pData;		
		hr = mrLockData->Map(0, NULL, &pData);	// Get cpu memory pointer
		if (SUCCEEDED(hr) )
			return reinterpret_cast<zU8*>(pData);
	}
	return nullptr;
}

void GfxIndexHAL_DX12::Unlock(const zenGfx::zContext& _rContext)
{
	zenAssert(mrLockData.Get() != nullptr);
	mrLockData->Unmap(0, NULL);

	//DirectXComRef<ID3D12Resource>& rTemphandle	= zcMgr::GfxRender.GetTempResourceHandle();
	//rTemphandle									= mrLockData;	//Keep the memory alive until gpu is done with it (temp handle valid 2 frames)
	//! @todo 2 support partial updates
	zcRes::GfxIndexRef rBuffer					= reinterpret_cast<zcRes::GfxIndex*>(this);
	zEngineRef<zcGfx::Command> rCommand			= zcGfx::CommandUpdateIndexDX12::Create(rBuffer, 0, maIndices.SizeMem());
	mrLockData									= nullptr;

	_rContext->AddCommand(rCommand.Get());
}

}