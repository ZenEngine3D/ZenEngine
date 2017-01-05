#pragma once
#ifndef __zCore_Exp_Gfx_Texture2d_DX12_DX12_h__
#define __zCore_Exp_Gfx_Texture2d_DX12_DX12_h__

namespace zcExp
{	

class ExporterGfxTexture2dDX12_DX12 : public ExporterBase
{
zenClassDeclare(ExporterGfxTexture2dDX12_DX12, ExporterBase)
public:

typedef zEngineRef<ExportGfxTexture2d>	ExportResultRef; //todo use proper ref class
										ExporterGfxTexture2dDX12_DX12(const ExportResultRef& _rExportOut);
protected:	
	virtual bool						ExportWork(bool _bIsTHRTask);	
	ExportResultRef						mrExport;
};

}

#endif
