#include "libZenCore.h"

namespace EExp
{	
	awResourceID CreateGfxSampler( awconst::eTextureFiltering _eFilterMin, awconst::eTextureFiltering _eFilterMag, awconst::eTextureWrap _eWrapU, awconst::eTextureWrap _eWrapV, float _fLodBias, const awVec4F& _vBorderColor )
	{
		static CMem::PoolAllocator sMemPool("Pool CreateSampler", sizeof(SerialSampler_Base::ExportInfo), 1, 5 );
		SerialSampler_Base::ExportInfo*	pExportInfo	= AWNew(&sMemPool) SerialSampler_Base::ExportInfo;
		pExportInfo->meFilterMin		= _eFilterMin;
		pExportInfo->meFilterMag		= _eFilterMag;
		pExportInfo->meWrapU			= _eWrapU;
		pExportInfo->meWrapV			= _eWrapV;			
		pExportInfo->mfLodBias			= _fLodBias;
		pExportInfo->mvBorderColor		= _vBorderColor;
		return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxSampler, pExportInfo );
	}

	awResourceID CreateGfxBlend( awtype::awBlendDesc::awRTBlendDesc* _pxBlendDesc, awU8 _uRenderTargets, bool _bAlphaToCoverageEnable, bool _bIndependentBlendEnable )
	{
		static CMem::PoolAllocator sMemPool("Pool CreateBlend", sizeof(SerialBlend_Base::ExportInfo), 1, 5 );
		SerialBlend_Base::ExportInfo*	pExportInfo	= AWNew(&sMemPool) SerialBlend_Base::ExportInfo;
		pExportInfo->mxBlendDesc.mbAlphaToCoverageEnable		= _bAlphaToCoverageEnable;
		pExportInfo->mxBlendDesc.mbIndependentBlendEnable		= _bIndependentBlendEnable;
		if(_pxBlendDesc)
		{
			for(awU8 i = 0; i < _uRenderTargets; ++i)
			{
				pExportInfo->mxBlendDesc.mxRenderTarget[i] = _pxBlendDesc[i];
			}
		}
		return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxBlend, pExportInfo );
	}

	awResourceID CreateGfxDepthStencil( bool _bDepthEnable, bool _bDepthWrite, bool _bStencilEnable, awU8 _uStencilReadMask, awU8 _uStencilWriteMask, awconst::eComparisonFunc _eDepthFunc, awtype::awDepthStencilDesc::DepthStencilOp _xFrontFace, awtype::awDepthStencilDesc::DepthStencilOp _xBackFace )
	{
		static CMem::PoolAllocator sMemPool("Pool CreateDepthStencil", sizeof(SerialDepthStencil_Base::ExportInfo), 1, 5 );
		SerialDepthStencil_Base::ExportInfo*	pExportInfo	= AWNew(&sMemPool) SerialDepthStencil_Base::ExportInfo;
		pExportInfo->mxDepthStencilDesc.mbDepthEnable		= _bDepthEnable;
		pExportInfo->mxDepthStencilDesc.mbDepthWrite		= _bDepthWrite;
		pExportInfo->mxDepthStencilDesc.mbStencilEnable		= _bStencilEnable;
		pExportInfo->mxDepthStencilDesc.meDepthFunc			= _eDepthFunc;
		pExportInfo->mxDepthStencilDesc.muStencilReadMask	= _uStencilReadMask;
		pExportInfo->mxDepthStencilDesc.muStencilWriteMask	= _uStencilWriteMask;
		pExportInfo->mxDepthStencilDesc.mxBackFace			= _xBackFace;
		pExportInfo->mxDepthStencilDesc.mxFrontFace			= _xFrontFace;

		return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxDepthStencil, pExportInfo );
	}

	awResourceID CreateGfxRasterizer( bool _bFrontCounterClockwise, bool _bDepthClipEnable, bool _bScissorEnable, bool _bMultisampleEnable, bool _bAntialiasedLineEnable, bool _bWireFrame, awconst::eCullMode _eCullMode, awS32 _iDepthBias, awF32 _fDepthBiasClamp, awF32 _fSlopeScaledDepthBias )
	{
		static CMem::PoolAllocator sMemPool("Pool CreateRasterizer", sizeof(SerialRasterizer_Base::ExportInfo), 1, 5 );
		SerialRasterizer_Base::ExportInfo*	pExportInfo	= AWNew(&sMemPool) SerialRasterizer_Base::ExportInfo;

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

		return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxSampler, pExportInfo );
	}
}