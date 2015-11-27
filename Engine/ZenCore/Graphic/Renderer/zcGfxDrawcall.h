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
class Drawcall : public zRefCounted
{
ZENClassDeclare(Drawcall, zRefCounted)
public:											
	static zEngineRef<Drawcall> Create(	const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip, const zVec4U16& _vScreenScissor =zVec4U16(0,0,0xFFFF,0xFFFF));
	virtual void Invoke(){};

//protected:			
	enum eGPUPipelineMode
	{
		keGpuPipe_PreDrawCommand,
		keGpuPipe_Compute, 
		keGpuPipe_Vertex, 
		keGpuPipe_VertexPixel, 
		keGpuPipe_VertexPixelGeo, 
		keGpuPipe_VertexPixelDomainHull, 
		keGpuPipe_PostDrawCommand,
	};

	union RenderStateSortID
	{
		// Put highest cost state change item last
		// Have a struct per Pipeline mode?
		struct
		{
			float	mfPriority;
			zU32	muShaderBindingID : 32;	//!< Use leftover bits to sort by shader binding
			zU32	VertexShaderID : 16;
			zU32	InputShaderID : 16;
			zU32	muRasterStateID : 12;	//! @todo Urgent finish sorting ID
			zU32	mbCullingFrontface : 1;	//!< Meshstrip wants frontface culling
			zU32	mbCullingBackface : 1;	//!< Meshstrip wants backface culling
			zU32	muGPUPipelineMode : 3;	//!< One of eGPUPipelineMode enum value			
			zU32	muRenderPassID : 10;	//!< Rendering pass ID
			zU32	UnusedBits : 5;	//!< Available for other uses
		};
		struct 
		{
			zU64	mSortKeyLo;
			zU64	mSortKeyHi;
		};
		
	};

	RenderStateSortID		mSortId;
	zcRes::GfxRenderPassRef	mrRenderPass;
	zcRes::GfxMeshStripRef	mrMeshStrip;
	zVec4U16				mvScreenScissor;		
protected:

	ZENInline void ConfigureBase( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, zU32 _uShaderBindingID, eGPUPipelineMode _eGPUPipelineMode ); 
	ZENInline void ConfigureBase( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip ); 
	Drawcall(){};
};

class DrawcallClearColor : public Drawcall
{
ZENClassDeclare(DrawcallClearColor, Drawcall)
public:
	static zEngineRef<Drawcall> Create( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxRenderTargetRef& _rRTColor, const zVec4F& _vRGBA,  const zColorMask& _ColorMask=zenConst::kColorMaskRGBA, const zVec2S16& _vOrigin=zVec2S16(0,0), const zVec2U16& _vDim=zVec2U16(0,0) );
	virtual void				Invoke();

protected:
	zcRes::GfxRenderTargetRef	mrRTColor;
	zVec4F						mvColor;
	zColorMask					mColorMask;
	zVec2S16					mvOrigin;
	zVec2U16					mvDim;	
};

class DrawcallClearDepthStencil : public Drawcall
{
ZENClassDeclare(DrawcallClearDepthStencil, Drawcall)
public:
	static zEngineRef<Drawcall> Create( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxRenderTargetRef& _rRTDepth, bool _bClearDepth, float _fDepthValue=1.f, bool _bClearStencil=false, zU8 _uStencilValue=128);
	virtual void				Invoke();

protected:
	zcRes::GfxRenderTargetRef	mrRTDepthStencil;
	bool						mbClearDepth;
	float						mfDepthValue;
	bool						mbClearStencil;
	zU8							muStencilValue;
};

}

#include "zcGfxDrawcall.inl"

#endif
