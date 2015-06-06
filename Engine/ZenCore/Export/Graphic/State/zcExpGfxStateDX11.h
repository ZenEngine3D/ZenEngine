#pragma once
#if !defined(__zCore_ExpGfxStateDX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_ExpGfxStateDX11_h__

namespace zcExp
{	
	class ResDataGfxSamplerDX11 : public ResDataBase
	{
	ZENClassDeclare(ResDataGfxSamplerDX11, ResDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_SAMPLER_DESC			mSamplerDesc;
	};

	class ResDataGfxStateBlendDX11 : public ResDataBase
	{
	ZENClassDeclare(ResDataGfxStateBlendDX11, ResDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_BLEND_DESC			mBlendDesc;
	};

	class ResDataGfxStateDepthStencilDX11 : public ResDataBase
	{
	ZENClassDeclare(ResDataGfxStateDepthStencilDX11, ResDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_DEPTH_STENCIL_DESC	mDepthStencilDesc;
	};

	class ResDataGfxStateRasterizerDX11 : public ResDataBase
	{
	ZENClassDeclare(ResDataGfxStateRasterizerDX11, ResDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_RASTERIZER_DESC		mRasterizerDesc;
	};

}

#endif
