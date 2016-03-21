#pragma once
#ifndef __zCore_Gfx_Renderer_Drawcall_h__
#define __zCore_Gfx_Renderer_Drawcall_h__

namespace zcGfx
{

//=================================================================================================
//! @brief		
//! @details	
//!				
//=================================================================================================	
//! @todo Urgent current work
class Command : public zRefCounted
{
ZENClassDeclare(Command, zRefCounted)
public:											
	virtual void			Invoke(){};	//!< Used by non drawing drawcall, else skip virtual call and render directly
	static void				ResetCommandCount();

//protected:			
	enum eGPUPipelineMode
	{
		keGpuPipe_PreDrawCommand,
		keGpuPipe_PreCompute, 
		keGpuPipe_Vertex, 
		keGpuPipe_VertexPixel, 
		keGpuPipe_VertexPixelGeo, 
		keGpuPipe_VertexPixelDomainHull, 
		keGpuPipe_PostCompute, 
		keGpuPipe_PostDrawCommand,
	};

	union RenderStateSortID
	{
		// Put highest cost state change item last
		// Have a struct per Pipeline mode?
		struct
		{
			float	mfPriority;
			zU32	muShaderBindingID	: 32;	
			zU32	muVertexShaderID	: 16;
			zU32	muInputShaderID		: 16;
			zU32	muRasterStateID		: 12;	//! @todo Urgent finish sorting ID
			zU32	mbCullingFrontface	: 1;	//!< Meshstrip wants frontface culling
			zU32	mbCullingBackface	: 1;	//!< Meshstrip wants backface culling
			zU32	muGPUPipelineMode	: 3;	//!< One of eGPUPipelineMode enum value			
			zU32	muRenderPassID		: 8;	//!< Rendering pass ID			
			zU32	Unused				: 7;	//!< Available bits
		};
		struct 
		{
			zU64	mSortKeyLo;
			zU64	mSortKeyHi;
		};		
	};

	bool					mbIsCommandDraw	= false;
	RenderStateSortID		mSortId;
	zcRes::GfxRenderPassRef	mrRenderPass;
	float					mfCommandIssuedIndex;	//! @todo Safe Not multithreaded safe
	static float			sfCommandCount;			//!< Number of command issued this frame. Used to set command priority when sorting. (made it a float to avoid float/int conversion cost for each drawcall)
protected:
							Command();
	ZENInline void			ConfigureBase( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, zU32 _uShaderBindingID, eGPUPipelineMode _eGPUPipelineMode ); 
	ZENInline void			ConfigureBase( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip ); 	
};

class CommandDraw : public Command
{
ZENClassDeclare(CommandDraw, Command)
public:
	static zEngineRef<Command>	Create(	const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0,0,0xFFFF,0xFFFF));

protected:
	zcRes::GfxMeshStripRef		mrMeshStrip;
	zVec4U16					mvScreenScissor;
	zU32						muIndexFirst	= 0;
	zU32						muIndexCount	= 0xFFFFFFFF;
	friend class ManagerRender;
};

class CommandClearColor : public Command
{
ZENClassDeclare(CommandClearColor, Command)
public:
	static zEngineRef<Command>	Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxRenderTargetRef& _rRTColor, const zVec4F& _vRGBA,  const zColorMask& _ColorMask=zenConst::kColorMaskRGBA, const zVec2S16& _vOrigin=zVec2S16(0,0), const zVec2U16& _vDim=zVec2U16(0,0) );
	virtual void				Invoke();

protected:
	zcRes::GfxRenderTargetRef	mrRTColor;
	zVec4F						mvColor;
	zColorMask					mColorMask;
	zVec2S16					mvOrigin;
	zVec2U16					mvDim;	
};

class CommandClearDepthStencil : public Command
{
ZENClassDeclare(CommandClearDepthStencil, Command)
public:
	static zEngineRef<Command> Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxRenderTargetRef& _rRTDepth, bool _bClearDepth, float _fDepthValue=1.f, bool _bClearStencil=false, zU8 _uStencilValue=128);
	virtual void				Invoke();

protected:
	zcRes::GfxRenderTargetRef	mrRTDepthStencil;
	bool						mbClearDepth;
	float						mfDepthValue;
	bool						mbClearStencil;
	zU8							muStencilValue;
};

class CommandEventStart : public Command
{
ZENClassDeclare(CommandEventStart, Command)
public:
	static zEngineRef<Command> Create(const zcRes::GfxRenderPassRef& _rRenderPass/*, const zenPerf::zEventRef& _rEventGPU*/ );
	virtual void				Invoke();

protected:	
	//zenPerf::zEventRef			mrEventGPU;
};

class CommandEventStop : public Command
{
	ZENClassDeclare(CommandEventStop, Command)
public:
	static zEngineRef<Command> Create(const zcRes::GfxRenderPassRef& _rRenderPass/*, const zenPerf::zEventRef& _rEventGPU*/ );
	virtual void				Invoke();

protected:
	//zenPerf::zEventRef			mrEventGPU;
};

}

#include "zcGfxDrawcall.inl"

#endif
