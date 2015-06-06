#pragma once
#ifndef __zCore_Exp_Gfx_GfxShader_DX11_DX11_h__
#define __zCore_Exp_Gfx_GfxShader_DX11_DX11_h__

namespace zcExp
{	
	class ExporterGfxShaderDX11_DX11 : public ExporterBase 
	{
	ZENClassDeclare(ExporterGfxShaderDX11_DX11, ExporterBase)
	public:				
		typedef ExportDataGfxShaderDX11 ExportData;
		typedef zGameRef<ExportData>	ExportDataRef;
										ExporterGfxShaderDX11_DX11(const ExportDataRef& _rExportData);		
	protected:	
		virtual bool					ExportWork(bool _bIsTHRTask);
		virtual bool					ExportEnd();
		bool							ExportWorkCompile();
		bool							ExportWorkExtractResources();		
		ExportDataRef					mrExportData;
	};
}

#endif