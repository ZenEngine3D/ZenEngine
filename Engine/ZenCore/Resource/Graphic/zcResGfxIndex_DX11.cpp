#include "zcCore.h"

namespace zcRes
{
	GfxIndex_DX11::GfxIndex_DX11()
	{
		mInstanceInfo.mpIndiceBuffer	= NULL;
	}

	bool GfxIndex_DX11::ResourceInit()
	{		
		//! @todo Missing: configure resource creations flags
		//D3D11_USAGE eUsage(D3D11_USAGE_DEFAULT);
		//zUInt uCpuAccess(0);
		D3D11_USAGE eUsage(D3D11_USAGE_DYNAMIC);
		UINT uCpuAccess(D3D11_CPU_ACCESS_WRITE);
		D3D11_BUFFER_DESC IndexDesc;
		IndexDesc.ByteWidth				= mInstanceInfo.mSerial.maIndices.Size();
		IndexDesc.Usage					= eUsage;						
		IndexDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;		
		IndexDesc.CPUAccessFlags		= uCpuAccess;					
		IndexDesc.MiscFlags				= 0;	
		IndexDesc.StructureByteStride	= 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem				= mInstanceInfo.mSerial.maIndices.First();
		InitData.SysMemPitch			= 0;
		InitData.SysMemSlicePitch		= 0;
		HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateBuffer( &IndexDesc, &InitData, &mInstanceInfo.mpIndiceBuffer );
		return SUCCEEDED(hr);
	}

	GfxIndex_DX11::~GfxIndex_DX11()
	{
		if( mInstanceInfo.mpIndiceBuffer )
			mInstanceInfo.mpIndiceBuffer->Release();
	}

	zU8* GfxIndex_DX11::Lock()
	{
		//! @todo Missing: Stream index parameter
		//! @todo Missing: Specify access type
		if( Get().mpIndiceBuffer )
		{
			D3D11_MAPPED_SUBRESOURCE mapRes;
			EMgr::GfxRender.DX11GetDeviceContext()->Map(Get().mpIndiceBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
			return (zU8*)mapRes.pData;
		}
		return NULL;
	}

	void GfxIndex_DX11::Unlock()
	{
		if( Get().mpIndiceBuffer )
		{
			EMgr::GfxRender.DX11GetDeviceContext()->Unmap(Get().mpIndiceBuffer, NULL);
		}
	}

}