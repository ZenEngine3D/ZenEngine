#pragma once
#ifndef __zCore_Res_Gfx_Shaders_DX11_h__
#define __zCore_Res_Gfx_Shaders_DX11_h__

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace zcRes
{
	typedef zMap<zcExp::ResDataGfxShaderDX11::BindInfo>::Key32	MapBindInfo;

	//=================================================================================================
	class GfxInputSignatureProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxInputSignatureProxy_DX11, zRefCounted)
	public:												
											GfxInputSignatureProxy_DX11();
		virtual								~GfxInputSignatureProxy_DX11();												
		bool								Initialize(class GfxInputSignature& _Owner);
		zArrayStatic<zU8>					maDummyShaderCode;
		ZENDbgCode(class GfxInputSignature*	mpOwner);
	};

	//=================================================================================================
	class GfxShaderVertexProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxShaderVertexProxy_DX11, zRefCounted)
	public:												
											GfxShaderVertexProxy_DX11();
		virtual								~GfxShaderVertexProxy_DX11();												
		bool								Initialize(class GfxShaderVertex& _Owner);
		ID3D11VertexShader*					mpVertexShader;		
		ZENDbgCode(class GfxShaderVertex*	mpOwner);
	};

	//=================================================================================================	
	class GfxShaderPixelProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxShaderPixelProxy_DX11, zRefCounted)
	public:											
											GfxShaderPixelProxy_DX11();
		virtual								~GfxShaderPixelProxy_DX11();												
		bool								Initialize(class GfxShaderPixel& _Owner);
		ID3D11PixelShader*					mpPixelShader;
		ZENDbgCode(class GfxShaderPixel*	mpOwner);
	};
}

#endif