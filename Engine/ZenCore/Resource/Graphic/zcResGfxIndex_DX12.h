#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_DX12_h__
#define __zCore_Res_Gfx_GfxIndex_DX12_h__
//SF DX12

namespace zcRes
{

class GfxIndexHAL_DX11 : public zcExp::ExportGfxIndexDX11
{
public:	
	virtual										~GfxIndexHAL_DX11();
	bool										Initialize();
	void										Update(zU8* _pData, zUInt _uOffset = 0, zUInt _uSize = 0xFFFFFFFFFFFFFFFF);

	ID3D11Buffer*								mpIndiceBuffer;
	typedef zcExp::ExporterGfxIndexDX11_DX11	RuntimeExporter;
};
class GfxIndexHAL : public GfxIndexHAL_DX11{};

}

#endif
