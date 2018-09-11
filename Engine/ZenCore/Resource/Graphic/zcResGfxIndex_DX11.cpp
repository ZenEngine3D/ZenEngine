#include "zcCore.h"

namespace zcRes
{

GfxIndex_DX11::~GfxIndex_DX11()
{
	if( mpIndiceBuffer )
	{
		mpIndiceBuffer->Release();
		mpIndiceBuffer = nullptr;
	}
}

bool GfxIndex_DX11::Initialize()
{
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
	HRESULT hr						= zcMgr::GfxRender.GetDevice()->CreateBuffer(&IndexDesc, &InitData, &mpIndiceBuffer);

	return SUCCEEDED(hr);
}

zU8* GfxIndex_DX11::Lock()
{
	zenAssertMsg(mpLockData==nullptr, "Need to unlock buffer before locking it again");

	//! @todo 3 Optim lock type and copy data if needed
	//! @todo 3 Optim Use ring buffer for this	
	//! @todo 3 Clean Design MemBuffer class to handle data copy between cpu/gpu
	//! @todo 3 Optim return directx11 map pointer directly
	//					Ref: https://developer.nvidia.com/sites/default/files/akamai/gamedev/files/gdc12/Efficient_Buffer_Management_McDonald.pdf
	mpLockData = zenNew zU8[ maIndices.SizeMem() ];		
	return mpLockData;
}

void GfxIndex_DX11::Unlock(const zenGfx::zCommandList& _rContext)
{
	//! @todo Urgent Update Cpu copy at frame end
	zenAssertMsg(mpLockData != nullptr, "Need to lock buffer before unlocking it");
	zcRes::GfxIndexRef rIndex			= reinterpret_cast<GfxIndex*>(this);
	zEngineRef<zcGfx::Command> rCommand = zcGfx::CommandUpdateIndex_DX11::Create( rIndex, mpLockData );
	mpLockData							= nullptr;
	_rContext->AddCommand(rCommand.Get());
}

}
