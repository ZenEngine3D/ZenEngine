#pragma once
#ifndef __zCore_Exp_Gfx_SerialGfxIndex_DX11_DX11_h__
#define __zCore_Exp_Gfx_SerialGfxIndex_DX11_DX11_h__

namespace zcExp
{	

class ExporterGfxIndexDX11_DX11 : public ExporterBase
{
ZENClassDeclare(ExporterGfxIndexDX11_DX11, ExporterBase)
public:				
typedef ExportGfxIndexDX11		ExportData;
typedef zEngineRef<ExportData>	ExportDataRef;
								ExporterGfxIndexDX11_DX11(const ExportDataRef& _rExportData);		

protected:	
	virtual bool				ExportWork(bool _bIsTHRTask);	
	ExportDataRef				mrExportData;
};

}

#endif