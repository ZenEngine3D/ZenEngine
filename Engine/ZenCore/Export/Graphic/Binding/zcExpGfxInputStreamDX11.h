#pragma once
#if !defined(__zCore_Exp_Gfx_GfxInputStreamBinding_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11 
#define __zCore_Exp_Gfx_GfxInputStreamBinding_DX11_h__

namespace zcExp
{	

class ResDataGfxInputStreamDX11 : public ResDataBase
{
ZENClassDeclare(ResDataGfxInputStreamDX11, ResDataBase)
public:
	virtual bool	Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	zResID			mVertexBufferID;
	zResID			mShaderInputSignatureID;
};

}

#endif
