#pragma once

namespace zcExp
{	
	class ExportGfxStateSamplerDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateSamplerDX12, zenRes::zExportData)
	public:
		D3D12_SAMPLER_DESC			mSamplerDesc;
	};

	class ExportGfxStateBlendDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateBlendDX12, zenRes::zExportData)
	public:
		D3D12_BLEND_DESC			mBlendDesc;
	};

	class ExportGfxStateDepthStencilDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateDepthStencilDX12, zenRes::zExportData)
	public:
		D3D12_DEPTH_STENCIL_DESC	mDepthStencilDesc;
	};

	class ExportGfxStateRasterDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateRasterDX12, zenRes::zExportData)
	public:
		D3D12_RASTERIZER_DESC		mRasterizerDesc;
	};
}
