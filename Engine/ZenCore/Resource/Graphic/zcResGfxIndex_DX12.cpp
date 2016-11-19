#include "zcCore.h"

//SF DX12

namespace zcRes
{

GfxIndexHAL_DX12::~GfxIndexHAL_DX12()
{
	if( mpIndiceBuffer )
	{
		mpIndiceBuffer->Release();
		mpIndiceBuffer = nullptr;
	}
}

bool GfxIndexHAL_DX12::Initialize()
{
#if DISABLE_DX12
	return false;
#else
	//! @todo Missing: configure resource creations flags
	//D3D11_USAGE eUsage(D3D11_USAGE_DEFAULT);
	//zUInt uCpuAccess(0);
	D3D11_USAGE eUsage(D3D11_USAGE_DYNAMIC);
	UINT uCpuAccess(D3D11_CPU_ACCESS_WRITE);
	D3D11_BUFFER_DESC IndexDesc;
	IndexDesc.ByteWidth				= static_cast<UINT>(maIndices.SizeMem());
	IndexDesc.Usage					= eUsage;
	IndexDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	IndexDesc.CPUAccessFlags		= uCpuAccess;
	IndexDesc.MiscFlags				= 0;
	IndexDesc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem				= maIndices.First();
	InitData.SysMemPitch			= 0;
	InitData.SysMemSlicePitch		= 0;
	HRESULT hr						= zcMgr::GfxRender.DX12GetDevice()->CreateBuffer(&IndexDesc, &InitData, &mpIndiceBuffer);

	return SUCCEEDED(hr);
#endif
}

void GfxIndexHAL_DX12::Update(zU8* _pData, zUInt _uOffset, zUInt _uSize)
{
	zenAssert(mpIndiceBuffer);	
#if !DISABLE_DX12
	D3D11_MAPPED_SUBRESOURCE mapRes;
	_uOffset		= 0;//zenMath::Min(_uOffset, (zUInt)muIndiceCount); //! @todo Urgent support partial updates
	_uSize			= zenMath::Min(_uSize, (zUInt)maIndices.SizeMem() - _uOffset);
	HRESULT result	= zcMgr::GfxRender.DX12GetDeviceContext()->Map(mpIndiceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	memcpy((zU8*)mapRes.pData, _pData, _uSize); 
	zcMgr::GfxRender.DX12GetDeviceContext()->Unmap(mpIndiceBuffer, 0);
#endif
}

}