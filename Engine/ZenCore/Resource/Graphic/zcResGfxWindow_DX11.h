#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_DX11_h__
#define __zCore_Res_Gfx_GfxWindow_DX11_h__

namespace zcRes
{
	class GfxWindow_DX11 : public Resource
	{
		struct InstanceInfo : public zcExp::SerialGfxWindow_DX11
		{
			IDXGISwapChain*			mDX11pSwapChain;			
			GfxRenderTargetRef		mrBackbufferColor;
			GfxRenderTargetRef		mrBackbufferDepth;
			GfxViewRef				mrBackbufferView;
			zenConst::eTextureFormat	meBackbufferColorFormat;
			zenConst::eTextureFormat	meBackbufferDepthFormat;
			zVec2U16				mvSize;			
		};
		ZENResourceDeclare(GfxWindow_DX11, InstanceInfo, zenConst::keResType_GfxWindow)
	
	//------------------------------------------------------------------
	// Common to all Window
	//------------------------------------------------------------------
	public:
		ZENInline zcRes::GfxViewRef		GetBackbuffer(){ return mInstanceInfo.mrBackbufferView; }
		void						Resize(const zVec2U16& _vSize);		

	//------------------------------------------------------------------
	// Platform Implementation
	//------------------------------------------------------------------
	public:
		virtual						~GfxWindow_DX11();
		virtual bool				ResourceInit();	
		void						PerformResize();
		zVec2U16					mvPendingResize;
	};
}

#endif