#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
	static D3D11_COMPARISON_FUNC eComparisonFuncTranslator[] = { D3D11_COMPARISON_NEVER, D3D11_COMPARISON_LESS, D3D11_COMPARISON_EQUAL, D3D11_COMPARISON_LESS_EQUAL,
		D3D11_COMPARISON_GREATER, D3D11_COMPARISON_NOT_EQUAL, D3D11_COMPARISON_GREATER_EQUAL, D3D11_COMPARISON_ALWAYS };
	//////////////////////////////////////////////////////////////////////////
	//SerialGfxSampler_DX11
	//////////////////////////////////////////////////////////////////////////
	bool SerialGfxSampler_DX11::ExportWork(bool _bIsTHRTask)
	{
		ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);		
		
		ZeroMemory( &mSerial.mSamplerDesc, sizeof(mSerial.mSamplerDesc) );
		mSerial.mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if( pExportInfo->meFilterMin == awconst::keTexFilter_Anisotropic || pExportInfo->meFilterMag == awconst::keTexFilter_Anisotropic )
		{
			mSerial.mSamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		}
		else if( pExportInfo->meFilterMin == awconst::keTexFilter_Trilinear || pExportInfo->meFilterMag == awconst::keTexFilter_Trilinear )
		{
			mSerial.mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}
		else
		{
			switch( pExportInfo->meFilterMag )
			{
			case awconst::keTexFilter_Point:
			{
				switch( pExportInfo->meFilterMin )
				{
				case awconst::keTexFilter_Point:	mSerial.mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;				break;
				case awconst::keTexFilter_Bilinear:	mSerial.mSamplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;		break;		
				}break;
			}
			case awconst::keTexFilter_Bilinear:
			{
				switch( pExportInfo->meFilterMin )
				{
				case awconst::keTexFilter_Point:	mSerial.mSamplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;	break;
				case awconst::keTexFilter_Bilinear:	mSerial.mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;		break;		
				}break;
			}
			}
		}

		const D3D11_TEXTURE_ADDRESS_MODE cWrapTranslate[]= { D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_BORDER };
		AWStaticAssert( AWArrayCount(cWrapTranslate)==awconst::keTexWrap__Count );

		mSerial.mSamplerDesc.AddressU		= cWrapTranslate[pExportInfo->meWrapU];
		mSerial.mSamplerDesc.AddressV		= cWrapTranslate[pExportInfo->meWrapV];
		mSerial.mSamplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
		mSerial.mSamplerDesc.MipLODBias		= pExportInfo->mfLodBias;
		mSerial.mSamplerDesc.BorderColor[0]	= pExportInfo->mvBorderColor.r * 0xFF;
		mSerial.mSamplerDesc.BorderColor[1]	= pExportInfo->mvBorderColor.g * 0xFF;
		mSerial.mSamplerDesc.BorderColor[2]	= pExportInfo->mvBorderColor.b * 0xFF;
		mSerial.mSamplerDesc.BorderColor[3]	= pExportInfo->mvBorderColor.a * 0xFF;
		mSerial.mSamplerDesc.MaxAnisotropy	= 16;
		mSerial.mSamplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;		
		mSerial.mSamplerDesc.MinLOD			= 0;
		mSerial.mSamplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;
		return TRUE;
	}

	awResourceID SerialGfxSampler_DX11::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		AWAssert(_ePlatform==awconst::keResPlatform_DX11 && _eType==awconst::keResType_GfxSampler);
		AWAssert( _pExportInfo );
		const ExportInfo* pExportInfo	= static_cast<const ExportInfo*>(_pExportInfo);
		awUInt uSize					= AWMemberOffsetAfter(SerialSampler_Base::ExportInfo, mvBorderColor) - AWMemberOffset(SerialSampler_Base::ExportInfo, meFilterMin);
		awResourceID::NameHash hName((void*)&pExportInfo->meFilterMin, uSize); 
		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	bool SerialGfxSampler_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		if( Super::Serialize(_Serializer) )
			return false;

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxBlend_DX11
	//////////////////////////////////////////////////////////////////////////
	void SetRenderTargetBlendDesc(D3D11_RENDER_TARGET_BLEND_DESC& xOut, const awtype::awBlendDesc::awRTBlendDesc& xIn)
	{
		static D3D11_BLEND eBlendTranslator[] = {D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_DEST_ALPHA,
			D3D11_BLEND_INV_DEST_ALPHA, D3D11_BLEND_DEST_COLOR, D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_SRC_ALPHA_SAT, D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR, D3D11_BLEND_SRC1_COLOR,
			D3D11_BLEND_INV_SRC1_COLOR, D3D11_BLEND_SRC1_ALPHA, D3D11_BLEND_INV_SRC1_ALPHA};
		static D3D11_BLEND_OP eBlendOpTranslator[] = {D3D11_BLEND_OP_ADD, D3D11_BLEND_OP_SUBTRACT, D3D11_BLEND_OP_REV_SUBTRACT, D3D11_BLEND_OP_MIN, D3D11_BLEND_OP_MAX};

		xOut.BlendEnable			= xIn.mbBlendEnable ? TRUE : FALSE;
		xOut.SrcBlend				= eBlendTranslator[xIn.meSrcBlend];
		xOut.DestBlend				= eBlendTranslator[xIn.meDestBlend];
		xOut.BlendOp				= eBlendOpTranslator[xIn.meBlendOp];
		xOut.SrcBlendAlpha			= eBlendTranslator[xIn.meSrcBlendAlpha];
		xOut.DestBlendAlpha			= eBlendTranslator[xIn.meDestBlendAlpha];
		xOut.BlendOpAlpha			= eBlendOpTranslator[xIn.meBlendOpAlpha];
		xOut.RenderTargetWriteMask	= xIn.muRenderTargetWriteMask;
	}
	bool SerialGfxBlend_DX11::ExportWork(bool _bIsTHRTask)
	{
		ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);

		mSerial.mBlendDesc.AlphaToCoverageEnable = pExportInfo->mxBlendDesc.mbAlphaToCoverageEnable;
		mSerial.mBlendDesc.IndependentBlendEnable= pExportInfo->mxBlendDesc.mbIndependentBlendEnable;
		for(awU8 u = 0; u < 8; ++u)
		{
			SetRenderTargetBlendDesc(mSerial.mBlendDesc.RenderTarget[u], pExportInfo->mxBlendDesc.mxRenderTarget[u]);
		}
		return TRUE;
	}

	awResourceID SerialGfxBlend_DX11::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		AWAssert(_ePlatform==awconst::keResPlatform_DX11 && _eType==awconst::keResType_GfxBlend);
		AWAssert( _pExportInfo );
		const ExportInfo* pExportInfo	= static_cast<const ExportInfo*>(_pExportInfo);
		awUInt uSize					= AWMemberOffsetAfter(SerialBlend_Base::ExportInfo, mxBlendDesc) - AWMemberOffset(SerialBlend_Base::ExportInfo, mxBlendDesc);
		awResourceID::NameHash hName((void*)&pExportInfo->mxBlendDesc, uSize);
		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	bool SerialGfxBlend_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		if( Super::Serialize(_Serializer) )
			return false;

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxDepthStencil_DX11
	//////////////////////////////////////////////////////////////////////////
	static void SetDepthStencilOpDesc(D3D11_DEPTH_STENCILOP_DESC& xOut, const awtype::awDepthStencilDesc::DepthStencilOp& xIn)
	{
		static D3D11_STENCIL_OP eStencilOpTranslator[] = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_INCR_SAT,
			D3D11_STENCIL_OP_DECR_SAT, D3D11_STENCIL_OP_INVERT, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_DECR };

		xOut.StencilFailOp		= eStencilOpTranslator[xIn.meStencilFailOp];
		xOut.StencilDepthFailOp	= eStencilOpTranslator[xIn.meStencilDepthFailOp];
		xOut.StencilPassOp		= eStencilOpTranslator[xIn.meStencilPassOp];
		xOut.StencilFunc		= eComparisonFuncTranslator[xIn.meStencilFunc];
	}
	bool SerialGfxDepthStencil_DX11::ExportWork(bool _bIsTHRTask)
	{
		ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);

		mSerial.mDepthStencilDesc.DepthEnable		= pExportInfo->mxDepthStencilDesc.mbDepthEnable ? TRUE : FALSE;
		mSerial.mDepthStencilDesc.DepthWriteMask	= pExportInfo->mxDepthStencilDesc.mbDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		mSerial.mDepthStencilDesc.StencilEnable		= pExportInfo->mxDepthStencilDesc.mbStencilEnable ? TRUE : FALSE;
		mSerial.mDepthStencilDesc.DepthFunc			= eComparisonFuncTranslator[pExportInfo->mxDepthStencilDesc.meDepthFunc];
		mSerial.mDepthStencilDesc.StencilReadMask	= pExportInfo->mxDepthStencilDesc.muStencilReadMask;
		mSerial.mDepthStencilDesc.StencilWriteMask	= pExportInfo->mxDepthStencilDesc.muStencilWriteMask;
		SetDepthStencilOpDesc(mSerial.mDepthStencilDesc.FrontFace, pExportInfo->mxDepthStencilDesc.mxFrontFace);
		SetDepthStencilOpDesc(mSerial.mDepthStencilDesc.BackFace,  pExportInfo->mxDepthStencilDesc.mxBackFace );
		return TRUE;
	}

	awResourceID SerialGfxDepthStencil_DX11::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		AWAssert(_ePlatform==awconst::keResPlatform_DX11 && _eType==awconst::keResType_GfxDepthStencil);
		AWAssert( _pExportInfo );
		const ExportInfo* pExportInfo	= static_cast<const ExportInfo*>(_pExportInfo);
		awUInt uSize					= AWMemberOffsetAfter(SerialDepthStencil_Base::ExportInfo, mxDepthStencilDesc) - AWMemberOffset(SerialDepthStencil_Base::ExportInfo, mxDepthStencilDesc);
		awResourceID::NameHash hName((void*)&pExportInfo->mxDepthStencilDesc, uSize);
		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	bool SerialGfxDepthStencil_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		if( Super::Serialize(_Serializer) )
			return false;

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//SerialGfxRasterizer_DX11
	//////////////////////////////////////////////////////////////////////////
	bool SerialGfxRasterizer_DX11::ExportWork(bool _bIsTHRTask)
	{
		static D3D11_CULL_MODE eCullModeTranslator[] = {D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK};
		ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);

		mSerial.mRasterizerDesc.AntialiasedLineEnable	= pExportInfo->mbAntialiasedLineEnable ? TRUE : FALSE;
		mSerial.mRasterizerDesc.CullMode				= eCullModeTranslator[pExportInfo->meCullMode];
		mSerial.mRasterizerDesc.DepthBias				= pExportInfo->miDepthBias;
		mSerial.mRasterizerDesc.DepthBiasClamp			= pExportInfo->mfDepthBiasClamp;
		mSerial.mRasterizerDesc.DepthClipEnable			= pExportInfo->mbDepthClipEnable ? TRUE : FALSE;
		mSerial.mRasterizerDesc.FillMode				= pExportInfo->mbWireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		mSerial.mRasterizerDesc.FrontCounterClockwise	= pExportInfo->mbFrontCounterClockwise ? TRUE : FALSE;
		mSerial.mRasterizerDesc.MultisampleEnable		= pExportInfo->mbMultisampleEnable ? TRUE : FALSE;
		mSerial.mRasterizerDesc.ScissorEnable			= pExportInfo->mbScissorEnable ? TRUE : FALSE;
		mSerial.mRasterizerDesc.SlopeScaledDepthBias	= pExportInfo->mfSlopeScaledDepthBias;
		return TRUE;
	}

	awResourceID SerialGfxRasterizer_DX11::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		AWAssert(_ePlatform==awconst::keResPlatform_DX11 && _eType==awconst::keResType_GfxRasterizer);
		AWAssert( _pExportInfo );
		const ExportInfo* pExportInfo	= static_cast<const ExportInfo*>(_pExportInfo);
		awUInt uSize					= AWMemberOffsetAfter(SerialRasterizer_Base::ExportInfo, mfSlopeScaledDepthBias) - AWMemberOffset(SerialRasterizer_Base::ExportInfo, mbFrontCounterClockwise);
		awResourceID::NameHash hName((void*)&pExportInfo->mbFrontCounterClockwise, uSize);
		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	bool SerialGfxRasterizer_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		if( Super::Serialize(_Serializer) )
			return false;

		return false;
	}
}

#endif //AW_EXPORT_OR_RESOURCE_DX11
