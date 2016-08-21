#pragma once
#ifndef __zCore_Res_Gfx_GfxRenderTarget_DX11_h__
#define __zCore_Res_Gfx_GfxRenderTarget_DX11_h__

namespace zcRes
{

	class GfxTarget2DHAL_DX11 : public zcExp::ExportGfxTarget2D
	{
	ZENClassDeclare(GfxTarget2DHAL_DX11, zcExp::ExportGfxTarget2D)
	public:
		typedef zcExp::ExporterGfxTarget2D	RuntimeExporter;

		virtual								~GfxTarget2DHAL_DX11();
		virtual bool						Initialize();
		void								ReleaseBackbuffer();

		ZENInline bool						IsDepth()const	{ return mpTargetDepthView != nullptr; }
		void								Clear(const zVec4F& _vRGBA);
		void								Clear(float _fDepth=1, zU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		
		
	//protected:
		ID3D11RenderTargetView*				mpTargetColorView;
		ID3D11DepthStencilView*				mpTargetDepthView;
		static GfxTarget2DRef				RuntimeCreate(IDXGISwapChain& _Swapchain, zenConst::eTextureFormat _eTexFormat, zUInt _uBufferId);
	protected:
		bool								InitializeCommon(ID3D11Texture2D& _Texture);
	};
	class GfxTarget2DHAL : public GfxTarget2DHAL_DX11{};


}
#endif
