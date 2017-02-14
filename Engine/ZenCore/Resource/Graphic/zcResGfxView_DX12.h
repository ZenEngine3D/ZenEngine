#pragma once
#ifndef __zCore_Res_Gfx_GfxView_DX12_h__
#define __zCore_Res_Gfx_GfxView_DX12_h__
//SF DX12
namespace zcRes
{
	class GfxView_DX12 : public zcExp::ExportGfxView
	{
	public:
		bool							Initialize();
		CD3DX12_VIEWPORT				mViewport;
		typedef zcExp::ExporterGfxView	RuntimeExporter;
	};
}

#endif