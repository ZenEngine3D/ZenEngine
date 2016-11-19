#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_DX12_h__
#define __zCore_Res_Gfx_GfxWindow_DX12_h__

//SF DX12
namespace zcRes
{
	class GfxWindowHAL_DX12 : public zcExp::ExportGfxWindowDX12
	{
	zenClassDeclare(GfxWindowHAL_DX12, zcExp::ExportGfxWindowDX12)
	public:
		virtual									~GfxWindowHAL_DX12();												
		bool									Initialize();
		bool									PerformResize();
		void									FrameBegin();
		void									FrameEnd();

		DirectXComRef<IDXGISwapChain3>			mrDXSwapChain;
		GfxTarget2DRef							mrBackbufferColor[2];
		zenConst::eTextureFormat				meBackbufferColorFormat		= zenConst::keTexFormat__Invalid; //! @todo clean feature expose desired format in ResData
		zVec2U16								mvSize						= zVec2U16(0,0);
		zVec2U16								mvPendingResize				= zVec2U16(0,0);

	public:
		static GfxWindowRef						RuntimeCreate(HWND _WindowHandle);
	};
	
	class GfxWindowHAL : public GfxWindowHAL_DX12 { zenClassDeclare(GfxWindowHAL, GfxWindowHAL_DX12) };
}

#endif