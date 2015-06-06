#include "zcCore.h"

namespace zcRes
{

GfxIndexProxy_DX11::GfxIndexProxy_DX11()
: mpIndiceBuffer(NULL)
{
}

GfxIndexProxy_DX11::~GfxIndexProxy_DX11()
{
	if( mpIndiceBuffer )
		mpIndiceBuffer->Release();
	mpIndiceBuffer = NULL;
}

bool GfxIndexProxy_DX11::Initialize(class GfxIndex& _Owner)
{
	const GfxIndex::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	//! @todo Missing: configure resource creations flags
	//D3D11_USAGE eUsage(D3D11_USAGE_DEFAULT);
	//zUInt uCpuAccess(0);
	D3D11_USAGE eUsage(D3D11_USAGE_DYNAMIC);
	UINT uCpuAccess(D3D11_CPU_ACCESS_WRITE);
	D3D11_BUFFER_DESC IndexDesc;
	IndexDesc.ByteWidth				= rResData->maIndices.Size();
	IndexDesc.Usage					= eUsage;						
	IndexDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;		
	IndexDesc.CPUAccessFlags		= uCpuAccess;					
	IndexDesc.MiscFlags				= 0;	
	IndexDesc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem				= rResData->maIndices.First();
	InitData.SysMemPitch			= 0;
	InitData.SysMemSlicePitch		= 0;
	HRESULT hr						= EMgr::GfxRender.DX11GetDevice()->CreateBuffer( &IndexDesc, &InitData, &mpIndiceBuffer );

	mePrimitiveType					= rResData->mePrimitiveType;
	meIndiceFormat					= rResData->meIndiceFormat;	
	muIndiceCount					= rResData->muIndiceCount;	
	muIndiceSize					= rResData->muIndiceSize;	
	muPrimitiveCount				= rResData->muPrimitiveCount;

	return SUCCEEDED(hr);
}

zU8* GfxIndexProxy_DX11::Lock()
{
	//! @todo Missing: Stream index parameter
	//! @todo Missing: Specify access type
	if( mpIndiceBuffer )
	{
		D3D11_MAPPED_SUBRESOURCE mapRes;
		EMgr::GfxRender.DX11GetDeviceContext()->Map(mpIndiceBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
		return (zU8*)mapRes.pData;
	}
	return NULL;
}

void GfxIndexProxy_DX11::Unlock()
{
	if( mpIndiceBuffer )
		EMgr::GfxRender.DX11GetDeviceContext()->Unmap(mpIndiceBuffer, NULL);
}

}