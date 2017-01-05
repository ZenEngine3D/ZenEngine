#pragma once
#ifndef __zCore_ExpGfxStateDX12_h__
#define __zCore_ExpGfxStateDX12_h__

namespace zcExp
{	
	class ExportGfxSamplerDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxSamplerDX12, zenRes::zExportData)
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

#endif
