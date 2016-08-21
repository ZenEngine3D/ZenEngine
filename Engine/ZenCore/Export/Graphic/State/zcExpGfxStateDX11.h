#pragma once
#if !defined(__zCore_ExpGfxStateDX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_ExpGfxStateDX11_h__

namespace zcExp
{	
	class ExportGfxSamplerDX11 : public zenRes::zExportData
	{
	ZENClassDeclare(ExportGfxSamplerDX11, zenRes::zExportData)
	public:
		D3D11_SAMPLER_DESC			mSamplerDesc;
	};

	class ExportGfxStateBlendDX11 : public zenRes::zExportData
	{
	ZENClassDeclare(ExportGfxStateBlendDX11, zenRes::zExportData)
	public:
		D3D11_BLEND_DESC			mBlendDesc;
	};

	class ExportGfxStateDepthStencilDX11 : public zenRes::zExportData
	{
	ZENClassDeclare(ExportGfxStateDepthStencilDX11, zenRes::zExportData)
	public:
		D3D11_DEPTH_STENCIL_DESC	mDepthStencilDesc;
	};

	class ExportGfxStateRasterDX11 : public zenRes::zExportData
	{
	ZENClassDeclare(ExportGfxStateRasterDX11, zenRes::zExportData)
	public:
		D3D11_RASTERIZER_DESC		mRasterizerDesc;
	};
}

#endif
