#pragma once
#ifndef __LibResource_Graphics_GfxRenderTarget_DX11_h__
#define __LibResource_Graphics_GfxRenderTarget_DX11_h__

namespace ERes
{
	class GfxRenderTarget_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialRenderTarget_Base
		{
			ID3D11RenderTargetView* mpTargetColorView;
			ID3D11DepthStencilView*	mpTargetDepthView;			
			GfxTexture2DRef			mrTargetTexture;			
		};
	AWResourceDeclare(GfxRenderTarget_DX11, InstanceInfo, awconst::keResType_GfxRenderTarget)
	//---------------------------------------------------------
	// Common to all RenderTarget
	//---------------------------------------------------------
	public:
		inline bool					IsDepth()	{ return mInstanceInfo.mpTargetDepthView != NULL; }
		inline zenVec2U16			GetDim()	{ return Get().mvDim; }
		void						Clear(const zenVec4F& _vRGBA);
		void						Clear(float _fDepth=1, zenU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		GfxTexture2DRef				GetTexture2D();
	
	//---------------------------------------------------------
	// Platform implementation
	//---------------------------------------------------------
	public:
		virtual						~GfxRenderTarget_DX11();
		virtual bool				ResourceInit();
		void						ReleaseBackbuffer();
		static GfxRenderTargetRef	CreateFromBackuffer(IDXGISwapChain* _pSwapchain, awconst::eTextureFormat _eFormat, const zenVec2U16& _vDim);

	protected:
		ID3D11Texture2D*			mpSwapchainBackbuffer;	//!< Only set when create with CreateFromBackuffer 
		bool						mbNeedResolve;
	};
}

#endif