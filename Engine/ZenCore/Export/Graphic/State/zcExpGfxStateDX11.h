#pragma once

namespace zcExp
{	
	class ExportGfxStateSamplerDX11 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateSamplerDX11, zenRes::zExportData)
	public:
		D3D11_SAMPLER_DESC			mSamplerDesc;
	};

	class ExportGfxStateBlendDX11 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateBlendDX11, zenRes::zExportData)
	public:
		D3D11_BLEND_DESC			mBlendDesc;
	};

	class ExportGfxStateDepthStencilDX11 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateDepthStencilDX11, zenRes::zExportData)
	public:
		D3D11_DEPTH_STENCIL_DESC	mDepthStencilDesc;
	};

	class ExportGfxStateRasterDX11 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateRasterDX11, zenRes::zExportData)
	public:
		D3D11_RASTERIZER_DESC		mRasterizerDesc;
	};
}
