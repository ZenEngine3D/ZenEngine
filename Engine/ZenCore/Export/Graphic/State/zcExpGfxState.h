#pragma once

namespace zcExp
{
	struct ExportInfoGfxStateSampler : public ExportInfoBase
	{			
		zenConst::eTextureFiltering	meFilterMin;
		zenConst::eTextureFiltering	meFilterMag;
		zenConst::eTextureWrap		meWrapU;
		zenConst::eTextureWrap		meWrapV;
		float						mfLodBias;
		zVec4F						mvBorderColor;
	};

	struct ExportInfoGfxStateBlend : public ExportInfoBase
	{
		const zArray< zenRes::zGfxRenderPass::ConfigColorRT>* mpaRenderTargetConfig; // Only designed for runtime, makes no copy
	};

	struct ExportInfoGfxStateDepthStencil : public ExportInfoBase
	{
		const zenRes::zGfxRenderPass::ConfigDepthRT* mpDepthStencilConfig; // Only designed for runtime, makes no copy
	};

	struct ExportInfoGfxStateRaster : public ExportInfoBase
	{
		zenRes::zGfxStateRaster::Config	mRasterConfig;
	};
		
	zResID CreateGfxStateSampler( zenConst::eTextureFiltering _eFilterMin=zenConst::keTexFilter_Bilinear, zenConst::eTextureFiltering _eFilterMag=zenConst::keTexFilter_Bilinear, zenConst::eTextureWrap _eWrapU=zenConst::keTexWrap_Repeat, zenConst::eTextureWrap _eWrapV=zenConst::keTexWrap_Repeat, float _fLodBias=0, const zVec4F& _vBorderColor=zVec4F(0,0,0,1) );	
	zResID CreateGfxRasterizer( const zenRes::zGfxStateRaster::Config& _RasterConfig );
	zResID CreateGfxDepthStencil( const zenRes::zGfxRenderPass::ConfigDepthRT& _DepthStencilConfig );
	zResID CreateGfxBlend( const zArray<zenRes::zGfxRenderPass::ConfigColorRT>& _aTargetColorConfig);
}
