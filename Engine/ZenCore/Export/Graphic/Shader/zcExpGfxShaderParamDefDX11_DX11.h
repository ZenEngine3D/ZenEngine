#pragma once
#ifndef __zCore_Exp_GfxShaderParamDef_DX11_DX11_h__
#define __zCore_Exp_GfxShaderParamDef_DX11_DX11_h__

namespace zcExp
{
	class ExporterGfxShaderParamDefDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxShaderParamDefDX11_DX11, ExporterBase)
	public:	
	typedef zEngineRef<ExportGfxShaderParamDefDX11>	ExportResultRef; //todo use proper ref class
													ExporterGfxShaderParamDefDX11_DX11(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool								ExportStart();
		virtual bool								ExportWork(bool _bIsTHRTask);	
		zArrayStatic<zU8>							maCompiledShader;
		ExportResultRef								mrExport;
	};
}

#endif
