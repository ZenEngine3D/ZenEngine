#include "zcCore.h"

//SF DX12 

namespace zcRes
{
//! @todo 0
void GfxShaderResourceDescRef::UpdateGPUBuffer()
{	
	eResType ResType	= mpResource ? mpResource->mResID.GetType() : keResType__Invalid;
	mpGPUBuffer			= nullptr;
	
	if( ResType == keResType_GfxBuffer)
	{
		zcRes::GfxBufferRef rBuffer = *this;
		mpGPUBuffer = &rBuffer.HAL()->mResource;
	}
	else if( ResType == keResType_GfxCBuffer )
	{
		zcRes::GfxCBufferRef rCBuffer = *this;
//		mpGPUBuffer = &rCBuffer.HAL()->mResource;
	}
	else if( ResType == keResType_GfxTexture2D ) 
	{
		zcRes::GfxTexture2DRef rTexture = *this;
		mpGPUBuffer = &rTexture.HAL()->mResource;
	}
}

const GfxShaderResourceDescRef&	GfxShaderResourceDescRef::operator=(const GfxShaderResourceRef& _rResource)
{
	Super::operator=(_rResource);
	UpdateGPUBuffer();
	return *this;
}

const GfxShaderResourceDescRef&	GfxShaderResourceDescRef::operator=(const GfxShaderResourceDescRef& _rResource)
{
	Super::operator=(_rResource);
	UpdateGPUBuffer();
	return *this;
}

const GfxShaderResourceDescRef&	GfxShaderResourceDescRef::operator=(zenRes::zExportData* _pResource)
{
	Super::operator=(_pResource);
	UpdateGPUBuffer();
	return *this;
}


//##################################################################################################

GfxBuffer_DX12::~GfxBuffer_DX12()
{
}

bool GfxBuffer_DX12::Initialize()
{
	bool bUploadData	= maData.SizeMem() > 0;
	mResource.meState	= bUploadData ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	HRESULT hr			= zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
									&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
									D3D12_HEAP_FLAG_NONE,
									&CD3DX12_RESOURCE_DESC::Buffer((UINT)muElementCount*muElementStride),
									mResource.meState,
									nullptr,
									IID_PPV_ARGS(&mResource.mrResource));
	if(FAILED(hr))
		return false;
	
	zSetGfxResourceName(mResource.mrResource, mResID, nullptr);

	// Create view on Buffer resource
	D3D12_SHADER_RESOURCE_VIEW_DESC SrvDesc = {};
	SrvDesc.Format						= DXGI_FORMAT_UNKNOWN;
	SrvDesc.ViewDimension				= D3D12_SRV_DIMENSION_BUFFER;
    SrvDesc.Shader4ComponentMapping		= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SrvDesc.Buffer.FirstElement			= 0;
	SrvDesc.Buffer.NumElements			= muElementCount;
	SrvDesc.Buffer.StructureByteStride	= muElementStride;
	SrvDesc.Buffer.Flags				= D3D12_BUFFER_SRV_FLAG_NONE;	    
	mResource.mView						= zcGfx::DescriptorSRV_UAV_CBV::Allocate();
	zcMgr::GfxRender.GetDevice()->CreateShaderResourceView(mResource.mrResource.Get(), &SrvDesc, mResource.mView.GetCpuHandle());

	// Send Resource update command
	if( bUploadData )
	{
		void* pUploadData = Lock();
		if( !pUploadData )
			return false;

		zenMem::Copy( reinterpret_cast<zU8*>(pUploadData), maData.First(), maData.SizeMem() );
		Unlock(zenGfx::zScopedDrawlist::GetFrameContext());
	}
	return true;
}

void* GfxBuffer_DX12::Lock()
{
	zenAssert(mResource.mrUpload.Get() == nullptr);

	// Allocate temp memory to upload data to buffer
	D3D12_RESOURCE_DESC BufferDesc = mResource.mrResource->GetDesc();
	zcMgr::GfxRender.GetDevice()->GetCopyableFootprints(&BufferDesc, 0, 1, 0, nullptr, nullptr, nullptr, &mResource.muUploadSize);
	
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(mResource.muUploadSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&mResource.mrUpload));
	if (FAILED(hr) )
		return nullptr;
	
	zSetGfxResourceName(mResource.mrUpload, mResID, L"Buffer UploadData");
	mResource.muUploadOffset = 0;

	// Get cpu memory pointer
	void* pData;
    hr = mResource.mrUpload->Map(0, NULL, &pData);
    if (FAILED(hr) )
		return nullptr;

	return pData;
}

void GfxBuffer_DX12::Unlock(const zenGfx::zScopedDrawlist& _rContext)
{
	zenAssert(mResource.mrUpload.Get() != nullptr);
	mResource.mrUpload->Unmap(0, NULL);

	zcRes::GfxBufferRef rBuffer	= reinterpret_cast<zcRes::GfxBuffer*>(this);
	zcGfx::CommandUpdateBuffer_DX12::Add(_rContext, rBuffer, mResource.muUploadOffset, mResource.muUploadSize );
	mResource.mrUpload			= nullptr;
	mResource.muUploadSize		= 0;
	mResource.muUploadOffset	= 0;
}

}