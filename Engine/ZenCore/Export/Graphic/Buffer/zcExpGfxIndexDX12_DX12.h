#pragma once
#ifndef __zCore_Exp_Gfx_SerialGfxIndex_DX12_DX12_h__
#define __zCore_Exp_Gfx_SerialGfxIndex_DX12_DX12_h__
//SF DX12

namespace zcExp
{	

class ExporterGfxIndexDX12_DX12 : public ExporterBase
{
zenClassDeclare(ExporterGfxIndexDX12_DX12, ExporterBase)
public:				
typedef zEngineRef<ExportGfxIndexDX12>	ExportDataRef; //todo use proper ref class
										ExporterGfxIndexDX12_DX12(const ExportDataRef& _rExportData);		
							
protected:	
	virtual bool						ExportWork(bool _bIsTHRTask);	
	ExportDataRef						mrExportData;
};

}

#endif