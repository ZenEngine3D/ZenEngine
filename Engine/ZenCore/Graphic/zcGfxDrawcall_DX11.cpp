#include "zcCore.h"

namespace zcGfx
{


//=================================================================================================
// DRAW COMMAND UPDATE INDEX BUFFER
//=================================================================================================
zEngineRef<Command>	CommandUpdateIndexDX11::Create(const zcRes::GfxIndexRef& _rIndex, zU8* _pData, zUInt _uOffset, zUInt _uSize)
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateIndex", sizeof(CommandUpdateIndexDX11), 128, 128);
	CommandUpdateIndexDX11* pCmdUpdateIndex = zenNew(&sMemPool) CommandUpdateIndexDX11;
	
	_uOffset							= 0;//zenMath::Min(_uOffset, (zUInt)muIndiceCount); //! @todo Urgent support partial updates
	_uSize								= zenMath::Min(_uSize, (zUInt)_rIndex.HAL()->maIndices.SizeMem() - _uOffset);

	pCmdUpdateIndex->mrIndex			= _rIndex;
	pCmdUpdateIndex->mpData				= _pData;
	pCmdUpdateIndex->muOffset			= _uOffset;
	pCmdUpdateIndex->muSize				= _uSize;
	pCmdUpdateIndex->SetSortKeyDataUpdate(_rIndex.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateIndexDX11::Invoke()
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateIndex);
	zcRes::GfxIndexHAL_DX11* pIndexDX11 = mrIndex.HAL();
	//mrIndex->Update(mpData, muOffset, muSize);	
	zenAssert(pIndexDX11->mpIndiceBuffer);	
	D3D11_MAPPED_SUBRESOURCE mapRes;
	
	HRESULT result	= zcMgr::GfxRender.DX11GetDeviceContext()->Map(pIndexDX11->mpIndiceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	memcpy((zU8*)mapRes.pData, mpData, muSize); 
	zcMgr::GfxRender.DX11GetDeviceContext()->Unmap(pIndexDX11->mpIndiceBuffer, 0);

	zenDelnullptrArray( mpData );
}

//=================================================================================================
// DRAW COMMAND UPDATE BUFFER
//=================================================================================================
zEngineRef<Command>	CommandUpdateBufferDX11::Create(const zcRes::GfxBufferRef& _rBuffer, zU8* _pUpdateData, zUInt _uOffset, zUInt _uSize)
{
	static zenMem::zAllocatorPool sMemPool("Pool CommandUpdateBuffer", sizeof(CommandUpdateBufferDX11), 128, 128);
	CommandUpdateBufferDX11* pCmdUpdateIndex	= zenNew(&sMemPool) CommandUpdateBufferDX11;
	_uOffset		= 0; //! @todo 2 support partial updates
	_uSize			= zenMath::Min(_uSize, (zUInt)_rBuffer.HAL()->muElementCount*_rBuffer.HAL()->muElementStride - _uOffset);

	pCmdUpdateIndex->mrBuffer					= _rBuffer;
	pCmdUpdateIndex->mpUpdateData				= _pUpdateData;
	pCmdUpdateIndex->muOffset					= _uOffset;
	pCmdUpdateIndex->muSize						= _uSize;
	pCmdUpdateIndex->SetSortKeyDataUpdate(_rBuffer.GetResID().GetHashID());
	return pCmdUpdateIndex;
}

void CommandUpdateBufferDX11::Invoke()
{
	zcPerf::EventGPUCounter::Create(zcPerf::EventGPUCounter::keType_UpdateBuffer);	
	//! @todo optim select proper write type
	D3D11_MAPPED_SUBRESOURCE mapRes;	
	HRESULT result = zcMgr::GfxRender.DX11GetDeviceContext()->Map(mrBuffer.HAL()->mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	memcpy((zU8*)mapRes.pData, mpUpdateData, muSize); 
	zcMgr::GfxRender.DX11GetDeviceContext()->Unmap(mrBuffer.HAL()->mpBuffer, 0);
	zenDelnullptrArray(mpUpdateData);
	
}

}
