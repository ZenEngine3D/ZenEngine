#include "zcCore.h"

//SF DX12 

namespace zcRes
{

GfxBufferHAL_DX12::~GfxBufferHAL_DX12()
{
}

bool GfxBufferHAL_DX12::Initialize()
{
	CD3DX12_HEAP_PROPERTIES DefaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC BufferDesc = CD3DX12_RESOURCE_DESC::Buffer((UINT)muElementCount*muElementStride);
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
			&DefaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&BufferDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&mrBuffer));

	// Create view on Buffer resource
	D3D12_SHADER_RESOURCE_VIEW_DESC SrvDesc = {};
	SrvDesc.Format						= DXGI_FORMAT_UNKNOWN;
	SrvDesc.ViewDimension				= D3D12_SRV_DIMENSION_BUFFER;
    SrvDesc.Shader4ComponentMapping		= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SrvDesc.Buffer.FirstElement			= 0;
	SrvDesc.Buffer.NumElements			= muElementCount;
	SrvDesc.Buffer.StructureByteStride	= muElementStride;
	SrvDesc.Buffer.Flags				= D3D12_BUFFER_SRV_FLAG_NONE;	    
	mBufferView							= zcGfx::DescriptorSRV_UAV_CBV::Allocate();
	zcMgr::GfxRender.GetDevice()->CreateShaderResourceView(mrBuffer.Get(), &SrvDesc, mBufferView.GetHandle());

	// Send Resource update command
	void* pUploadData = Lock();
	if( !pUploadData )
		return false;

	zenMem::Copy( reinterpret_cast<zU8*>(pUploadData), maData.First(), maData.SizeMem() );
	Unlock(zenGfx::zContext::GetFrameContext());
	return true;
}

void* GfxBufferHAL_DX12::Lock()
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
	if (FAILED(hr) )
		return nullptr;
	
	// Get cpu memory pointer
	void* pData;
    hr = mrLockData->Map(0, NULL, &pData);
    if (FAILED(hr) )
		return nullptr;

	return pData;
}

void GfxBufferHAL_DX12::Unlock(const zenGfx::zContext& _rContext)
{
	zenAssert(mrLockData.Get() != nullptr);
	mrLockData->Unmap(0, NULL);

	//DirectXComRef<ID3D12Resource>& rTemphandle	= zcMgr::GfxRender.GetTempResourceHandle();
	//rTemphandle									= mrLockData;	//Keep the memory alive until gpu is done with it (temp handle valid 2 frames)

	zcRes::GfxBufferRef rBuffer					= reinterpret_cast<zcRes::GfxBuffer*>(this);
	zEngineRef<zcGfx::Command> rCommand			= zcGfx::CommandUpdateBufferDX12::Create(rBuffer, 0, maData.SizeMem());
	mrLockData									= nullptr;

	_rContext->AddCommand(rCommand.Get());
}

}