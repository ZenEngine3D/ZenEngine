#pragma once
#ifndef __zCore_Exp_Gfx_SerialGfxVertex_DX11_DX11_h__
#define __zCore_Exp_Gfx_SerialGfxVertex_DX11_DX11_h__

namespace zcExp
{

class ExporterGfxVertexDX11_DX11 : public ExporterBase
{
ZENClassDeclare(ExporterGfxVertexDX11_DX11, ExporterBase)
public:
typedef zEngineRef<ExportGfxVertexDX11>	ExportResultRef; //todo use proper ref class
										ExporterGfxVertexDX11_DX11(const ExportResultRef& _rExportOut);
								
protected:	
	virtual bool						ExportWork(bool _bIsTHRTask);
	ExportResultRef						mrExport;
};

}

#endif