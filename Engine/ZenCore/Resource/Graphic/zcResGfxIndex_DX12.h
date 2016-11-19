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
	void										Update(zU8* _pData, zUInt _uOffset = 0, zUInt _uSize = 0xFFFFFFFFFFFFFFFF);

	ID3D11Buffer*								mpIndiceBuffer;
	typedef zcExp::ExporterGfxIndexDX12_DX12	RuntimeExporter;
};
class GfxIndexHAL : public GfxIndexHAL_DX12{};

}

#endif
