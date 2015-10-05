#pragma once
#ifndef __zenApi_Core_ResGfxState_h__
#define __zenApi_Core_ResGfxState_h__

namespace zen { namespace zenRes {	

	ZENClassResourceRefDeclare(zGfxSampler, zenConst::keResType_GfxSampler)
	public:
		static zGfxSampler				Create( zenConst::eTextureFiltering _eFilterMin=zenConst::keTexFilter_Bilinear, zenConst::eTextureFiltering _eFilterMag=zenConst::keTexFilter_Bilinear, zenConst::eTextureWrap _eWrapU=zenConst::keTexWrap_Repeat, zenConst::eTextureWrap _eWrapV=zenConst::keTexWrap_Repeat, float _fLodBias=0, const zVec4F& _vBorderColor=zVec4F(0,0,0,1) );
	};

	ZENClassResourceRefDeclare(zGfxStateBlend, zenConst::keResType_GfxBlend)
	public:
		static zGfxStateBlend			Create( zenType::zBlendDesc* _pBlendDesc=nullptr );
	};

	ZENClassResourceRefDeclare(zGfxStateDepthStencil, zenConst::keResType_GfxDepthStencil)
	public:
		static zGfxStateDepthStencil	Create( bool _bDepthEnable = false, bool _bDepthWrite = false, bool _bStencilEnable = false, zU8 _uStencilReadMask = 0xFF, zU8 _uStencilWriteMask = 0xFF, zenConst::eComparisonFunc _eDepthFunc = zenConst::keComparisonFunc_Always, zenType::zDepthStencilDesc::DepthStencilOp _xFrontFace = zenType::zDepthStencilDesc::DepthStencilOp(), zenType::zDepthStencilDesc::DepthStencilOp _xBackFace = zenType::zDepthStencilDesc::DepthStencilOp() );
	};

	ZENClassResourceRefDeclare(zGfxStateRasterizer, zenConst::keResType_GfxRasterizer)
	public:
		static zGfxStateRasterizer		Create( bool _bFrontCounterClockwise = false, bool _bDepthClipEnable = false, bool _bScissorEnable = false, bool _bMultisampleEnable = false, bool _bAntialiasedLineEnable = false, bool _bWireFrame = false, zenConst::eCullMode _eCullMode = zenConst::keCullMode_None, zI32 _iDepthBias = 0, float _fDepthBiasClamp = 0.0f, float _fSlopeScaledDepthBias = 0.0f );
	};

}} // namespace zen, zenRes

#endif

