#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_DX11_h__
#define __zCore_Res_Gfx_GfxWindow_DX11_h__

namespace zcRes
{
	class GfxWindowHAL_DX11 : public zcExp::ExportGfxWindowDX11
	{
	zenClassDeclare(GfxWindowHAL_DX11, zcExp::ExportGfxWindowDX11)
	public:
		virtual							~GfxWindowHAL_DX11();												
		bool							Initialize();
		bool							PerformResize();
		
		IDXGISwapChain*					mpDX11SwapChain				= nullptr;
		GfxTarget2DRef					mrBackbufferColor[2];
		zenConst::eTextureFormat		meBackbufferColorFormat		= zenConst::keTexFormat_RGBA8; //! @todo clean feature expose desired format in ResData
		zVec2U16						mvSize						= zVec2U16(0,0);
		zVec2U16						mvPendingResize				= zVec2U16(0,0);

	public:
		static GfxWindowRef				RuntimeCreate(HWND _WindowHandle);
	};
	class GfxWindowHAL : public GfxWindowHAL_DX11{};
}

#endif