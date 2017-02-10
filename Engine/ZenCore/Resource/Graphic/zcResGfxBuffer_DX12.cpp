#include "zcCore.h"

//SF DX12 

namespace zcRes
{

GfxBuffer_DX12::~GfxBuffer_DX12()
{
}

bool GfxBuffer_DX12::Initialize()
{
	bool bUploadData	= maData.SizeMem() > 0;
	meResourceState		= bUploadData ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	HRESULT hr			= zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
									&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
									D3D12_HEAP_FLAG_NONE,
									&CD3DX12_RESOURCE_DESC::Buffer((UINT)muElementCount*muElementStride),
									meResourceState,
									nullptr,
									IID_PPV_ARGS(&mrResource));
	if(FAILED(hr))
		return false;
	
	zSetGfxResourceName(mrResource, mResID, nullptr);

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
	zcMgr::GfxRender.GetDevice()->CreateShaderResourceView(mrResource.Get(), &SrvDesc, mBufferView.GetCpuHandle());

	// Send Resource update command
	if( bUploadData )
	{
		void* pUploadData = Lock();
		if( !pUploadData )
			return false;

		zenMem::Copy( reinterpret_cast<zU8*>(pUploadData), maData.First(), maData.SizeMem() );
		Unlock(zenGfx::zContext::GetFrameContext());
	}
	return true;
}

void* GfxBuffer_DX12::Lock()
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
	if (FAILED(hr) )
		return nullptr;
	
	zSetGfxResourceName(mrResourceUpload, mResID, L"Buffer UploadData");

	// Get cpu memory pointer
	void* pData;
    hr = mrResourceUpload->Map(0, NULL, &pData);
    if (FAILED(hr) )
		return nullptr;

	return pData;
}

void GfxBuffer_DX12::Unlock(const zenGfx::zContext& _rContext)
{
	zenAssert(mrResourceUpload.Get() != nullptr);
	mrResourceUpload->Unmap(0, NULL);

	zcRes::GfxBufferRef rBuffer					= reinterpret_cast<zcRes::GfxBuffer*>(this);
	zEngineRef<zcGfx::Command> rCommand			= zcGfx::CommandUpdateBuffer_DX12::Create(rBuffer, 0, maData.SizeMem());
	mrResourceUpload							= nullptr;

	_rContext->AddCommand(rCommand.Get());
}

}