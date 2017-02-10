#pragma once
#ifndef __zCore_Res_Gfx_GfxRenderTarget_DX11_h__
#define __zCore_Res_Gfx_GfxRenderTarget_DX11_h__

namespace zcRes
{

	class GfxTarget2D_DX11 : public zcExp::ExportGfxTarget2D
	{
	zenClassDeclare(GfxTarget2D_DX11, zcExp::ExportGfxTarget2D)
	public:
		typedef zcExp::ExporterGfxTarget2D	RuntimeExporter;

		virtual								~GfxTarget2D_DX11();
		virtual bool						Initialize();
		void								ReleaseBackbuffer();

		zenInline bool						IsDepth()const	{ return mpTargetDepthView != nullptr; }	
		
	//protected:
		ID3D11RenderTargetView*				mpTargetColorView;
		ID3D11DepthStencilView*				mpTargetDepthView;
		static GfxTarget2DRef				RuntimeCreate(IDXGISwapChain& _Swapchain, zenConst::eTextureFormat _eTexFormat, zUInt _uBufferId);
	protected:
		bool								InitializeCommon(ID3D11Texture2D& _Texture);
	};

}
#endif
