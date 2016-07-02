#include "zcCore.h"

namespace zcRes
{

bool GfxIndex_DX11::Initialize()
{
	ZENAssert(mrResourceData.IsValid());

	//! @todo Missing: configure resource creations flags
	//D3D11_USAGE eUsage(D3D11_USAGE_DEFAULT);
	//zUInt uCpuAccess(0);
	D3D11_USAGE eUsage(D3D11_USAGE_DYNAMIC);
	UINT uCpuAccess(D3D11_CPU_ACCESS_WRITE);
	D3D11_BUFFER_DESC IndexDesc;
	IndexDesc.ByteWidth				= mrResourceData->maIndices.SizeMem();
	IndexDesc.Usage					= eUsage;
	IndexDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	IndexDesc.CPUAccessFlags		= uCpuAccess;
	IndexDesc.MiscFlags				= 0;
	IndexDesc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem				= mrResourceData->maIndices.First();
	InitData.SysMemPitch			= 0;
	InitData.SysMemSlicePitch		= 0;
	HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateBuffer(&IndexDesc, &InitData, &mrResourceData->mpIndiceBuffer);

	return SUCCEEDED(hr);
}

const zArrayStatic<zU8>& GfxIndex_DX11::GetIndices()const
{
	return mrResourceData->maIndices;
}

zU8	GfxIndex_DX11::GetIndiceSize()const
{
	return mrResourceData->muIndiceSize;
}

void GfxIndex_DX11::Update( zU8* _pData, zUInt _uOffset, zUInt _uSize )
{
	if (mrResourceData->mpIndiceBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE mapRes;
		_uOffset		= zenMath::Min(_uOffset,	(zUInt)mrResourceData->muIndiceCount );
		_uSize			= zenMath::Min(_uSize,		(zUInt)mrResourceData->maIndices.SizeMem() - _uOffset);
		HRESULT result = zcMgr::GfxRender.DX11GetDeviceContext()->Map(mrResourceData->mpIndiceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
		memcpy( (zU8*)mapRes.pData, _pData, _uSize ); //! @todo Urgent : offset support, and not use map/unmap
		zcMgr::GfxRender.DX11GetDeviceContext()->Unmap(mrResourceData->mpIndiceBuffer, 0);
	}
}

}