#pragma once
#if !defined(__zCore_Exp_Gfx_GfxShaderInputSignature_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxShaderInputSignature_DX11_h__

namespace zcExp
{	
class ExportGfxInputSignatureDX11 : public zenRes::zExportData
{
ZENClassDeclare(ExportGfxInputSignatureDX11, zenRes::zExportData)
public:
	zArrayStatic<zU8>			maDummyShaderCode;
};

}

#endif
