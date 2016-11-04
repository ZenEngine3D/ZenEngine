#include "zcCore.h"

namespace zcRes
{

GfxBufferHAL_DX11::~GfxBufferHAL_DX11()
{
	if( mpBuffer )
	{
		mpBuffer->Release();
		mpBuffer = nullptr;
	}
}

bool GfxBufferHAL_DX11::Initialize()
{
	//! @todo Urgent configure resource creations flags (for all buffer type)
	D3D11_BUFFER_DESC BufferDesc; 
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));	
	BufferDesc.ByteWidth				= muElementCount*muElementStride;
	BufferDesc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	
	//BufferDesc.BindFlags				= D3D11_BIND_UNORDERED_ACCESS|D3D11_BIND_SHADER_RESOURCE;  //! @todo optim only create if gpu write active
	BufferDesc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;  //Can't support uav while cpu access on
	BufferDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;	//! @todo Optim implement useage flags
	BufferDesc.Usage					= D3D11_USAGE_DYNAMIC;		//! @todo Optim implement useage flags
	BufferDesc.StructureByteStride		= muElementStride;
	
	D3D11_SUBRESOURCE_DATA InitData;
	zenAssert( maData.Count()==0 || maData.Count() == BufferDesc.ByteWidth);
	InitData.pSysMem					= maData.Count() ? maData.First() : nullptr;
	InitData.SysMemPitch				= 0;
	InitData.SysMemSlicePitch			= 0;
	
	mpSRV = nullptr;
	mpUAV = nullptr;
	HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateBuffer(&BufferDesc, maData.Count() ? &InitData : nullptr, &mpBuffer);	
	if( SUCCEEDED(hr) )
		hr = zcMgr::GfxRender.DX11GetDevice()->CreateShaderResourceView(mpBuffer, nullptr, &mpSRV);

	//! @todo optim only create if gpu write active
	if( SUCCEEDED(hr) && (BufferDesc.MiscFlags & D3D11_BIND_UNORDERED_ACCESS) )
		hr = zcMgr::GfxRender.DX11GetDevice()->CreateUnorderedAccessView(mpBuffer, nullptr, &mpUAV);

	return SUCCEEDED(hr);
}
	
void GfxBufferHAL_DX11::Update(zU8* _pData, zUInt _uOffset, zUInt _uSize)
{
	//! @todo optim select proper write type
//	zenAssert(maStreamBuffer[0] != nullptr);
	D3D11_MAPPED_SUBRESOURCE mapRes;
	_uOffset		= 0; //! @todo Urgent support partial updates
	_uSize			= zenMath::Min(_uSize, (zUInt)muElementCount*muElementStride - _uOffset);
	HRESULT result	= zcMgr::GfxRender.DX11GetDeviceContext()->Map(mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	memcpy((zU8*)mapRes.pData, _pData, _uSize); 
	zcMgr::GfxRender.DX11GetDeviceContext()->Unmap(mpBuffer, 0);
}

}