#include "zcCore.h"

namespace zcExp
{	
	zResID CreateGfxSampler( zenConst::eTextureFiltering _eFilterMin, zenConst::eTextureFiltering _eFilterMag, zenConst::eTextureWrap _eWrapU, zenConst::eTextureWrap _eWrapV, float _fLodBias, const zVec4F& _vBorderColor )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateSampler", sizeof(SerialSampler_Base::ExportInfo), 1, 5 );
		SerialSampler_Base::ExportInfo*	pExportInfo	= zenNew(&sMemPool) SerialSampler_Base::ExportInfo;
		pExportInfo->meFilterMin		= _eFilterMin;
		pExportInfo->meFilterMag		= _eFilterMag;
		pExportInfo->meWrapU			= _eWrapU;
		pExportInfo->meWrapV			= _eWrapV;			
		pExportInfo->mfLodBias			= _fLodBias;
		pExportInfo->mvBorderColor		= _vBorderColor;
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxSampler, pExportInfo );
	}

	zResID CreateGfxBlend( zenType::awBlendDesc::awRTBlendDesc* _pxBlendDesc, zU8 _uRenderTargets, bool _bAlphaToCoverageEnable, bool _bIndependentBlendEnable )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateBlend", sizeof(SerialBlend_Base::ExportInfo), 1, 5 );
		SerialBlend_Base::ExportInfo*	pExportInfo	= zenNew(&sMemPool) SerialBlend_Base::ExportInfo;
		pExportInfo->mxBlendDesc.mbAlphaToCoverageEnable		= _bAlphaToCoverageEnable;
		pExportInfo->mxBlendDesc.mbIndependentBlendEnable		= _bIndependentBlendEnable;
		if(_pxBlendDesc)
		{
			for(zU8 i = 0; i < _uRenderTargets; ++i)
			{
				pExportInfo->mxBlendDesc.mxRenderTarget[i] = _pxBlendDesc[i];
			}
		}
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxBlend, pExportInfo );
	}

	zResID CreateGfxDepthStencil( bool _bDepthEnable, bool _bDepthWrite, bool _bStencilEnable, zU8 _uStencilReadMask, zU8 _uStencilWriteMask, zenConst::eComparisonFunc _eDepthFunc, zenType::awDepthStencilDesc::DepthStencilOp _xFrontFace, zenType::awDepthStencilDesc::DepthStencilOp _xBackFace )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateDepthStencil", sizeof(SerialDepthStencil_Base::ExportInfo), 1, 5 );
		SerialDepthStencil_Base::ExportInfo*	pExportInfo	= zenNew(&sMemPool) SerialDepthStencil_Base::ExportInfo;
		pExportInfo->mxDepthStencilDesc.mbDepthEnable		= _bDepthEnable;
		pExportInfo->mxDepthStencilDesc.mbDepthWrite		= _bDepthWrite;
		pExportInfo->mxDepthStencilDesc.mbStencilEnable		= _bStencilEnable;
		pExportInfo->mxDepthStencilDesc.meDepthFunc			= _eDepthFunc;
		pExportInfo->mxDepthStencilDesc.muStencilReadMask	= _uStencilReadMask;
		pExportInfo->mxDepthStencilDesc.muStencilWriteMask	= _uStencilWriteMask;
		pExportInfo->mxDepthStencilDesc.mxBackFace			= _xBackFace;
		pExportInfo->mxDepthStencilDesc.mxFrontFace			= _xFrontFace;

		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxDepthStencil, pExportInfo );
	}

	zResID CreateGfxRasterizer( bool _bFrontCounterClockwise, bool _bDepthClipEnable, bool _bScissorEnable, bool _bMultisampleEnable, bool _bAntialiasedLineEnable, bool _bWireFrame, zenConst::eCullMode _eCullMode, zI32 _iDepthBias, float _fDepthBiasClamp, float _fSlopeScaledDepthBias )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateRasterizer", sizeof(SerialRasterizer_Base::ExportInfo), 1, 5 );
		SerialRasterizer_Base::ExportInfo*	pExportInfo	= zenNew(&sMemPool) SerialRasterizer_Base::ExportInfo;

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

		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxSampler, pExportInfo );
	}
}