#pragma once
#ifndef __LibExport_Graphics_Surface_Sampler_h__
#define __LibExport_Graphics_Surface_Sampler_h__

namespace EExp
{
	class SerialSampler_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialSampler_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			zenConst::eTextureFiltering	meFilterMin;
			zenConst::eTextureFiltering	meFilterMag;
			zenConst::eTextureWrap		meWrapU;
			zenConst::eTextureWrap		meWrapV;
			float						mfLodBias;
			zenVec4F						mvBorderColor;
		};
	};

	class SerialBlend_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialBlend_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{
			zenType::awBlendDesc	mxBlendDesc;
		};
	};

	class SerialDepthStencil_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialDepthStencil_Base, EExp::ExportItem)
	public:

		struct ExportInfo : public EExp::ExportInfoBase
		{
			zenType::awDepthStencilDesc	mxDepthStencilDesc;
		};
	};

	class SerialRasterizer_Base : public EExp::ExportItem
	{
		ZENClassDeclare(SerialRasterizer_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			bool				mbFrontCounterClockwise;
			bool				mbDepthClipEnable;
			bool				mbScissorEnable;
			bool				mbMultisampleEnable;
			bool				mbAntialiasedLineEnable;
			bool				mbWireFrame;
			zenConst::eCullMode	meCullMode;
			zenS32				miDepthBias;
			zenF32				mfDepthBiasClamp;
			zenF32				mfSlopeScaledDepthBias;
		};
	};

	zenResID CreateGfxSampler( zenConst::eTextureFiltering _eFilterMin=zenConst::keTexFilter_Bilinear, zenConst::eTextureFiltering _eFilterMag=zenConst::keTexFilter_Bilinear, zenConst::eTextureWrap _eWrapU=zenConst::keTexWrap_Repeat, zenConst::eTextureWrap _eWrapV=zenConst::keTexWrap_Repeat, float _fLodBias=0, const zenVec4F& _vBorderColor=zenVec4F(0,0,0,1) );
	zenResID CreateGfxBlend( zenType::awBlendDesc::awRTBlendDesc* _pxBlendDesc=nullptr, zenU8 _uRenderTargets = 0, bool _bAlphaToCoverageEnable = false, bool _bIndependentBlendEnable = false );
	zenResID CreateGfxDepthStencil( bool _bDepthEnable = false, bool _bDepthWrite = false, bool _bStencilEnable = false, zenU8 _uStencilReadMask = 0xFF, zenU8 _uStencilWriteMask = 0xFF, zenConst::eComparisonFunc _eDepthFunc = zenConst::keComparisonFunc_Always, zenType::awDepthStencilDesc::DepthStencilOp _xFrontFace = zenType::awDepthStencilDesc::DepthStencilOp(), zenType::awDepthStencilDesc::DepthStencilOp _xBackFace = zenType::awDepthStencilDesc::DepthStencilOp() );
	zenResID CreateGfxRasterizer( bool _bFrontCounterClockwise = false, bool _bDepthClipEnable = false, bool _bScissorEnable = false, bool _bMultisampleEnable = false, bool _bAntialiasedLineEnable = false, bool _bWireFrame = false, zenConst::eCullMode _eCullMode = zenConst::keCullMode_None, zenS32 _iDepthBias = 0, zenF32 _fDepthBiasClamp = 0.0f, zenF32 _fSlopeScaledDepthBias = 0.0f );
}

#include "EExpStateDX11.h"

#endif