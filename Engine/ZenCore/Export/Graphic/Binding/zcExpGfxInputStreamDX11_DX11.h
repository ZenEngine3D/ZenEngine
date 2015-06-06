#pragma once
#ifndef __zCore_Exp_Gfx_GfxInputStreamBinding_DX11_DX11_h__
#define __zCore_Exp_Gfx_GfxInputStreamBinding_DX11_DX11_h__

namespace zcExp
{	

class ExporterGfxInputStreamDX11_DX11 : public ExporterBase
{
ZENClassDeclare(ExporterGfxInputStreamDX11_DX11, ExporterBase)
public:				
typedef ExportDataGfxInputStreamDX11	ExportData;
typedef zGameRef<ExportData>			ExportDataRef;
										ExporterGfxInputStreamDX11_DX11(const ExportDataRef& _rExportData);		
	static zResID						CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
protected:	
	virtual bool						ExportWork(bool _bIsTHRTask);	
	ExportDataRef						mrExportData;
};

}

#endif
