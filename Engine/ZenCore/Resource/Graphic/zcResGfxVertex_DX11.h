#pragma once
#ifndef __zCore_Res_Gfx_GfxVertex_DX11_h__
#define __zCore_Res_Gfx_GfxVertex_DX11_h__

namespace zcRes
{

class GfxVertexHAL_DX11 : public zcExp::ExportGfxVertexDX11
{
public:
	virtual										~GfxVertexHAL_DX11();
	bool										Initialize();
	void										Update(zU8* _pData, zUInt _uOffset = 0, zUInt _uSize = 0xFFFFFFFFFFFFFFFF);
	zArrayStatic<ID3D11Buffer*>					maStreamBuffer;	//!< D3D11 Buffer object
	zArrayStatic<UINT>							maStreamStride;	//!< Stride between 2 vertices of a buffer //todo move exo
	typedef zcExp::ExporterGfxVertexDX11_DX11	RuntimeExporter;
};
class GfxVertexHAL : public GfxVertexHAL_DX11{};

}

#endif
