#pragma once
#ifndef __zenApi_Core_ResGfxState_h__
#define __zenApi_Core_ResGfxState_h__

namespace zen { namespace zenRes {	

	ZENClassResourceRefDeclare(zGfxSampler, zenConst::keResType_GfxSampler)
	public:
		static zGfxSampler				Create( zenConst::eTextureFiltering _eFilterMin=zenConst::keTexFilter_Bilinear, zenConst::eTextureFiltering _eFilterMag=zenConst::keTexFilter_Bilinear, zenConst::eTextureWrap _eWrapU=zenConst::keTexWrap_Repeat, zenConst::eTextureWrap _eWrapV=zenConst::keTexWrap_Repeat, float _fLodBias=0, const zVec4F& _vBorderColor=zVec4F(0,0,0,1) );
	};
	
	ZENClassResourceRefDeclare(zGfxStateBlend, zenConst::keResType_GfxBlend)
	};
	
	ZENClassResourceRefDeclare(zGfxStateDepthStencil, zenConst::keResType_GfxDepthStencil)
	};
	
	ZENClassResourceRefDeclare(zGfxStateRasterizer, zenConst::keResType_GfxRasterizer)
	public:
		struct Config
		{
			Config()
			: mbFrontClockwise(true)
			, mbShowBackface(false)
			, mbShowFrontface(true)
			, mbWireFrame(false)
			, mbDepthClipEnable(false)
			, mbScissorEnable(false)
			, mbMultisampleEnable(false)
			, mbAntialiasedLineEnable(false)			
			, mfDepthBias(0.0f)
			, mfDepthBiasClamp(0.0f)
			, mfSlopeScaledDepthBias(0.0f)
			{
			}

			zU16	mbFrontClockwise		: 1;
			zU16	mbShowBackface			: 1;
			zU16	mbShowFrontface			: 1;			
			zU16	mbWireFrame				: 1;
			zU16	mbDepthClipEnable		: 1;
			zU16	mbScissorEnable			: 1;
			zU16	mbMultisampleEnable		: 1;
			zU16	mbAntialiasedLineEnable	: 1;
			float	mfDepthBias;
			float	mfDepthBiasClamp;
			float	mfSlopeScaledDepthBias;
		};

		static zGfxStateRasterizer		Create( const Config& _RasterConfig );
	};

}} // namespace zen, zenRes

#endif

