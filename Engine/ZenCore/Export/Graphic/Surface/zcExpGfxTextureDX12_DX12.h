#pragma once
#ifndef __zCore_Exp_Gfx_Texture2d_DX12_DX12_h__
#define __zCore_Exp_Gfx_Texture2d_DX12_DX12_h__
//SF DX12

namespace zcExp
{	

class ExporterGfxTexture2dDX11_DX11 : public ExporterBase
{
zenClassDeclare(ExporterGfxTexture2dDX11_DX11, ExporterBase)
public:

typedef zEngineRef<ExportGfxTexture2d>	ExportResultRef; //todo use proper ref class
										ExporterGfxTexture2dDX11_DX11(const ExportResultRef& _rExportOut);
protected:	
	virtual bool				ExportWork(bool _bIsTHRTask);	
	ExportResultRef				mrExport;
};

}

#endif
