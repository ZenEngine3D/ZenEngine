#pragma once
#ifndef __zCore_Res_Gfx_GfxBuffer_DX12_h__
#define __zCore_Res_Gfx_GfxBuffer_DX12_h__
//SF DX12

namespace zcRes
{

class GfxBufferHAL_DX11 : public zcExp::ExportGfxBuffer
{
public:	
	virtual										~GfxBufferHAL_DX11();
	bool										Initialize();
	void										Update(zU8* _pData, zUInt _uOffset = 0, zUInt _uSize = 0xFFFFFFFFFFFFFFFF);

	ID3D11Buffer*								mpBuffer;
	ID3D11ShaderResourceView*					mpSRV;
	ID3D11UnorderedAccessView*					mpUAV;
	typedef zcExp::ExporterGfxBuffer_DX11		RuntimeExporter;
};
class GfxBufferHAL : public GfxBufferHAL_DX11{};

}

#endif
