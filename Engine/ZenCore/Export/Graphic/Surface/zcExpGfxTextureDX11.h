#pragma once
#if !defined(__zCore_Exp_Gfx_Texture2dDX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_Texture2dDX11_h__

namespace zcExp
{	

class ExportDataGfxTexture2dDX11 : public ExportDataBase
{
ZENClassDeclare(ExportDataGfxTexture2dDX11, ExportDataBase)
public:
	virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	zArrayStatic<MipInfo>		maMipData;
	zenConst::eTextureFormat	meFormat;
	zFlagResTexUse				mCreationFlags;
};

}

#endif
