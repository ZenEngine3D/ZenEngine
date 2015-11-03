#pragma once
#ifndef __zCore_Res_Gfx_GfxRenderTarget_DX11_h__
#define __zCore_Res_Gfx_GfxRenderTarget_DX11_h__

namespace zcRes
{
	class GfxRenderTargetProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxRenderTargetProxy_DX11, zRefCountedAutoDel)
	public:
										GfxRenderTargetProxy_DX11();
		virtual							~GfxRenderTargetProxy_DX11();												
		bool							Initialize(class GfxRenderTarget& _Owner);

		ZENInline bool					IsDepth()	{ return mpTargetDepthView != NULL; }
		void							Clear(const zVec4F& _vRGBA);
		void							Clear(float _fDepth=1, zU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		void							ReleaseBackbuffer();
	
	//protected:
		ID3D11RenderTargetView*			mpTargetColorView;
		ID3D11DepthStencilView*			mpTargetDepthView;
		zenConst::eTextureFormat		meFormat;
		zVec2U16						mvDim;
		bool							mbNeedResolve;
		GfxTexture2dProxyRef			mrProxParentTexture;
		ZENDbgCode(class GfxRenderTarget*	mpOwner);
	};
}

#endif