#pragma once

//! @todo 5 Clean : rename these files GfxDrawCommand

namespace zcGfx
{

//=================================================================================================
//! @brief		
//! @details	
//!				
//=================================================================================================	
class Command : public zRefCounted
{
zenClassDeclare(Command, zRefCounted)
public:											
	virtual void			Invoke(class GPUContext& _Context){};	//!< Used by non drawing drawcall, else skip virtual call and render directly
	static void				ResetCommandCount();

//protected:			
	union RenderStateSortID
	{
		// Put highest cost state change item last
		struct 
		{
			float	mfPriority;			
			zU32	muShaderBindingID	: 32;	
			zU32	muVertexShaderID	: 16;
			zU32	muInputShaderID		: 16;
			zU32	muRasterStateID		: 12;	//! @todo Urgent finish sorting ID
			zU32	mbCullingFrontface	: 1;	//!< Meshstrip wants frontface culling
			zU32	mbCullingBackface	: 1;	//!< Meshstrip wants backface culling
			zU32	muUnused			: 15;

			zU32	muGPUPipelineMode	: 3;	//!< One of eGPUPipelineMode enum value
		}Draw;
		struct 
		{
			float	mfPriority;						
			zU32	muShaderBindingID	: 32;
			zU32	muUnused1			: 32;
			zU32	muUnused2			: 29;
			
			zU32	muGPUPipelineMode	: 3;	//!< One of eGPUPipelineMode enum value
		}Compute;
		struct 
		{
			zU64	muResID;
			zU32	muUnused1			: 32;
			zU32	muUnused2			: 29;
			
			zU32	muGPUPipelineMode	: 3;	//!< One of eGPUPipelineMode enum value
		}DataUpdate;
		struct 
		{
			zU64	mSortKeyLo;
			zU64	mSortKeyHi;
		};
		bool operator>(const RenderStateSortID& _Cmp)const { return mSortKeyHi > _Cmp.mSortKeyHi || (mSortKeyHi == _Cmp.mSortKeyHi && mSortKeyLo > _Cmp.mSortKeyLo); }
	};

	RenderStateSortID		mSortId;
	zcRes::GfxRenderPassRef	mrRenderPass;
	static float			sfCommandCount;			//!< Number of command issued this frame. Used to set command priority when sorting. (made it a float to avoid float/int conversion cost for each drawcall)	
protected:
							Command();
	zenInline void			SetSortKeyDraw		( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip ); 	
	zenInline void			SetSortKeyCompute	( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, zU32 _uShaderBindingID, bool _bBeforeDraw=true);
	zenInline void			SetSortKeyDataUpdate( zU64 _uResID ); 	
};

zenInline bool operator>(const zEngineRef<zcGfx::Command>& _rCmp1, const zEngineRef<zcGfx::Command>& _rCmp2)
{	 
	return _rCmp1.GetConst() && _rCmp2.GetConst() && _rCmp1.GetConst()->mSortId > _rCmp2.GetConst()->mSortId;
}

class CommandDraw : public Command
{
zenClassDeclare(CommandDraw, Command)
public:
	static zEngineRef<Command>	Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0,0,0xFFFF,0xFFFF));

// protected: //! @todo 1 clean remove public access (needed for gfxmgr::updatestate
	zcRes::GfxMeshStripRef		mrMeshStrip;
	zVec4U16					mvScreenScissor;
	zU32						muIndexFirst	= 0;
	zU32						muIndexCount	= 0xFFFFFFFF;
};

class CommandClearColor : public Command
{
zenClassDeclare(CommandClearColor, Command)
public:
	static zEngineRef<Command>	Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTColor, const zVec4F& _vRGBA,  const zColorMask& _ColorMask=zenConst::kColorMaskRGBA, const zVec2S16& _vOrigin=zVec2S16(0,0), const zVec2U16& _vDim=zVec2U16(0,0) );

protected:
	zcRes::GfxTarget2DRef		mrRTColor;
	zVec4F						mvColor;
	zColorMask					mColorMask;
	zVec2S16					mvOrigin;
	zVec2U16					mvDim;	
};

class CommandClearDepthStencil : public Command
{
zenClassDeclare(CommandClearDepthStencil, Command)
public:
	static zEngineRef<Command>	Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTDepth, bool _bClearDepth, float _fDepthValue=1.f, bool _bClearStencil=false, zU8 _uStencilValue=128);

protected:
	zcRes::GfxTarget2DRef		mrRTDepthStencil;
	bool						mbClearDepth;
	float						mfDepthValue;
	bool						mbClearStencil;
	zU8							muStencilValue;
};

}

#include zenHeaderRenderer(zcGfxDrawcall)
#include "zcGfxDrawcall.inl"
