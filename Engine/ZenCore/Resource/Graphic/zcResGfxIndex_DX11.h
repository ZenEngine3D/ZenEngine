#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_DX11_h__
#define __zCore_Res_Gfx_GfxIndex_DX11_h__

namespace zcRes
{

class GfxIndexHAL_DX11 : public zcExp::ExportGfxIndexDX11
{
public:	
	virtual										~GfxIndexHAL_DX11();
	bool										Initialize();
	zU8*										Lock();
	void										Unlock(const zenGfx::zContext& _rContext);
	
	ID3D11Buffer*								mpIndiceBuffer;
	zU8*										mpLockData = nullptr; //! @todo safe support multi threading (lock return context?)
	typedef zcExp::ExporterGfxIndexDX11_DX11	RuntimeExporter;
};
class GfxIndexHAL : public GfxIndexHAL_DX11{};

}

#endif
