#pragma once
#if !defined(__zCore_Exp_Gfx_GfxShaderInputSignature_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxShaderInputSignature_DX11_h__

namespace zcExp
{	

class ResDataGfxInputSignatureDX11 : public ResourceData
{
ZENClassDeclare(ResDataGfxInputSignatureDX11, ResourceData)
public:
	virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	zArrayStatic<zU8>			maDummyShaderCode;
};

}

#endif
