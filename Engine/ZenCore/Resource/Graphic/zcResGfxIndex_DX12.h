#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_DX12_h__
#define __zCore_Res_Gfx_GfxIndex_DX12_h__
//SF DX12

namespace zcRes
{

class GfxIndexHAL_DX12 : public zcExp::ExportGfxIndexDX12
{
public:	
	virtual										~GfxIndexHAL_DX12();
	bool										Initialize();
	zU8*										Lock();
	void										Unlock(const zenGfx::zContext& _rContext);

	DirectXComRef<ID3D12Resource>				mrBuffer;
	DirectXComRef<ID3D12Resource>				mrLockData;
	D3D12_RESOURCE_STATES						meBufferState;
	typedef zcExp::ExporterGfxIndexDX12_DX12	RuntimeExporter;
};
class GfxIndexHAL : public GfxIndexHAL_DX12{};

}

#endif
