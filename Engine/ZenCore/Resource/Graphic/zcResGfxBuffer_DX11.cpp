#include "zcCore.h"

namespace zcRes
{

GfxBuffer_DX11::~GfxBuffer_DX11()
{
	if( mpBuffer )
	{
		mpBuffer->Release();
		mpBuffer = nullptr;
	}
}

bool GfxBuffer_DX11::Initialize()
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
	HRESULT hr = zcMgr::GfxRender.GetDevice()->CreateBuffer(&BufferDesc, maData.Count() ? &InitData : nullptr, &mpBuffer);	
	if( SUCCEEDED(hr) )
		hr = zcMgr::GfxRender.GetDevice()->CreateShaderResourceView(mpBuffer, nullptr, &mpSRV);

	//! @todo optim only create if gpu write active
	if( SUCCEEDED(hr) && (BufferDesc.MiscFlags & D3D11_BIND_UNORDERED_ACCESS) )
		hr = zcMgr::GfxRender.GetDevice()->CreateUnorderedAccessView(mpBuffer, nullptr, &mpUAV);

	return SUCCEEDED(hr);
}

void* GfxBuffer_DX11::Lock()
{
	zenAssertMsg(mpLockData==nullptr, "Need to unlock buffer before locking it again");
	mpLockData = zenNewDefault zU8[ muElementCount*muElementStride ]; //!todo 2 perf Use ring buffer instead
	return mpLockData;
}

void GfxBuffer_DX11::Unlock(const zenGfx::zScopedDrawlist& _rContext)
{
	//! @todo 2 Update Cpu copy at frame end
	zenAssertMsg(mpLockData!=nullptr, "Need to lock buffer before unlocking it");
	zEngineRef<zcGfx::Command> rCommand	= zcGfx::CommandUpdateBuffer_DX11::Create(reinterpret_cast<GfxBuffer*>(this), mpLockData);
	mpLockData							= nullptr;
	_rContext->AddCommand(rCommand.Get());
}

}