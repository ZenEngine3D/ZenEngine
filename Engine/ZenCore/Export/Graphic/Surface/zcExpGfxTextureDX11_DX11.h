#pragma once
#ifndef __zCore_Exp_Gfx_Texture2d_DX11_DX11_h__
#define __zCore_Exp_Gfx_Texture2d_DX11_DX11_h__

namespace zcExp
{	

class ExporterGfxTexture2dDX11_DX11 : public ExporterBase
{
ZENClassDeclare(ExporterGfxTexture2dDX11_DX11, ExporterBase)
public:				
typedef ExportDataGfxTexture2dDX11	ExportData;
typedef zGameRef<ExportData>		ExportDataRef;
									ExporterGfxTexture2dDX11_DX11(const ExportDataRef& _rExportData);
protected:	
	virtual bool					ExportWork(bool _bIsTHRTask);	
	ExportDataRef					mrExportData;
};

}

#endif
