#pragma once
#if !defined(__zCore_Exp_Gfx_Texture2dDX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_Texture2dDX11_h__

namespace zcExp
{	

class ResDataGfxTexture2dDX11 : public ResDataBase
{
ZENClassDeclare(ResDataGfxTexture2dDX11, ResDataBase)
public:
	virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	zArrayStatic<MipInfo>		maMipData;
	zenConst::eTextureFormat	meFormat;
	zFlagResTexUse				mCreationFlags;
};

}

#endif
