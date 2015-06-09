#pragma once
#if !defined(__zCore_Exp_Gfx_GfxInputStreamBinding_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11 
#define __zCore_Exp_Gfx_GfxInputStreamBinding_DX11_h__

namespace zcExp
{	

class ResDataGfxInputStreamDX11 : public ResourceData
{
ZENClassDeclare(ResDataGfxInputStreamDX11, ResourceData)
public:
	virtual bool	Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	zResID			mVertexBufferID;
	zResID			mShaderInputSignatureID;
};

}

#endif
