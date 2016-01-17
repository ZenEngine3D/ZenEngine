#pragma once
#ifndef __zCore_Res_Gfx_GfxRenderTarget_h__
#define __zCore_Res_Gfx_GfxRenderTarget_h__

namespace zcRes
{
	class GfxRenderTarget : public TResource<GfxRenderTarget, GfxRenderTargetResData, GfxRenderTargetProxy, GfxRenderTargetExporterRuntime>
	{
	ZENClassDeclare(GfxRenderTarget, TResource)
	public:	
		ZENInline bool					IsDepth()	{ return mrProxy->mpTargetDepthView != nullptr; }
		ZENInline const zVec2U16&		GetDim()	{ return mrResData->mvDim; }
		void							Clear(const zVec4F& _vRGBA){mrProxy->Clear(_vRGBA);} //! @todo clean thread unsafe access
		void							Clear(float _fDepth=1, zU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false){mrProxy->Clear(_fDepth, _uStencil, _bClearDepth, _bClearStencil);} //! @todo clean thread unsafe access
		const GfxTexture2dRef&			GetTexture2D()const{return mrTexture2D;}
	
	protected:										
		virtual bool					ResourceInit();		
		GfxTexture2dRef					mrTexture2D;		
	};
}

#endif