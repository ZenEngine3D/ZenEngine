#pragma once
#ifndef __zCore_Exp_Gfx_GfxShaderBinding_DX11_h__
#define __zCore_Exp_Gfx_GfxShaderBinding_DX11_h__

namespace zcExp
{	

	class ExporterGfxShaderBindingDX11_DX11 : public ExporterGfxShaderBinding
	{
	zenClassDeclare(ExporterGfxShaderBindingDX11_DX11, ExporterGfxShaderBinding)
	public:
	typedef zEngineRef<ExportGfxShaderBinding>	ExportResultRef; //todo use proper ref class
												ExporterGfxShaderBindingDX11_DX11(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool							ExportStart();
		ExportResultRef							mrExport;
	};
}

#endif