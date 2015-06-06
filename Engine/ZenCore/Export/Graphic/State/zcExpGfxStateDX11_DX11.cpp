#include "zcCore.h"

#if ZEN_EXPORT_DX11

namespace zcExp
{
	static const D3D11_COMPARISON_FUNC eComparisonFuncTranslator[] = {	D3D11_COMPARISON_NEVER, D3D11_COMPARISON_LESS, D3D11_COMPARISON_EQUAL, D3D11_COMPARISON_LESS_EQUAL,
																		D3D11_COMPARISON_GREATER, D3D11_COMPARISON_NOT_EQUAL, D3D11_COMPARISON_GREATER_EQUAL, D3D11_COMPARISON_ALWAYS };

	//////////////////////////////////////////////////////////////////////////
	//ExporterGfxSamplerDX11_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxSamplerDX11_DX11::ExporterGfxSamplerDX11_DX11(const ExportDataRef& _rExportData)
	: ExporterBase(_rExportData.GetSafe())
	, mrExportData(_rExportData)
	{
	}

	bool ExporterGfxSamplerDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrExportData.IsValid());
		ExportInfoGfxSampler* pExportInfo = static_cast<ExportInfoGfxSampler*>(mpExportInfo);
		ZeroMemory( &mrExportData->mSamplerDesc, sizeof(mrExportData->mSamplerDesc) );
		mrExportData->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if( pExportInfo->meFilterMin == zenConst::keTexFilter_Anisotropic || pExportInfo->meFilterMag == zenConst::keTexFilter_Anisotropic )
		{
			mrExportData->mSamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		}
		else if( pExportInfo->meFilterMin == zenConst::keTexFilter_Trilinear || pExportInfo->meFilterMag == zenConst::keTexFilter_Trilinear )
		{
			mrExportData->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}
		else
		{
			switch( pExportInfo->meFilterMag )
			{
			case zenConst::keTexFilter_Point:
			{
				switch( pExportInfo->meFilterMin )
				{
				case zenConst::keTexFilter_Point:		mrExportData->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;				break;
				case zenConst::keTexFilter_Bilinear:	mrExportData->mSamplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;		break;		
				}break;
			}
			case zenConst::keTexFilter_Bilinear:
			{
				switch( pExportInfo->meFilterMin )
				{
				case zenConst::keTexFilter_Point:		mrExportData->mSamplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;	break;
				case zenConst::keTexFilter_Bilinear:	mrExportData->mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;		break;		
				}break;
			}
			}
		}

		const D3D11_TEXTURE_ADDRESS_MODE cWrapTranslate[]= { D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_BORDER };
		ZENStaticAssert( ZENArrayCount(cWrapTranslate)==zenConst::keTexWrap__Count );

		mrExportData->mSamplerDesc.AddressU			= cWrapTranslate[pExportInfo->meWrapU];
		mrExportData->mSamplerDesc.AddressV			= cWrapTranslate[pExportInfo->meWrapV];
		mrExportData->mSamplerDesc.AddressW			= D3D11_TEXTURE_ADDRESS_CLAMP;
		mrExportData->mSamplerDesc.MipLODBias		= pExportInfo->mfLodBias;
		mrExportData->mSamplerDesc.BorderColor[0]	= pExportInfo->mvBorderColor.r * 0xFF;
		mrExportData->mSamplerDesc.BorderColor[1]	= pExportInfo->mvBorderColor.g * 0xFF;
		mrExportData->mSamplerDesc.BorderColor[2]	= pExportInfo->mvBorderColor.b * 0xFF;
		mrExportData->mSamplerDesc.BorderColor[3]	= pExportInfo->mvBorderColor.a * 0xFF;
		mrExportData->mSamplerDesc.MaxAnisotropy	= 16;
		mrExportData->mSamplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;		
		mrExportData->mSamplerDesc.MinLOD			= 0;
		mrExportData->mSamplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;
		return TRUE;
	}

	zResID ExporterGfxSamplerDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxSampler);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxSampler* pExportInfo	= static_cast<const ExportInfoGfxSampler*>(_pExportInfo);
		zUInt uSize							= ZENMemberOffsetAfter(ExportInfoGfxSampler, mvBorderColor) - ZENMemberOffset(ExportInfoGfxSampler, meFilterMin);
		zResID::NameHash hName((void*)&pExportInfo->meFilterMin, uSize); 
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//SerialGfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxStateBlendDX11_DX11::ExporterGfxStateBlendDX11_DX11(const ExportDataRef& _rExportData)
	: ExporterBase(_rExportData.GetSafe())
	, mrExportData(_rExportData)
	{
	}

	void SetRenderTargetBlendDesc(D3D11_RENDER_TARGET_BLEND_DESC& _Out, const zenType::zBlendDesc::zRTBlendDesc& _In)
	{
		static D3D11_BLEND eBlendTranslator[] = {D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_DEST_ALPHA,
			D3D11_BLEND_INV_DEST_ALPHA, D3D11_BLEND_DEST_COLOR, D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_SRC_ALPHA_SAT, D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR, D3D11_BLEND_SRC1_COLOR,
			D3D11_BLEND_INV_SRC1_COLOR, D3D11_BLEND_SRC1_ALPHA, D3D11_BLEND_INV_SRC1_ALPHA};
		static D3D11_BLEND_OP eBlendOpTranslator[] = {D3D11_BLEND_OP_ADD, D3D11_BLEND_OP_SUBTRACT, D3D11_BLEND_OP_REV_SUBTRACT, D3D11_BLEND_OP_MIN, D3D11_BLEND_OP_MAX};

		_Out.BlendEnable			= _In.mbBlendEnable ? TRUE : FALSE;
		_Out.SrcBlend				= eBlendTranslator[_In.meSrcBlend];
		_Out.DestBlend				= eBlendTranslator[_In.meDestBlend];
		_Out.BlendOp				= eBlendOpTranslator[_In.meBlendOp];
		_Out.SrcBlendAlpha			= eBlendTranslator[_In.meSrcBlendAlpha];
		_Out.DestBlendAlpha			= eBlendTranslator[_In.meDestBlendAlpha];
		_Out.BlendOpAlpha			= eBlendOpTranslator[_In.meBlendOpAlpha];
		_Out.RenderTargetWriteMask	= _In.muRenderTargetWriteMask;
	}
	bool ExporterGfxStateBlendDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrExportData.IsValid());
		ExportInfoGfxStateBlend* pExportInfo = static_cast<ExportInfoGfxStateBlend*>(mpExportInfo);
		mrExportData->mBlendDesc.AlphaToCoverageEnable = pExportInfo->mBlendDesc.mbAlphaToCoverageEnable;
		mrExportData->mBlendDesc.IndependentBlendEnable= pExportInfo->mBlendDesc.mbIndependentBlendEnable;
		for(zUInt u(0); u < 8; ++u)
			SetRenderTargetBlendDesc(mrExportData->mBlendDesc.RenderTarget[u], pExportInfo->mBlendDesc.mxRenderTarget[u]);

		return TRUE;
	}

	zResID ExporterGfxStateBlendDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxBlend);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxStateBlend* pExportInfo	= static_cast<const ExportInfoGfxStateBlend*>(_pExportInfo);
		zUInt uSize										= ZENMemberOffsetAfter(ExportInfoGfxStateBlend, mBlendDesc) - ZENMemberOffset(ExportInfoGfxStateBlend, mBlendDesc);
		zResID::NameHash hName((void*)&pExportInfo->mBlendDesc, uSize);
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxStateDepthStencilDX11_DX11::ExporterGfxStateDepthStencilDX11_DX11(const ExportDataRef& _rExportData)
	: ExporterBase(_rExportData.GetSafe())
	, mrExportData(_rExportData)
	{
	}

	static void SetDepthStencilOpDesc(D3D11_DEPTH_STENCILOP_DESC& xOut, const zenType::zDepthStencilDesc::DepthStencilOp& xIn)
	{
		static D3D11_STENCIL_OP eStencilOpTranslator[] = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_INCR_SAT,
			D3D11_STENCIL_OP_DECR_SAT, D3D11_STENCIL_OP_INVERT, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_DECR };

		xOut.StencilFailOp		= eStencilOpTranslator[xIn.meStencilFailOp];
		xOut.StencilDepthFailOp	= eStencilOpTranslator[xIn.meStencilDepthFailOp];
		xOut.StencilPassOp		= eStencilOpTranslator[xIn.meStencilPassOp];
		xOut.StencilFunc		= eComparisonFuncTranslator[xIn.meStencilFunc];
	}
	bool ExporterGfxStateDepthStencilDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		ZENAssert(mrExportData.IsValid());
		ExportInfoGfxStateDepthStencil* pExportInfo		= static_cast<ExportInfoGfxStateDepthStencil*>(mpExportInfo);
		mrExportData->mDepthStencilDesc.DepthEnable			= pExportInfo->mDepthStencilDesc.mbDepthEnable ? TRUE : FALSE;
		mrExportData->mDepthStencilDesc.DepthWriteMask		= pExportInfo->mDepthStencilDesc.mbDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		mrExportData->mDepthStencilDesc.StencilEnable		= pExportInfo->mDepthStencilDesc.mbStencilEnable ? TRUE : FALSE;
		mrExportData->mDepthStencilDesc.DepthFunc			= eComparisonFuncTranslator[pExportInfo->mDepthStencilDesc.meDepthFunc];
		mrExportData->mDepthStencilDesc.StencilReadMask		= pExportInfo->mDepthStencilDesc.muStencilReadMask;
		mrExportData->mDepthStencilDesc.StencilWriteMask	= pExportInfo->mDepthStencilDesc.muStencilWriteMask;
		SetDepthStencilOpDesc(mrExportData->mDepthStencilDesc.FrontFace, pExportInfo->mDepthStencilDesc.mxFrontFace);
		SetDepthStencilOpDesc(mrExportData->mDepthStencilDesc.BackFace,  pExportInfo->mDepthStencilDesc.mxBackFace );
		return TRUE;
	}

	zResID ExporterGfxStateDepthStencilDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxDepthStencil);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxStateDepthStencil* pExportInfo	= static_cast<const ExportInfoGfxStateDepthStencil*>(_pExportInfo);
		zUInt uSize											= ZENMemberOffsetAfter(ExportInfoGfxStateDepthStencil, mDepthStencilDesc) - ZENMemberOffset(ExportInfoGfxStateDepthStencil, mDepthStencilDesc);
		zResID::NameHash hName((void*)&pExportInfo->mDepthStencilDesc, uSize);
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	ExporterGfxStateRasterizerDX11_DX11::ExporterGfxStateRasterizerDX11_DX11(const ExportDataRef& _rExportData)
	: ExporterBase(_rExportData.GetSafe())
	, mrExportData(_rExportData)
	{
	}

	bool ExporterGfxStateRasterizerDX11_DX11::ExportWork(bool _bIsTHRTask)
	{
		static D3D11_CULL_MODE eCullModeTranslator[] = {D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK};
		ZENAssert(mrExportData.IsValid());
		ExportInfoGfxStateRasterizer* pExportInfo			= static_cast<ExportInfoGfxStateRasterizer*>(mpExportInfo);
		mrExportData->mRasterizerDesc.AntialiasedLineEnable	= pExportInfo->mbAntialiasedLineEnable ? TRUE : FALSE;
		mrExportData->mRasterizerDesc.CullMode				= eCullModeTranslator[pExportInfo->meCullMode];
		mrExportData->mRasterizerDesc.DepthBias				= pExportInfo->miDepthBias;
		mrExportData->mRasterizerDesc.DepthBiasClamp		= pExportInfo->mfDepthBiasClamp;
		mrExportData->mRasterizerDesc.DepthClipEnable		= pExportInfo->mbDepthClipEnable ? TRUE : FALSE;
		mrExportData->mRasterizerDesc.FillMode				= pExportInfo->mbWireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		mrExportData->mRasterizerDesc.FrontCounterClockwise	= pExportInfo->mbFrontCounterClockwise ? TRUE : FALSE;
		mrExportData->mRasterizerDesc.MultisampleEnable		= pExportInfo->mbMultisampleEnable ? TRUE : FALSE;
		mrExportData->mRasterizerDesc.ScissorEnable			= pExportInfo->mbScissorEnable ? TRUE : FALSE;
		mrExportData->mRasterizerDesc.SlopeScaledDepthBias	= pExportInfo->mfSlopeScaledDepthBias;
		return TRUE;
	}

	zResID ExporterGfxStateRasterizerDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxRasterizer);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxStateRasterizer* pExportInfo	= static_cast<const ExportInfoGfxStateRasterizer*>(_pExportInfo);
		zUInt uSize											= ZENMemberOffsetAfter(ExportInfoGfxStateRasterizer, mfSlopeScaledDepthBias) - ZENMemberOffset(ExportInfoGfxStateRasterizer, mbFrontCounterClockwise);
		zResID::NameHash hName((void*)&pExportInfo->mbFrontCounterClockwise, uSize);
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

}

#endif //ZEN_EXPORT_OR_RESOURCE_DX11
