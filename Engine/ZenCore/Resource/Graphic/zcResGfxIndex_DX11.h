#pragma once

namespace zcRes
{

class GfxIndex_DX11 : public zcExp::ExportGfxIndexDX11
{
public:	
	virtual										~GfxIndex_DX11();
	bool										Initialize();
	zU8*										Lock();
	void										Unlock(const zenGfx::zScopedDrawlist& _rContext);
	
	ID3D11Buffer*								mpIndiceBuffer;
	zU8*										mpLockData = nullptr; //! @todo safe support multi threading (lock return context?)
	typedef zcExp::ExporterGfxIndexDX11_DX11	RuntimeExporter;
};

}
