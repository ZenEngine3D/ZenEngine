#include "zcCore.h"

namespace zcExp
{
	static const zArrayFixed<D3D11_COMPARISON_FUNC,8> gaComparisonTestTranslator = {	
			D3D11_COMPARISON_NEVER,				// keCmpTest_Never
			D3D11_COMPARISON_LESS, 				// keCmpTest_Less
			D3D11_COMPARISON_EQUAL, 			// keCmpTest_Equal
			D3D11_COMPARISON_LESS_EQUAL,		// keCmpTest_LessEqual
			D3D11_COMPARISON_GREATER,			// keCmpTest_Greater
			D3D11_COMPARISON_NOT_EQUAL, 		// keCmpTest_NotEqual
			D3D11_COMPARISON_GREATER_EQUAL, 	// keCmpTest_GreaterEqual
			D3D11_COMPARISON_ALWAYS 			// keCmpTest_Always
			};

	static const zArrayFixed<D3D11_STENCIL_OP,8> gaStencilOpTranslator = { 
			D3D11_STENCIL_OP_KEEP,				// keStencilOp_Keep,
			D3D11_STENCIL_OP_ZERO, 				// keStencilOp_Zero,
			D3D11_STENCIL_OP_REPLACE,			// keStencilOp_Replace,
			D3D11_STENCIL_OP_INCR_SAT,			// keStencilOp_IncrSat,
			D3D11_STENCIL_OP_DECR_SAT,			// keStencilOp_DecrSat,
			D3D11_STENCIL_OP_INVERT,			// keStencilOp_Invert,
			D3D11_STENCIL_OP_INCR, 				// keStencilOp_Incr,
			D3D11_STENCIL_OP_DECR				// keStencilOp_Decr
			};

	static const zArrayFixed<D3D11_BLEND,13> gaBlendValueTranslator = {
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

	static const zArrayFixed<D3D11_BLEND_OP,5> gaBlendOpTranslator = {
			D3D11_BLEND_OP_ADD,					// keBlendOp_Add, 
			D3D11_BLEND_OP_SUBTRACT, 			// keBlendOp_Substract, 
			D3D11_BLEND_OP_REV_SUBTRACT, 		// keBlendOp_RevSubstract,
			D3D11_BLEND_OP_MIN, 				// keBlendOp_Min, 
			D3D11_BLEND_OP_MAX					// keBlendOp_Max, 
			};

	//////////////////////////////////////////////////////////////////////////
	//ExporterGfxStateSamplerDX11_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxStateSamplerDX11_DX11::ExporterGfxStateSamplerDX11_DX11(const ExportResultRef& _rExportOut)
	: ExporterBase(_rExportOut.GetSafe())
	, mrExport(_rExportOut)
	{
		zenAssert(mrExport.IsValid());
	}
	bool ExporterGfxStateSamplerDX11_DX11::ExportWork(bool _bIsTHRTask)
	{		
		ExportInfoGfxStateSampler* pExportInfo = static_cast<ExportInfoGfxStateSampler*>(mpExportInfo);
		ZeroMemory( &mrExport->mSamplerDesc, sizeof(mrExport->mSamplerDesc) );
		mrExport->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if( pExportInfo->meFilterMin == zenConst::keTexFilter_Anisotropic || pExportInfo->meFilterMag == zenConst::keTexFilter_Anisotropic )
		{
			mrExport->mSamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		}
		else if( pExportInfo->meFilterMin == zenConst::keTexFilter_Trilinear || pExportInfo->meFilterMag == zenConst::keTexFilter_Trilinear )
		{
			mrExport->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}
		else
		{
			switch( pExportInfo->meFilterMag )
			{
			case zenConst::keTexFilter_Point:
			{
				switch( pExportInfo->meFilterMin )
				{
				case zenConst::keTexFilter_Point:		mrExport->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;		break;
				case zenConst::keTexFilter_Bilinear:	mrExport->mSamplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;	break;		
				}break;
			}
			case zenConst::keTexFilter_Bilinear:
			{
				switch( pExportInfo->meFilterMin )
				{
				case zenConst::keTexFilter_Point:		mrExport->mSamplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;	break;
				case zenConst::keTexFilter_Bilinear:	mrExport->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;			break;		
				}break;
			}
			}
		}

		const D3D11_TEXTURE_ADDRESS_MODE cWrapTranslate[]= { D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_BORDER };
		zenStaticAssert( zenArrayCount(cWrapTranslate)==zenConst::keTexWrap__Count );

		mrExport->mSamplerDesc.AddressU			= cWrapTranslate[pExportInfo->meWrapU];
		mrExport->mSamplerDesc.AddressV			= cWrapTranslate[pExportInfo->meWrapV];
		mrExport->mSamplerDesc.AddressW			= D3D11_TEXTURE_ADDRESS_CLAMP;
		mrExport->mSamplerDesc.MipLODBias		= pExportInfo->mfLodBias;
		mrExport->mSamplerDesc.BorderColor[0]	= pExportInfo->mvBorderColor.r * 0xFF;
		mrExport->mSamplerDesc.BorderColor[1]	= pExportInfo->mvBorderColor.g * 0xFF;
		mrExport->mSamplerDesc.BorderColor[2]	= pExportInfo->mvBorderColor.b * 0xFF;
		mrExport->mSamplerDesc.BorderColor[3]	= pExportInfo->mvBorderColor.a * 0xFF;
		mrExport->mSamplerDesc.MaxAnisotropy	= 16;
		mrExport->mSamplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;		
		mrExport->mSamplerDesc.MinLOD			= 0;
		mrExport->mSamplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;
		return TRUE;
	}

	zResID ExporterGfxStateSamplerDX11_DX11::CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		zenAssert(_ePlatform==zenConst::kePlatform_DX11 && _eType==zenConst::keResType_GfxStateSampler);
		zenAssert( _pExportInfo );
		const ExportInfoGfxStateSampler* pExportInfo	= static_cast<const ExportInfoGfxStateSampler*>(_pExportInfo);
		zUInt uSize								= zenOffsetAfter(&ExportInfoGfxStateSampler::mvBorderColor) - zenOffsetOf(&ExportInfoGfxStateSampler::meFilterMin);
		zResID::NameHash hName((void*)&pExportInfo->meFilterMin, uSize); 
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//SerialGfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxStateBlendDX11_DX11::ExporterGfxStateBlendDX11_DX11(const ExportResultRef& _rExportOut)
	: ExporterBase(_rExportOut.GetSafe())
	, mrExport(_rExportOut)
	{
		zenAssert(mrExport.IsValid());
	}

	bool ExporterGfxStateBlendDX11_DX11::ExportWork(bool _bIsTHRTask)
	{		
		ExportInfoGfxStateBlend* pExportInfo = static_cast<ExportInfoGfxStateBlend*>(mpExportInfo);		
		zenAssert(gaBlendValueTranslator.size() == zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal__Count);
		zenAssert(gaBlendOpTranslator.size() == zenRes::zGfxRenderPass::ConfigColorRT::keBlendOp__Count);		
		zenAssert( pExportInfo->mpaRenderTargetConfig && pExportInfo->mpaRenderTargetConfig->size() < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT );

		zenMem::Zero(&mrExport->mBlendDesc, sizeof(mrExport->mBlendDesc));
		mrExport->mBlendDesc.AlphaToCoverageEnable		= false;	//!< Currently not implemented
		mrExport->mBlendDesc.IndependentBlendEnable	= true;		
		for(zUInt rtIdx(0), rtCount(pExportInfo->mpaRenderTargetConfig->size()); rtIdx<rtCount && rtIdx<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++rtIdx)
		{			
			const zenRes::zGfxRenderPass::ConfigColorRT& RtConfigIn	= (*pExportInfo->mpaRenderTargetConfig)[rtIdx];
			D3D11_RENDER_TARGET_BLEND_DESC& RtConfigOut				= mrExport->mBlendDesc.RenderTarget[rtIdx];
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

	zResID ExporterGfxStateBlendDX11_DX11::CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		zenAssert(_ePlatform==zenConst::kePlatform_DX11 && _eType==zenConst::keResType_GfxStateBlend);
		zenAssert( _pExportInfo );
		const ExportInfoGfxStateBlend* pExportInfo	= static_cast<const ExportInfoGfxStateBlend*>(_pExportInfo);
		zenAssert( pExportInfo->mpaRenderTargetConfig->empty()==false );
		zResID::NameHash hName((void*)&pExportInfo->mpaRenderTargetConfig->front(), pExportInfo->mpaRenderTargetConfig->SizeMem());
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	zenInline void SetDepthStencilOpDesc(D3D11_DEPTH_STENCILOP_DESC& _StencilConfigDX11Out, const zenRes::zGfxRenderPass::ConfigDepthRT::ConfigStencil& _StencilConfigIn)
	{		
		zenAssert( gaStencilOpTranslator.size() == zenRes::zGfxRenderPass::ConfigDepthRT::keStencilOp__Count );
		_StencilConfigDX11Out.StencilFunc			= gaComparisonTestTranslator[_StencilConfigIn.meStencilTest];
		_StencilConfigDX11Out.StencilFailOp			= gaStencilOpTranslator[_StencilConfigIn.meStencilFailOp];		
		_StencilConfigDX11Out.StencilPassOp			= gaStencilOpTranslator[_StencilConfigIn.meStencilPassOp];
		_StencilConfigDX11Out.StencilDepthFailOp	= gaStencilOpTranslator[_StencilConfigIn.meDepthFailOp];		
	}

	ExporterGfxStateDepthStencilDX11_DX11::ExporterGfxStateDepthStencilDX11_DX11(const ExportResultRef& _rExportOut)
	: ExporterBase(_rExportOut.GetSafe())
	, mrExport(_rExportOut)
	{
		zenAssert(_rExportOut.IsValid());
	}

	bool ExporterGfxStateDepthStencilDX11_DX11::ExportWork(bool _bIsTHRTask)
	{		
		zenAssert( gaComparisonTestTranslator.size() == zenRes::zGfxRenderPass::ConfigDepthRT::keCmpTest__Count );
		ExportInfoGfxStateDepthStencil* pExportInfo				= static_cast<ExportInfoGfxStateDepthStencil*>(mpExportInfo);
		const zenRes::zGfxRenderPass::ConfigDepthRT* pConfig	= pExportInfo->mpDepthStencilConfig;
		if( pConfig )
		{
			mrExport->mDepthStencilDesc.DepthEnable			= pConfig->mbDepthEnable;
			mrExport->mDepthStencilDesc.DepthWriteMask		= pConfig->mbDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			mrExport->mDepthStencilDesc.StencilEnable		= pConfig->mbStencilEnable;
			mrExport->mDepthStencilDesc.DepthFunc			= gaComparisonTestTranslator[pConfig->meDepthTest];
			mrExport->mDepthStencilDesc.StencilReadMask		= pConfig->muStencilReadMask;
			mrExport->mDepthStencilDesc.StencilWriteMask	= pConfig->muStencilWriteMask;
			SetDepthStencilOpDesc(mrExport->mDepthStencilDesc.FrontFace, pConfig->meStencilConfigFront);
			SetDepthStencilOpDesc(mrExport->mDepthStencilDesc.BackFace,  pConfig->meStencilConfigBack);
		}
		return TRUE;
	}

	zResID ExporterGfxStateDepthStencilDX11_DX11::CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		zenAssert(_ePlatform==zenConst::kePlatform_DX11 && _eType==zenConst::keResType_GfxStateDepthStencil);
		zenAssert( _pExportInfo );
		const ExportInfoGfxStateDepthStencil* pExportInfo	= static_cast<const ExportInfoGfxStateDepthStencil*>(_pExportInfo);
		zenAssert( pExportInfo->mpDepthStencilConfig );
		zResID::NameHash hName((void*)pExportInfo->mpDepthStencilConfig, sizeof(zenRes::zGfxRenderPass::ConfigDepthRT));
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxStateRasterDX11_DX11::ExporterGfxStateRasterDX11_DX11(const ExportResultRef& _rExportOut)
	: ExporterBase(_rExportOut.GetSafe())
	, mrExport(_rExportOut)
	{
		zenAssert(mrExport.IsValid());
	}

	bool ExporterGfxStateRasterDX11_DX11::ExportWork(bool _bIsTHRTask)
	{		
		ExportInfoGfxStateRaster* pExportInfo			= static_cast<ExportInfoGfxStateRaster*>(mpExportInfo);
		zenRes::zGfxStateRaster::Config& Config			= pExportInfo->mRasterConfig;
		mrExport->mRasterizerDesc.AntialiasedLineEnable	= pExportInfo->mRasterConfig.mbAntialiasedLineEnable;
		mrExport->mRasterizerDesc.CullMode				= (Config.mbShowBackface && Config.mbShowFrontface) ? D3D11_CULL_NONE : Config.mbShowBackface ? D3D11_CULL_FRONT : D3D11_CULL_BACK;
		mrExport->mRasterizerDesc.DepthBias				= zInt(Config.mfDepthBias);
		mrExport->mRasterizerDesc.DepthBiasClamp		= Config.mfDepthBiasClamp;
		mrExport->mRasterizerDesc.DepthClipEnable		= Config.mbDepthClipEnable;
		mrExport->mRasterizerDesc.FillMode				= Config.mbWireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		mrExport->mRasterizerDesc.FrontCounterClockwise	= !Config.mbFrontClockwise;
		mrExport->mRasterizerDesc.MultisampleEnable		= Config.mbMultisampleEnable;
		mrExport->mRasterizerDesc.ScissorEnable			= Config.mbScissorEnable;
		mrExport->mRasterizerDesc.SlopeScaledDepthBias	= Config.mfSlopeScaledDepthBias;
		return TRUE;
	}

	zResID ExporterGfxStateRasterDX11_DX11::CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		zenAssert(_ePlatform==zenConst::kePlatform_DX11 && _eType==zenConst::keResType_GfxStateRaster);
		zenAssert( _pExportInfo );
		const ExportInfoGfxStateRaster* pExportInfo	= static_cast<const ExportInfoGfxStateRaster*>(_pExportInfo);
		zResID::NameHash hName((void*)&pExportInfo->mRasterConfig, sizeof(zenRes::zGfxStateRaster::Config) );
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

}
