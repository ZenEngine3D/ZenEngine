#pragma once

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
