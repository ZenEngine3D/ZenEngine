#pragma once

namespace zcRes
{

class GfxBuffer_DX11 : public zcExp::ExportGfxBuffer
{
public:	
	virtual										~GfxBuffer_DX11();
	bool										Initialize();
	void*										Lock();
	void										Unlock(const zenGfx::zCommandList& _rContext);
	
	zU8*										mpLockData = nullptr; //! @todo safe support multi threading (lock return context?)
	ID3D11Buffer*								mpBuffer;
	ID3D11ShaderResourceView*					mpSRV;
	ID3D11UnorderedAccessView*					mpUAV;
	typedef zcExp::ExporterGfxBuffer_DX11		RuntimeExporter;
};

}
