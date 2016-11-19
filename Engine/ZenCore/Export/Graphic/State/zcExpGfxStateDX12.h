#pragma once
#if !defined(__zCore_ExpGfxStateDX12_h__) && ZEN_EXPORT_OR_RESOURCE_DX12
#define __zCore_ExpGfxStateDX12_h__
//SF DX12

namespace zcExp
{	
	class ExportGfxSamplerDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxSamplerDX12, zenRes::zExportData)
	public:
		D3D11_SAMPLER_DESC			mSamplerDesc;
	};

	class ExportGfxStateBlendDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateBlendDX12, zenRes::zExportData)
	public:
		D3D11_BLEND_DESC			mBlendDesc;
	};

	class ExportGfxStateDepthStencilDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateDepthStencilDX12, zenRes::zExportData)
	public:
		D3D11_DEPTH_STENCIL_DESC	mDepthStencilDesc;
	};

	class ExportGfxStateRasterDX12 : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxStateRasterDX12, zenRes::zExportData)
	public:
		D3D11_RASTERIZER_DESC		mRasterizerDesc;
	};
}

#endif
