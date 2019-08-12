#pragma once

namespace zcRes
{

class GfxShaderResourceDescRef : public GfxShaderResourceRef
{
zenClassDeclare(GfxShaderResourceDescRef, GfxShaderResourceRef);
public:

	const GfxShaderResourceDescRef&				operator=(const GfxShaderResourceRef& _rResource);
	const GfxShaderResourceDescRef&				operator=(const GfxShaderResourceDescRef& _rResource);
	const GfxShaderResourceDescRef&				operator=(zenRes::zExportData* _pResource);
	zcGfx::GpuMemoryResStandard*				GetGpuBuffer()const{return mpGPUBuffer;} //! @todo 1 find good name for this and the buffer ptr
protected:
	void										UpdateGPUBuffer();
	zcGfx::GpuMemoryResStandard*				mpGPUBuffer = nullptr; 
};

class GfxBuffer_DX12 : public zcExp::ExportGfxBuffer
{
public:	
	virtual										~GfxBuffer_DX12();
	bool										Initialize();
	void*										Lock();
	void										Unlock(const zenGfx::zCommandList& _rContext);

	zcGfx::GpuMemoryResStandard					mResource;
	typedef zcExp::ExporterGfxBuffer_DX12		RuntimeExporter;
};

}
