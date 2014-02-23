#pragma once
#ifndef __LibResource_Graphics_GfxWindow_DX11_h__
#define __LibResource_Graphics_GfxWindow_DX11_h__

namespace ERes
{
	class GfxWindow_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialGfxWindow_DX11
		{
			IDXGISwapChain*			mDX11pSwapChain;			
			GfxRenderTargetRef		mrBackbufferColor;
			GfxRenderTargetRef		mrBackbufferDepth;
			GfxViewRef				mrBackbufferView;
			awconst::eTextureFormat	meBackbufferColorFormat;
			awconst::eTextureFormat	meBackbufferDepthFormat;
			awVec2U16				mvSize;			
		};
		AWResourceDeclare(GfxWindow_DX11, InstanceInfo, awconst::keResType_GfxWindow)
	
	//------------------------------------------------------------------
	// Common to all Window
	//------------------------------------------------------------------
	public:
		inline ERes::GfxViewRef		GetBackbuffer(){ return mInstanceInfo.mrBackbufferView; }
		void						Resize(const awVec2U16& _vSize);		

	//------------------------------------------------------------------
	// Platform Implementation
	//------------------------------------------------------------------
	public:
		virtual						~GfxWindow_DX11();
		virtual bool				ResourceInit();	
		void						PerformResize();
		awVec2U16					mvPendingResize;
	};
}

#endif