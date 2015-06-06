#pragma once
#ifndef __zCore_Exp_GfxShaderParamDef_DX11_DX11_h__
#define __zCore_Exp_GfxShaderParamDef_DX11_DX11_h__

namespace zcExp
{
	class ExporterGfxShaderParamDefDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxShaderParamDefDX11_DX11, ExporterBase)
	public:				
	typedef ExportDataGfxShaderParamDefDX11	ExportData;
	typedef zGameRef<ExportData>			ExportDataRef;
											ExporterGfxShaderParamDefDX11_DX11(const ExportDataRef& _rExportData);
	protected:	
		virtual bool						ExportStart();
		virtual bool						ExportWork(bool _bIsTHRTask);	
		ExportDataRef						mrExportData;
		zArrayStatic<zU8>					maCompiledShader;
	};
}

#endif
