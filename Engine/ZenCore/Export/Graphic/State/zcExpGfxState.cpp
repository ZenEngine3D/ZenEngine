#include "zcCore.h"

namespace zcExp
{	
	zResID CreateGfxSampler( zenConst::eTextureFiltering _eFilterMin, zenConst::eTextureFiltering _eFilterMag, zenConst::eTextureWrap _eWrapU, zenConst::eTextureWrap _eWrapV, float _fLodBias, const zVec4F& _vBorderColor )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateSampler", sizeof(ExportInfoGfxSampler), 1, 5 );
		ExportInfoGfxSampler*	pExportInfo	= zenNew(&sMemPool) ExportInfoGfxSampler;
		pExportInfo->meFilterMin			= _eFilterMin;
		pExportInfo->meFilterMag			= _eFilterMag;
		pExportInfo->meWrapU				= _eWrapU;
		pExportInfo->meWrapV				= _eWrapV;			
		pExportInfo->mfLodBias				= _fLodBias;
		pExportInfo->mvBorderColor			= _vBorderColor;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxSampler, pExportInfo );
	}

	zResID CreateGfxBlend( zenType::zBlendDesc* _pBlendDesc )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateBlend", sizeof(ExportInfoGfxStateBlend), 1, 5 );
		ExportInfoGfxStateBlend*	pExportInfo				= zenNew(&sMemPool) ExportInfoGfxStateBlend;
		if( _pBlendDesc )
			pExportInfo->mBlendDesc = *_pBlendDesc;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxBlend, pExportInfo );
	}

	zResID CreateGfxDepthStencil( bool _bDepthEnable, bool _bDepthWrite, bool _bStencilEnable, zU8 _uStencilReadMask, zU8 _uStencilWriteMask, zenConst::eComparisonFunc _eDepthFunc, zenType::zDepthStencilDesc::DepthStencilOp _xFrontFace, zenType::zDepthStencilDesc::DepthStencilOp _xBackFace )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateDepthStencil", sizeof(ExportInfoGfxStateDepthStencil), 1, 5 );
		ExportInfoGfxStateDepthStencil*	pExportInfo				= zenNew(&sMemPool) ExportInfoGfxStateDepthStencil;
		pExportInfo->mDepthStencilDesc.mbDepthEnable		= _bDepthEnable;
		pExportInfo->mDepthStencilDesc.mbDepthWrite			= _bDepthWrite;
		pExportInfo->mDepthStencilDesc.mbStencilEnable		= _bStencilEnable;
		pExportInfo->mDepthStencilDesc.meDepthFunc			= _eDepthFunc;
		pExportInfo->mDepthStencilDesc.muStencilReadMask	= _uStencilReadMask;
		pExportInfo->mDepthStencilDesc.muStencilWriteMask	= _uStencilWriteMask;
		pExportInfo->mDepthStencilDesc.mxBackFace			= _xBackFace;
		pExportInfo->mDepthStencilDesc.mxFrontFace			= _xFrontFace;

		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxDepthStencil, pExportInfo );
	}

	zResID CreateGfxRasterizer( bool _bFrontCounterClockwise, bool _bDepthClipEnable, bool _bScissorEnable, bool _bMultisampleEnable, bool _bAntialiasedLineEnable, bool _bWireFrame, zenConst::eCullMode _eCullMode, zI32 _iDepthBias, float _fDepthBiasClamp, float _fSlopeScaledDepthBias )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateRasterizer", sizeof(ExportInfoGfxStateRasterizer), 1, 5 );
		ExportInfoGfxStateRasterizer*	pExportInfo	= zenNew(&sMemPool) ExportInfoGfxStateRasterizer;

		pExportInfo->mbFrontCounterClockwise	= _bFrontCounterClockwise;
		pExportInfo->mbDepthClipEnable			= _bDepthClipEnable;
		pExportInfo->mbScissorEnable			= _bScissorEnable;
		pExportInfo->mbMultisampleEnable		= _bMultisampleEnable;
		pExportInfo->mbAntialiasedLineEnable	= _bAntialiasedLineEnable;
		pExportInfo->mbWireFrame				= _bWireFrame;
		pExportInfo->meCullMode					= _eCullMode;
		pExportInfo->miDepthBias				= _iDepthBias;
		pExportInfo->mfDepthBiasClamp			= _fDepthBiasClamp;
		pExportInfo->mfSlopeScaledDepthBias		= _fSlopeScaledDepthBias;

		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxRasterizer, pExportInfo );
	}
}