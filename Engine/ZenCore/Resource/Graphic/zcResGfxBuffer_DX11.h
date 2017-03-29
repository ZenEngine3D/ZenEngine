#pragma once
#ifndef __zCore_Res_Gfx_GfxBuffer_DX11_h__
#define __zCore_Res_Gfx_GfxBuffer_DX11_h__

namespace zcRes
{

class GfxBuffer_DX11 : public zcExp::ExportGfxBuffer
{
public:	
	virtual										~GfxBuffer_DX11();
	bool										Initialize();
	void*										Lock();
	void										Unlock(const zenGfx::zScopedDrawlist& _rContext);
	
	zU8*										mpLockData = nullptr; //! @todo safe support multi threading (lock return context?)
	ID3D11Buffer*								mpBuffer;
	ID3D11ShaderResourceView*					mpSRV;
	ID3D11UnorderedAccessView*					mpUAV;
	typedef zcExp::ExporterGfxBuffer_DX11		RuntimeExporter;
};

}

#endif
