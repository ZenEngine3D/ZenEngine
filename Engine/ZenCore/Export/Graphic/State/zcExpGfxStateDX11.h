#pragma once
#if !defined(__zCore_ExpGfxStateDX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_ExpGfxStateDX11_h__

namespace zcExp
{	
	class ExportDataGfxSamplerDX11 : public ExportDataBase
	{
	ZENClassDeclare(ExportDataGfxSamplerDX11, ExportDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_SAMPLER_DESC			mSamplerDesc;
	};

	class ExportDataGfxStateBlendDX11 : public ExportDataBase
	{
	ZENClassDeclare(ExportDataGfxStateBlendDX11, ExportDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_BLEND_DESC			mBlendDesc;
	};

	class ExportDataGfxStateDepthStencilDX11 : public ExportDataBase
	{
	ZENClassDeclare(ExportDataGfxStateDepthStencilDX11, ExportDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_DEPTH_STENCIL_DESC	mDepthStencilDesc;
	};

	class ExportDataGfxStateRasterizerDX11 : public ExportDataBase
	{
	ZENClassDeclare(ExportDataGfxStateRasterizerDX11, ExportDataBase)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		D3D11_RASTERIZER_DESC		mRasterizerDesc;
	};

}

#endif
