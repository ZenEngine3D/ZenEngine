#pragma once
#if !defined(__zCore_ExpGfxStateDX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_ExpGfxStateDX11_h__

namespace zcExp
{	
	class ResDataGfxSamplerDX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxSamplerDX11, ResourceData)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_SAMPLER_DESC			mSamplerDesc;
	};

	class ResDataGfxStateBlendDX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxStateBlendDX11, ResourceData)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_BLEND_DESC			mBlendDesc;
	};

	class ResDataGfxStateDepthStencilDX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxStateDepthStencilDX11, ResourceData)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_DEPTH_STENCIL_DESC	mDepthStencilDesc;
	};

	class ResDataGfxStateRasterizerDX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxStateRasterizerDX11, ResourceData)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_RASTERIZER_DESC		mRasterizerDesc;
	};

}

#endif
