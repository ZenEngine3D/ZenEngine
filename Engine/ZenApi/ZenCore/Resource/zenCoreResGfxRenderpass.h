#pragma once
#ifndef __zenApi_Core_ResGfxRenderpass_h__
#define __zenApi_Core_ResGfxRenderpass_h__

namespace zen { namespace zenRes {	

	ZENClassResourceRefDeclare(zGfxRenderPass, zenConst::keResType_GfxRenderPass)
	public:
		struct ConfigColorRT
		{
		// If editing these enum, make sure platform specific implementation are also updated to reflect this
		enum eBlendValue {	keBlendVal_Zero, 
							keBlendVal_One, 
							keBlendVal_SrcColor, 
							keBlendVal_InvSrcColor, 
							keBlendVal_SrcAlpha, 
							keBlendVal_InvSrcAlpha, 
							keBlendVal_DestAlpha, 
							keBlendVal_InvDestAlpha, 
							keBlendVal_DestColor, 
							keBlendVal_InvDestColor, 
							keBlendVal_SrcAlphaSat, 
							keBlendVal_BlendFactor, 
							keBlendVal_InvBlendFactor, 
							keBlendVal__Count, 
							keBlendVal__Invalid };
		enum eBlendOp {		keBlendOp_Add, 
							keBlendOp_Substract, 
							keBlendOp_RevSubstract, 
							keBlendOp_Min, 
							keBlendOp_Max, 
							keBlendOp__Count, 
							keBlendOp__Invalid };

			zGfxRenderTarget	mrTargetSurface		= nullptr;
			zColorMask			mWriteMask			= zenConst::kColorMaskRGBA;				
			bool				mbBlendEnable		= false;
			eBlendValue			meBlendColorSrc		= keBlendVal_SrcAlpha;
			eBlendValue			meBlendColorDest	= keBlendVal_InvSrcAlpha;
			eBlendOp			meBlendColorOp		= keBlendOp_Add;
			eBlendValue			meBlendAlphaSrc		= keBlendVal_One;
			eBlendValue			meBlendAlphaDest	= keBlendVal_Zero;
			eBlendOp			meBlendAlphaOp		= keBlendOp_Add;			
		};

		struct ConfigDepthRT
		{
			enum eCompareTest
			{
				keCmpTest_Never,
				keCmpTest_Less,
				keCmpTest_Equal,
				keCmpTest_LessEqual,
				keCmpTest_Greater,
				keCmpTest_NotEqual,
				keCmpTest_GreaterEqual,
				keCmpTest_Always,
				keCmpTest__Count
			};
			enum eStencilOp
			{
				keStencilOp_Keep,
				keStencilOp_Zero,
				keStencilOp_Replace,
				keStencilOp_IncrSat,
				keStencilOp_DecrSat,
				keStencilOp_Invert,
				keStencilOp_Incr,
				keStencilOp_Decr,
				keStencilOp__Count
			};
			struct ConfigStencil
			{
				eCompareTest	meStencilTest	= keCmpTest_Always;
				eStencilOp		meStencilPassOp	= keStencilOp_Keep;
				eStencilOp		meStencilFailOp	= keStencilOp_Keep;
				eStencilOp		meDepthFailOp	= keStencilOp_Keep;				
			};

		public:
			zGfxRenderTarget	mrTargetSurface;
			bool				mbDepthEnable				= false;
			bool				mbDepthWrite				= false;
			bool				mbStencilEnable				= false;
			eCompareTest		meDepthTest					= keCmpTest_Always;			
			zU8					muStencilReadMask			= 0xFF;
			zU8					muStencilWriteMask			= 0xFF;
			ConfigStencil		meStencilConfigFront;
			ConfigStencil		meStencilConfigBack;
		};
		
		//! @todo Clean Use Raster Config instead of reference?
		static zGfxRenderPass	Create(const zString& _zStageName, zU8 _uPassPriority, const ConfigColorRT& _RTColor, const ConfigDepthRT& _RTDepth, const zGfxStateRasterizer& _rStateRaster, const zVec2U16& _vDim = zVec2U16(0xFFFF, 0xFFFF), const zVec2S16& _vOrigin = zVec2S16(0,0));
		static zGfxRenderPass	Create(const zString& _zStageName, zU8 _uPassPriority, const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aRTColors, const ConfigDepthRT& _RTDepth, const zGfxStateRasterizer& _rStateRaster, const zVec2U16& _vDim = zVec2U16(0xFFFF, 0xFFFF), const zVec2S16& _vOrigin = zVec2S16(0,0));
	};	
}} // namespace zen, zenRes


//! @todo Urgent, this is not a resource, should be moved elsewhere
namespace zcGfx  { class Drawcall; }	//Forward declare
namespace zen { namespace zenRes 
{	
	class zGfxDrawcall : public zEngineRef<zcGfx::Drawcall>
	{
	ZENClassDeclare(zGfxDrawcall, zEngineRef<zcGfx::Drawcall>);
	public:			
										zGfxDrawcall();
										zGfxDrawcall(const zGfxDrawcall& _Copy);		
										zGfxDrawcall(zcGfx::Drawcall* _Drawcall);
		
		zGfxDrawcall&					operator=(zcGfx::Drawcall* _pCopy);
		zGfxDrawcall&					operator=(const zGfxDrawcall& _Copy);
		bool							zGfxDrawcall::operator>(const zenRes::zGfxDrawcall& _Cmp)const;
		bool							zGfxDrawcall::operator>=(const zenRes::zGfxDrawcall& _Cmp)const;
		static zGfxDrawcall				DrawMesh			(const zGfxRenderPass& _rRenderPass, float _fPriority, const zGfxMeshStrip&	_rMeshStrip);
		static zGfxDrawcall				ClearColor			(const zGfxRenderPass& _rRenderPass, const float& _fPriority, const zGfxRenderTarget& _RTColor, const zVec4F& _vRGBA,  const zColorMask& _ColorMask=zenConst::kColorMaskRGBA, const zVec2S16& _vOrigin=zVec2S16(0,0), const zVec2U16& _vDim=zVec2U16(0,0) );
		static zGfxDrawcall				ClearDepthStencil	(const zGfxRenderPass& _rRenderPass, float _fPriority, const zGfxRenderTarget& _rRTDepthStencil, bool _bClearDepth=true, float _fDepthValue=1, bool _bClearStencil=false, zU8 _uStencilValue=128);
		static void						Submit				(zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcalls);
	};
	
}}
#include "zenCoreResGfxBinding.inl"

#endif

