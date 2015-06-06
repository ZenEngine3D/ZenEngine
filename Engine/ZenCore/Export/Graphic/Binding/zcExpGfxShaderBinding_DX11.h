#pragma once
#ifndef __zCore_Exp_Gfx_GfxShaderBinding_DX11_h__
#define __zCore_Exp_Gfx_GfxShaderBinding_DX11_h__

namespace zcExp
{	
	class ExporterGfxShaderBindingDX11_DX11 : public ExporterGfxShaderBinding
	{
	ZENClassDeclare(ExporterGfxShaderBindingDX11_DX11, ExporterGfxShaderBinding)
	public:				
	typedef ExportDataGfxShaderBinding		ExportData;
	typedef zGameRef<ExportData>			ExportDataRef;
											ExporterGfxShaderBindingDX11_DX11(const ExportDataRef& _rExportData);
	protected:	
		virtual bool						ExportStart();		
		ExportDataRef						mrExportData;
	};
}

#endif