#pragma once
#ifndef __zCore_Exp_Gfx_GfxShaderInputSignature_DX11_DX11_h__
#define __zCore_Exp_Gfx_GfxShaderInputSignature_DX11_DX11_h__

namespace zcExp
{	

class ExporterGfxInputSignatureDX11_DX11 : public ExporterBase
{
ZENClassDeclare(ExporterGfxInputSignatureDX11_DX11, ExporterBase)
public:				
typedef ExportDataGfxInputSignatureDX11	ExportData;
typedef zGameRef<ExportData>			ExportDataRef;
										ExporterGfxInputSignatureDX11_DX11(const ExportDataRef& _rExportData);		
	static zResID						CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
protected:	
	virtual bool						ExportStart();
	virtual bool						ExportWork(bool _bIsTHRTask);	
	ExportDataRef						mrExportData;
	zArrayStatic<zU8>					maParentCompiledShader;
};

}

#endif