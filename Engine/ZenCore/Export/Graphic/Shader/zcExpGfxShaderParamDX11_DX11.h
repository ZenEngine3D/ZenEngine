#pragma once
#ifndef __zCore_Exp_GfxShaderParamDX11_DX11_h__
#define __zCore_Exp_GfxShaderParamDX11_DX11_h__

namespace zcExp
{
	class ExporterGfxShaderParamDX11_DX11 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxShaderParamDX11_DX11, ExporterBase)
	public:
	typedef zEngineRef<ExportGfxShaderParamDX11>	ExportResultRef; //todo use proper ref class
													ExporterGfxShaderParamDX11_DX11(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool								ExportStart();
		ExportResultRef								mrExport;
	};
}

#endif
