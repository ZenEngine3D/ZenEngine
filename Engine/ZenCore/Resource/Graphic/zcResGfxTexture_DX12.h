#pragma once

namespace zcRes
{
	class GfxTexture2D_DX12 : public zcExp::ExportGfxTexture2D
	{
	zenClassDeclare(GfxTexture2D_DX12, zcExp::ExportGfxTexture2D);
	public:
		virtual											~GfxTexture2D_DX12();
		bool											Initialize();
		void*											Lock();	//!< @todo 2 Support mipmap
		void											Unlock(const zenGfx::zScopedDrawlist& _rContext);				
		zcGfx::GpuMemoryResStandard						mResource;
		//! @todo 3 Add support for UAV and Stencil view
		typedef zcExp::ExporterGfxTexture2DDX12_DX12	RuntimeExporter;
	};
}
