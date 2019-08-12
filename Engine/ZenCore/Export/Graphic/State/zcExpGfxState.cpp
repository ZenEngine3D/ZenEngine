#include "zcCore.h"

namespace zcExp
{	
	zResID CreateGfxStateSampler( zenConst::eTextureFiltering _eFilterMin, zenConst::eTextureFiltering _eFilterMag, zenConst::eTextureWrap _eWrapU, zenConst::eTextureWrap _eWrapV, float _fLodBias, const zVec4F& _vBorderColor )
	{
		auto* pExportInfo					= zenMem::NewPool<ExportInfoGfxStateSampler>();
		pExportInfo->meFilterMin			= _eFilterMin;
		pExportInfo->meFilterMag			= _eFilterMag;
		pExportInfo->meWrapU				= _eWrapU;
		pExportInfo->meWrapV				= _eWrapV;			
		pExportInfo->mfLodBias				= _fLodBias;
		pExportInfo->mvBorderColor			= _vBorderColor;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxStateSampler, pExportInfo );
	}

	zResID CreateGfxRasterizer( const zenRes::zGfxStateRaster::Config& _RasterConfig )
	{
		auto* pExportInfo					= zenMem::NewPool<ExportInfoGfxStateRaster>();
		pExportInfo->mRasterConfig			= _RasterConfig; //! @todo optim prevent copy duplicate for dynamically created element (done right away, no 
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxStateRaster, pExportInfo );
	}

	zResID CreateGfxDepthStencil( const zenRes::zGfxRenderPass::ConfigDepthRT& _DepthStencilConfig )
	{
		auto* pExportInfo					= zenMem::NewPool<ExportInfoGfxStateDepthStencil>();
		pExportInfo->mpDepthStencilConfig	= &_DepthStencilConfig; 
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxStateDepthStencil, pExportInfo );
	}

	zResID CreateGfxBlend( const zArray<zenRes::zGfxRenderPass::ConfigColorRT>& _aTargetColorConfig)
	{
		auto* pExportInfo					= zenMem::NewPool<ExportInfoGfxStateBlend>();
		pExportInfo->mpaRenderTargetConfig	= &_aTargetColorConfig;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxStateBlend, pExportInfo );
	}
}