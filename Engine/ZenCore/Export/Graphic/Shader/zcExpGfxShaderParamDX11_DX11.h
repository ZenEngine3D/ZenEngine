#pragma once
#ifndef __zCore_Exp_GfxShaderParamDX11_DX11_h__
#define __zCore_Exp_GfxShaderParamDX11_DX11_h__

namespace zcExp
{
	class ExporterGfxShaderParamDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxShaderParamDX11_DX11, ExporterBase)
	public:				
	typedef ExportDataGfxShaderParamDX11	ExportData;
	typedef zGameRef<ExportData>			ExportDataRef;
											ExporterGfxShaderParamDX11_DX11(const ExportDataRef& _rExportData);
	protected:	
		virtual bool						ExportStart();
		ExportDataRef						mrExportData;
	};
}

#endif
