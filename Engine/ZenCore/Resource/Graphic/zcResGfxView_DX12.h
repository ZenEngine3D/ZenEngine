#pragma once
#ifndef __zCore_Res_Gfx_GfxView_DX12_h__
#define __zCore_Res_Gfx_GfxView_DX12_h__
//SF DX12
namespace zcRes
{
	class GfxView_DX12 : public zcExp::ExportGfxView
	{
	public:
		bool										Initialize();
		ID3D11RenderTargetView*						mpColorViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		ID3D11DepthStencilView*						mpDepthView;
		D3D11_VIEWPORT								mViewport;
		UINT										muColorCount;	

		typedef zcExp::ExporterGfxView				RuntimeExporter;
	};
}

#endif