#pragma once
#ifndef __zCore_ExpGfxState_h__
#define __zCore_ExpGfxState_h__

namespace zcExp
{
	struct ExportInfoGfxSampler : public ExportInfoBase
	{			
		zenConst::eTextureFiltering	meFilterMin;
		zenConst::eTextureFiltering	meFilterMag;
		zenConst::eTextureWrap		meWrapU;
		zenConst::eTextureWrap		meWrapV;
		float						mfLodBias;
		zVec4F						mvBorderColor;
	};

	//! @todo Urgent remove items below from export logic, just created dynamically with template?
	struct ExportInfoGfxStateBlend : public ExportInfoBase
	{
		zenType::zBlendDesc			mBlendDesc; //! @todo cleanup move zBlendDesc descriptor elsewhere
	};

	struct ExportInfoGfxStateDepthStencil : public ExportInfoBase
	{
		zenType::zDepthStencilDesc	mDepthStencilDesc; //! @todo cleanup move zDepthStencilDesc descriptor elsewhere
	};

	struct ExportInfoGfxStateRasterizer : public ExportInfoBase
	{
		bool				mbFrontCounterClockwise;
		bool				mbDepthClipEnable;
		bool				mbScissorEnable;
		bool				mbMultisampleEnable;
		bool				mbAntialiasedLineEnable;
		bool				mbWireFrame;
		zenConst::eCullMode	meCullMode;
		zI32				miDepthBias;
		float				mfDepthBiasClamp;
		float				mfSlopeScaledDepthBias;
	};
	
	zResID CreateGfxSampler( zenConst::eTextureFiltering _eFilterMin=zenConst::keTexFilter_Bilinear, zenConst::eTextureFiltering _eFilterMag=zenConst::keTexFilter_Bilinear, zenConst::eTextureWrap _eWrapU=zenConst::keTexWrap_Repeat, zenConst::eTextureWrap _eWrapV=zenConst::keTexWrap_Repeat, float _fLodBias=0, const zVec4F& _vBorderColor=zVec4F(0,0,0,1) );
	zResID CreateGfxBlend( zenType::zBlendDesc* _pBlendDesc=nullptr );
	zResID CreateGfxDepthStencil( bool _bDepthEnable = false, bool _bDepthWrite = false, bool _bStencilEnable = false, zU8 _uStencilReadMask = 0xFF, zU8 _uStencilWriteMask = 0xFF, zenConst::eComparisonFunc _eDepthFunc = zenConst::keComparisonFunc_Always, zenType::zDepthStencilDesc::DepthStencilOp _xFrontFace = zenType::zDepthStencilDesc::DepthStencilOp(), zenType::zDepthStencilDesc::DepthStencilOp _xBackFace = zenType::zDepthStencilDesc::DepthStencilOp() );
	zResID CreateGfxRasterizer( bool _bFrontCounterClockwise = false, bool _bDepthClipEnable = false, bool _bScissorEnable = false, bool _bMultisampleEnable = false, bool _bAntialiasedLineEnable = false, bool _bWireFrame = false, zenConst::eCullMode _eCullMode = zenConst::keCullMode_None, zI32 _iDepthBias = 0, float _fDepthBiasClamp = 0.0f, float _fSlopeScaledDepthBias = 0.0f );	
}

#include "zcExpGfxStateDX11.h"

#endif