#pragma once
#ifndef __zenApi_Core_ResGfxState_h__
#define __zenApi_Core_ResGfxState_h__

namespace zen { namespace zenRes {	

	class zGfxSampler : public zcRes::GfxSamplerRef
	{
	zenClassDeclare(zGfxSampler, zcRes::GfxSamplerRef);
	public:
		using Super::Super;
		using Super::operator=;
		static zGfxSampler				Create( zenConst::eTextureFiltering _eFilterMin=zenConst::keTexFilter_Bilinear, zenConst::eTextureFiltering _eFilterMag=zenConst::keTexFilter_Bilinear, zenConst::eTextureWrap _eWrapU=zenConst::keTexWrap_Repeat, zenConst::eTextureWrap _eWrapV=zenConst::keTexWrap_Repeat, float _fLodBias=0, const zVec4F& _vBorderColor=zVec4F(0,0,0,1) );
	};

	class zGfxStateBlend : public zcRes::GfxStateBlendRef
	{
	zenClassDeclare(zGfxStateBlend, zcRes::GfxStateBlendRef);
	public:
		using Super::Super;
		using Super::operator=;
	};
	
	class zGfxStateDepthStencil : public zcRes::GfxStateDepthStencilRef
	{
	zenClassDeclare(zGfxStateDepthStencil, zcRes::GfxStateDepthStencilRef);
	public:
		using Super::Super;
		using Super::operator=;
	};

	class zGfxStateRaster : public zcRes::GfxStateRasterRef
	{
	zenClassDeclare(zGfxStateRaster, zcRes::GfxStateRasterRef);
	public:
		using Super::Super;
		using Super::operator=;
	
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

		static zGfxStateRaster		Create( const Config& _RasterConfig );
	};

}} // namespace zen, zenRes

#endif

