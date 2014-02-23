#pragma once
#ifndef __LibExport_Graphics_Surface_Sampler_h__
#define __LibExport_Graphics_Surface_Sampler_h__

namespace EExp
{
	class SerialSampler_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialSampler_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			awconst::eTextureFiltering	meFilterMin;
			awconst::eTextureFiltering	meFilterMag;
			awconst::eTextureWrap		meWrapU;
			awconst::eTextureWrap		meWrapV;
			float						mfLodBias;
			zenVec4F						mvBorderColor;
		};
	};

	class SerialBlend_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialBlend_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{
			zenType::awBlendDesc	mxBlendDesc;
		};
	};

	class SerialDepthStencil_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialDepthStencil_Base, EExp::ExportItem)
	public:

		struct ExportInfo : public EExp::ExportInfoBase
		{
			zenType::awDepthStencilDesc	mxDepthStencilDesc;
		};
	};

	class SerialRasterizer_Base : public EExp::ExportItem
	{
		AWClassDeclare(SerialRasterizer_Base, EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			bool				mbFrontCounterClockwise;
			bool				mbDepthClipEnable;
			bool				mbScissorEnable;
			bool				mbMultisampleEnable;
			bool				mbAntialiasedLineEnable;
			bool				mbWireFrame;
			awconst::eCullMode	meCullMode;
			zenS32				miDepthBias;
			zenF32				mfDepthBiasClamp;
			zenF32				mfSlopeScaledDepthBias;
		};
	};

	zenResID CreateGfxSampler( awconst::eTextureFiltering _eFilterMin=awconst::keTexFilter_Bilinear, awconst::eTextureFiltering _eFilterMag=awconst::keTexFilter_Bilinear, awconst::eTextureWrap _eWrapU=awconst::keTexWrap_Repeat, awconst::eTextureWrap _eWrapV=awconst::keTexWrap_Repeat, float _fLodBias=0, const zenVec4F& _vBorderColor=zenVec4F(0,0,0,1) );
	zenResID CreateGfxBlend( zenType::awBlendDesc::awRTBlendDesc* _pxBlendDesc=nullptr, zenU8 _uRenderTargets = 0, bool _bAlphaToCoverageEnable = false, bool _bIndependentBlendEnable = false );
	zenResID CreateGfxDepthStencil( bool _bDepthEnable = false, bool _bDepthWrite = false, bool _bStencilEnable = false, zenU8 _uStencilReadMask = 0xFF, zenU8 _uStencilWriteMask = 0xFF, awconst::eComparisonFunc _eDepthFunc = awconst::keComparisonFunc_Always, zenType::awDepthStencilDesc::DepthStencilOp _xFrontFace = zenType::awDepthStencilDesc::DepthStencilOp(), zenType::awDepthStencilDesc::DepthStencilOp _xBackFace = zenType::awDepthStencilDesc::DepthStencilOp() );
	zenResID CreateGfxRasterizer( bool _bFrontCounterClockwise = false, bool _bDepthClipEnable = false, bool _bScissorEnable = false, bool _bMultisampleEnable = false, bool _bAntialiasedLineEnable = false, bool _bWireFrame = false, awconst::eCullMode _eCullMode = awconst::keCullMode_None, zenS32 _iDepthBias = 0, zenF32 _fDepthBiasClamp = 0.0f, zenF32 _fSlopeScaledDepthBias = 0.0f );
}

#include "EExpStateDX11.h"

#endif