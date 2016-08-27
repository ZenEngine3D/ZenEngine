#pragma once
#ifndef __zCore_Exp_Gfx_Buffer_DX11_h__
#define __zCore_Exp_Gfx_Buffer_DX11_h__

namespace zcExp
{	

class ExporterGfxBuffer_DX11 : public ExporterBase
{
zenClassDeclare(ExporterGfxBuffer_DX11, ExporterBase)
public:				
typedef zEngineRef<ExportGfxBuffer>		ExportDataRef; //todo use proper ref class
										ExporterGfxBuffer_DX11(const ExportDataRef& _rExportData);		
							
protected:	
	virtual bool						ExportStart();	
	ExportDataRef						mrExportData;
};

}

#endif
