#pragma once

namespace zcRes
{
//! @todo 2 clean rename all xxx_DX11 to xxx_HAL, skipping the typedef
	class GfxWindow_DX11 : public zcExp::ExportGfxWindowDX11
	{
	zenClassDeclare(GfxWindow_DX11, zcExp::ExportGfxWindowDX11)
	public:
		virtual							~GfxWindow_DX11();												
		bool							Initialize();
		bool							PerformResize();
		void							FrameBegin();
		void							FrameEnd();

		IDXGISwapChain*					mpDX11SwapChain				= nullptr;
		GfxTarget2DRef					mrBackbufferColor[2];
		zenConst::eTextureFormat		meBackbufferColorFormat		= zenConst::keTexFormat_RGBA8; //! @todo clean feature expose desired format in ResData
		zVec2U16						mvSize						= zVec2U16(0,0);
		zVec2U16						mvPendingResize				= zVec2U16(0,0);

	public:
		static GfxWindowRef				RuntimeCreate(HWND _WindowHandle);
	};
}
