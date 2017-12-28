#pragma once

namespace zen { namespace zenRes {	

	class zGfxRenderPass : public zcRes::GfxRenderPassRef
	{
	zenClassDeclare(zGfxRenderPass, zcRes::GfxRenderPassRef);
	public:
		const zGfxTarget2D&		GetColorTarget(zUInt _uIndex=0)const;
		const zGfxTarget2D&		GetDepthTarget()const;

		using Super::Super;
		using Super::operator=;

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

			zGfxTarget2D	mrTargetSurface;	//! @todo clean make this a resid to be serializable? 
			zColorMask		mWriteMask			= zenConst::kColorMaskRGBA;				
			bool			mbBlendEnable		= false;
			eBlendValue		meBlendColorSrc		= keBlendVal_SrcAlpha;
			eBlendValue		meBlendColorDest	= keBlendVal_InvSrcAlpha;
			eBlendOp		meBlendColorOp		= keBlendOp_Add;
			eBlendValue		meBlendAlphaSrc		= keBlendVal_One;
			eBlendValue		meBlendAlphaDest	= keBlendVal_Zero;
			eBlendOp		meBlendAlphaOp		= keBlendOp_Add;			
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
			zGfxTarget2D		mrTargetSurface;
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
		static zGfxRenderPass	Create(const zString& _zStageName, zU8 _uPassPriority, const ConfigColorRT& _RTColor, const ConfigDepthRT& _RTDepth, const zGfxStateRaster& _rStateRaster, const zVec2U16& _vDim = zVec2U16(0xFFFF, 0xFFFF), const zVec2S16& _vOrigin = zVec2S16(0,0));
		static zGfxRenderPass	Create(const zString& _zStageName, zU8 _uPassPriority, const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aRTColors, const ConfigDepthRT& _RTDepth, const zGfxStateRaster& _rStateRaster, const zVec2U16& _vDim = zVec2U16(0xFFFF, 0xFFFF), const zVec2S16& _vOrigin = zVec2S16(0,0));
	};	
}} // namespace zen, zenRes

//! @todo Urgent, this is not a resource, should be moved elsewhere
namespace zcGfx  { class CommandList; }	//Forward declare
namespace zen { namespace zenGfx 
{	
	class zCommandList : public zEngineRef<zcGfx::CommandList>
	{
	zenClassDeclare(zCommandList, zEngineRef<zcGfx::CommandList>);
	public:
		void						Submit();
		void						SetBeforeChildren(bool _bBeforeChild=true); //!< @brief Tell commandlist where to insert new commands from now on
		bool						GetBeforeChildren()const;
		static zCommandList			Create(const zStringHash32& _zContextName, const zCommandList& _rParent=zCommandList(), const zenRes::zGfxRenderPass& _rRenderState=zenRes::zGfxRenderPass());
		static zCommandList			Create(const zStringHash32& _zContextName, const zenRes::zGfxRenderPass& _rRenderState);
		static const zCommandList&	GetFrameContext();
		using zEngineRef<zcGfx::CommandList>::operator=;
		using zEngineRef<zcGfx::CommandList>::zEngineRef;
	};
}}

#include "zenCoreResGfxBinding.inl"
