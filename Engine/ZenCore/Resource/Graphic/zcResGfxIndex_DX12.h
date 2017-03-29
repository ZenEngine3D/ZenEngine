#pragma once

//SF DX12
namespace zcRes
{

class GfxIndex_DX12 : public zcExp::ExportGfxIndexDX12
{
public:	
	virtual										~GfxIndex_DX12();
	bool										Initialize();
	zU8*										Lock();
	void										Unlock(const zenGfx::zScopedDrawlist& _rContext);

	zcGfx::GpuMemoryResIndex					mResource;
	typedef zcExp::ExporterGfxIndexDX12_DX12	RuntimeExporter;
};

}
