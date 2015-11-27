#pragma once
#ifndef __zCore_Res_Gfx_Texture_DX11_h__
#define __zCore_Res_Gfx_Texture_DX11_h__

namespace zcRes
{

class GfxTexture2dProxy_DX11 : public zRefCounted
{
ZENClassDeclare(GfxTexture2dProxy_DX11, zRefCounted)
public:
											GfxTexture2dProxy_DX11();
	virtual									~GfxTexture2dProxy_DX11();												
	bool									Initialize(class GfxTexture2d& _Owner);
	ID3D11Texture2D*						mpTextureBuffer;
	ID3D11ShaderResourceView*				mpTextureView;
	ZENDbgCode(class GfxTexture2d*	mpOwner);
};

}

#endif
