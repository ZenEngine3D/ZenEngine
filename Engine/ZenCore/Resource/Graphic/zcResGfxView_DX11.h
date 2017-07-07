#pragma once

namespace zcRes
{
	class GfxView_DX11 : public zcExp::ExportGfxView
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
