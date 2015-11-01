#include "zcCore.h"

#if ZEN_EXPORT_DX11

namespace zcExp
{
	static const zArrayStatic<D3D11_COMPARISON_FUNC> gaComparisonTestTranslator = {	
			D3D11_COMPARISON_NEVER,				// keCmpTest_Never
			D3D11_COMPARISON_LESS, 				// keCmpTest_Less
			D3D11_COMPARISON_EQUAL, 			// keCmpTest_Equal
			D3D11_COMPARISON_LESS_EQUAL,		// keCmpTest_LessEqual
			D3D11_COMPARISON_GREATER,			// keCmpTest_Greater
			D3D11_COMPARISON_NOT_EQUAL, 		// keCmpTest_NotEqual
			D3D11_COMPARISON_GREATER_EQUAL, 	// keCmpTest_GreaterEqual
			D3D11_COMPARISON_ALWAYS 			// keCmpTest_Always
			};

	static const zArrayStatic<D3D11_STENCIL_OP> gaStencilOpTranslator = { 
			D3D11_STENCIL_OP_KEEP,				// keStencilOp_Keep,
			D3D11_STENCIL_OP_ZERO, 				// keStencilOp_Zero,
			D3D11_STENCIL_OP_REPLACE,			// keStencilOp_Replace,
			D3D11_STENCIL_OP_INCR_SAT,			// keStencilOp_IncrSat,
			D3D11_STENCIL_OP_DECR_SAT,			// keStencilOp_DecrSat,
			D3D11_STENCIL_OP_INVERT,			// keStencilOp_Invert,
			D3D11_STENCIL_OP_INCR, 				// keStencilOp_Incr,
			D3D11_STENCIL_OP_DECR				// keStencilOp_Decr
			};

	static const zArrayStatic<D3D11_BLEND> gaBlendValueTranslator = {
			D3D11_BLEND_ZERO,					// keBlendVal_Zero, 
			D3D11_BLEND_ONE, 					// keBlendVal_One, 
			D3D11_BLEND_SRC_COLOR, 				// keBlendVal_SrcColor, 
			D3D11_BLEND_INV_SRC_COLOR, 			// keBlendVal_InvSrcColor, 
			D3D11_BLEND_SRC_ALPHA, 				// keBlendVal_SrcAlpha, 
			D3D11_BLEND_INV_SRC_ALPHA, 			// keBlendVal_InvSrcAlpha, 
			D3D11_BLEND_DEST_ALPHA,				// keBlendVal_DestAlpha, 
			D3D11_BLEND_INV_DEST_ALPHA, 		// keBlendVal_InvDestAlpha, 
			D3D11_BLEND_DEST_COLOR, 			// keBlendVal_DestColor, 
			D3D11_BLEND_INV_DEST_COLOR, 		// keBlendVal_InvDestColor, 
			D3D11_BLEND_SRC_ALPHA_SAT, 			// keBlendVal_SrcAlphaSat, 
			D3D11_BLEND_BLEND_FACTOR, 			// keBlendVal_BlendFactor, 
			D3D11_BLEND_INV_BLEND_FACTOR		// keBlendVal_InvBlendFactor, 
			};

	static const zArrayStatic<D3D11_BLEND_OP> gaBlendOpTranslator = {
			D3D11_BLEND_OP_ADD,					// keBlendOp_Add, 
			D3D11_BLEND_OP_SUBTRACT, 			// keBlendOp_Substract, 
			D3D11_BLEND_OP_REV_SUBTRACT, 		// keBlendOp_RevSubstract,
			D3D11_BLEND_OP_MIN, 				// keBlendOp_Min, 
			D3D11_BLEND_OP_MAX					// keBlendOp_Max, 
			};

	//////////////////////////////////////////////////////////////////////////
	//ExporterGfxSamplerDX11_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxSamplerDX11_DX11::ExporterGfxSamplerDX11_DX11(const ResDataRef& _rResData)
	: ExporterBase(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxSamplerDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrResData.IsValid());
		ExportInfoGfxSampler* pExportInfo = static_cast<ExportInfoGfxSampler*>(mpExportInfo);
		ZeroMemory( &mrResData->mSamplerDesc, sizeof(mrResData->mSamplerDesc) );
		mrResData->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if( pExportInfo->meFilterMin == zenConst::keTexFilter_Anisotropic || pExportInfo->meFilterMag == zenConst::keTexFilter_Anisotropic )
		{
			mrResData->mSamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		}
		else if( pExportInfo->meFilterMin == zenConst::keTexFilter_Trilinear || pExportInfo->meFilterMag == zenConst::keTexFilter_Trilinear )
		{
			mrResData->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}
		else
		{
			switch( pExportInfo->meFilterMag )
			{
			case zenConst::keTexFilter_Point:
			{
				switch( pExportInfo->meFilterMin )
				{
				case zenConst::keTexFilter_Point:		mrResData->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		break;
				case zenConst::keTexFilter_Bilinear:	mrResData->mSamplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;	break;		
				}break;
			}
			case zenConst::keTexFilter_Bilinear:
			{
				switch( pExportInfo->meFilterMin )
				{
				case zenConst::keTexFilter_Point:		mrResData->mSamplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;	break;
				case zenConst::keTexFilter_Bilinear:	mrResData->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;			break;		
				}break;
			}
			}
		}

		const D3D11_TEXTURE_ADDRESS_MODE cWrapTranslate[]= { D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_BORDER };
		ZENStaticAssert( ZENArrayCount(cWrapTranslate)==zenConst::keTexWrap__Count );

		mrResData->mSamplerDesc.AddressU		= cWrapTranslate[pExportInfo->meWrapU];
		mrResData->mSamplerDesc.AddressV		= cWrapTranslate[pExportInfo->meWrapV];
		mrResData->mSamplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
		mrResData->mSamplerDesc.MipLODBias		= pExportInfo->mfLodBias;
		mrResData->mSamplerDesc.BorderColor[0]	= pExportInfo->mvBorderColor.r * 0xFF;
		mrResData->mSamplerDesc.BorderColor[1]	= pExportInfo->mvBorderColor.g * 0xFF;
		mrResData->mSamplerDesc.BorderColor[2]	= pExportInfo->mvBorderColor.b * 0xFF;
		mrResData->mSamplerDesc.BorderColor[3]	= pExportInfo->mvBorderColor.a * 0xFF;
		mrResData->mSamplerDesc.MaxAnisotropy	= 16;
		mrResData->mSamplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;		
		mrResData->mSamplerDesc.MinLOD			= 0;
		mrResData->mSamplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;
		return TRUE;
	}

	zResID ExporterGfxSamplerDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxSampler);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxSampler* pExportInfo	= static_cast<const ExportInfoGfxSampler*>(_pExportInfo);
		zUInt uSize								= ZENMemberOffsetAfter(ExportInfoGfxSampler, mvBorderColor) - ZENMemberOffset(ExportInfoGfxSampler, meFilterMin);
		zResID::NameHash hName((void*)&pExportInfo->meFilterMin, uSize); 
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//SerialGfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxStateBlendDX11_DX11::ExporterGfxStateBlendDX11_DX11(const ResDataRef& _rResData)
	: ExporterBase(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxStateBlendDX11_DX11::ExportWork(bool _bIsTHRTask)
	{		
		ExportInfoGfxStateBlend* pExportInfo = static_cast<ExportInfoGfxStateBlend*>(mpExportInfo);
		ZENAssert(mrResData.IsValid());		
		ZENAssert(gaBlendValueTranslator.Count() == zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal__Count);
		ZENAssert(gaBlendOpTranslator.Count() == zenRes::zGfxRenderPass::ConfigColorRT::keBlendOp__Count);		
		ZENAssert( pExportInfo->mpaRenderTargetConfig && pExportInfo->mpaRenderTargetConfig->Count() < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT );

		zenMem::Zero(&mrResData->mBlendDesc, sizeof(mrResData->mBlendDesc));
		mrResData->mBlendDesc.AlphaToCoverageEnable		= false;	//!< Currently not implemented
		mrResData->mBlendDesc.IndependentBlendEnable	= true;		
		for(zUInt rtIdx(0), rtCount(pExportInfo->mpaRenderTargetConfig->Count()); rtIdx<rtCount && rtIdx<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++rtIdx)
		{			
			const zenRes::zGfxRenderPass::ConfigColorRT& RtConfigIn	= (*pExportInfo->mpaRenderTargetConfig)[rtIdx];
			D3D11_RENDER_TARGET_BLEND_DESC& RtConfigOut				= mrResData->mBlendDesc.RenderTarget[rtIdx];
			RtConfigOut.BlendEnable									= RtConfigIn.mbBlendEnable;
			RtConfigOut.SrcBlend									= gaBlendValueTranslator[RtConfigIn.meBlendColorSrc];
			RtConfigOut.DestBlend									= gaBlendValueTranslator[RtConfigIn.meBlendColorDest];
			RtConfigOut.BlendOp										= gaBlendOpTranslator[RtConfigIn.meBlendColorOp];
			RtConfigOut.SrcBlendAlpha								= gaBlendValueTranslator[RtConfigIn.meBlendAlphaSrc];
			RtConfigOut.DestBlendAlpha								= gaBlendValueTranslator[RtConfigIn.meBlendAlphaDest];
			RtConfigOut.BlendOpAlpha								= gaBlendOpTranslator[RtConfigIn.meBlendAlphaOp];
			RtConfigOut.RenderTargetWriteMask						 = RtConfigIn.mWriteMask.All(zenConst::keColor_R) ? D3D11_COLOR_WRITE_ENABLE_RED : 0;
			RtConfigOut.RenderTargetWriteMask						|= RtConfigIn.mWriteMask.All(zenConst::keColor_G) ? D3D11_COLOR_WRITE_ENABLE_GREEN : 0;
			RtConfigOut.RenderTargetWriteMask						|= RtConfigIn.mWriteMask.All(zenConst::keColor_B) ? D3D11_COLOR_WRITE_ENABLE_BLUE : 0;
			RtConfigOut.RenderTargetWriteMask						|= RtConfigIn.mWriteMask.All(zenConst::keColor_A) ? D3D11_COLOR_WRITE_ENABLE_ALPHA : 0;
		}
		 
		return TRUE;
	}

	zResID ExporterGfxStateBlendDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxBlend);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxStateBlend* pExportInfo	= static_cast<const ExportInfoGfxStateBlend*>(_pExportInfo);
		ZENAssert( pExportInfo->mpaRenderTargetConfig->IsEmpty()==false );
		zResID::NameHash hName((void*)pExportInfo->mpaRenderTargetConfig->First(), pExportInfo->mpaRenderTargetConfig->SizeMem());
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	ZENInline void SetDepthStencilOpDesc(D3D11_DEPTH_STENCILOP_DESC& _StencilConfigDX11Out, const zenRes::zGfxRenderPass::ConfigDepthRT::ConfigStencil& _StencilConfigIn)
	{		
		ZENAssert( gaStencilOpTranslator.Count() == zenRes::zGfxRenderPass::ConfigDepthRT::keStencilOp__Count );
		_StencilConfigDX11Out.StencilFunc			= gaComparisonTestTranslator[_StencilConfigIn.meStencilTest];
		_StencilConfigDX11Out.StencilFailOp			= gaStencilOpTranslator[_StencilConfigIn.meStencilFailOp];		
		_StencilConfigDX11Out.StencilPassOp			= gaStencilOpTranslator[_StencilConfigIn.meStencilPassOp];
		_StencilConfigDX11Out.StencilDepthFailOp	= gaStencilOpTranslator[_StencilConfigIn.meDepthFailOp];		
	}

	ExporterGfxStateDepthStencilDX11_DX11::ExporterGfxStateDepthStencilDX11_DX11(const ResDataRef& _rResData)
	: ExporterBase(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxStateDepthStencilDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrResData.IsValid());		
		ZENAssert( gaComparisonTestTranslator.Count() == zenRes::zGfxRenderPass::ConfigDepthRT::keCmpTest__Count );
		ExportInfoGfxStateDepthStencil* pExportInfo				= static_cast<ExportInfoGfxStateDepthStencil*>(mpExportInfo);
		const zenRes::zGfxRenderPass::ConfigDepthRT* pConfig	= pExportInfo->mpDepthStencilConfig;
		if( pConfig )
		{
			mrResData->mDepthStencilDesc.DepthEnable			= pConfig->mbDepthEnable;
			mrResData->mDepthStencilDesc.DepthWriteMask			= pConfig->mbDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			mrResData->mDepthStencilDesc.StencilEnable			= pConfig->mbStencilEnable;
			mrResData->mDepthStencilDesc.DepthFunc				= gaComparisonTestTranslator[pConfig->meDepthTest];
			mrResData->mDepthStencilDesc.StencilReadMask		= pConfig->muStencilReadMask;
			mrResData->mDepthStencilDesc.StencilWriteMask		= pConfig->muStencilWriteMask;
			SetDepthStencilOpDesc(mrResData->mDepthStencilDesc.FrontFace, pConfig->meStencilConfigFront);
			SetDepthStencilOpDesc(mrResData->mDepthStencilDesc.BackFace,  pConfig->meStencilConfigBack);
		}
		return TRUE;
	}

	zResID ExporterGfxStateDepthStencilDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxDepthStencil);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxStateDepthStencil* pExportInfo	= static_cast<const ExportInfoGfxStateDepthStencil*>(_pExportInfo);
		ZENAssert( pExportInfo->mpDepthStencilConfig );
		zResID::NameHash hName((void*)pExportInfo->mpDepthStencilConfig, sizeof(zenRes::zGfxRenderPass::ConfigDepthRT));
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxStateRasterizerDX11_DX11::ExporterGfxStateRasterizerDX11_DX11(const ResDataRef& _rResData)
	: ExporterBase(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxStateRasterizerDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrResData.IsValid());
		ExportInfoGfxStateRasterizer* pExportInfo			= static_cast<ExportInfoGfxStateRasterizer*>(mpExportInfo);
		zenRes::zGfxStateRasterizer::Config& Config			= pExportInfo->mRasterConfig;
		mrResData->mRasterizerDesc.AntialiasedLineEnable	= pExportInfo->mRasterConfig.mbAntialiasedLineEnable;
		mrResData->mRasterizerDesc.CullMode					= (Config.mbShowBackface && Config.mbShowFrontface) ? D3D11_CULL_NONE : Config.mbShowBackface ? D3D11_CULL_FRONT : D3D11_CULL_BACK;
		mrResData->mRasterizerDesc.DepthBias				= zInt(Config.mfDepthBias);
		mrResData->mRasterizerDesc.DepthBiasClamp			= Config.mfDepthBiasClamp;
		mrResData->mRasterizerDesc.DepthClipEnable			= Config.mbDepthClipEnable;
		mrResData->mRasterizerDesc.FillMode					= Config.mbWireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		mrResData->mRasterizerDesc.FrontCounterClockwise	= !Config.mbFrontClockwise;
		mrResData->mRasterizerDesc.MultisampleEnable		= Config.mbMultisampleEnable;
		mrResData->mRasterizerDesc.ScissorEnable			= Config.mbScissorEnable;
		mrResData->mRasterizerDesc.SlopeScaledDepthBias		= Config.mfSlopeScaledDepthBias;
		return TRUE;
	}

	zResID ExporterGfxStateRasterizerDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxRasterizer);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxStateRasterizer* pExportInfo	= static_cast<const ExportInfoGfxStateRasterizer*>(_pExportInfo);
		zResID::NameHash hName((void*)&pExportInfo->mRasterConfig, sizeof(zenRes::zGfxStateRasterizer::Config) );
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

}

#endif //ZEN_EXPORT_OR_RESOURCE_DX11
