#pragma once

namespace zcRes
{
	class GfxTarget2D_DX12 : public zcExp::ExportGfxTarget2D
	{
	zenClassDeclare(GfxTarget2D_DX12, zcExp::ExportGfxTarget2D)
	public:
		typedef zcExp::ExporterGfxTarget2D	RuntimeExporter;

		virtual								~GfxTarget2D_DX12();
		virtual bool						Initialize();
		void								ReleaseBackbuffer();
		zenInline bool						IsDepth()const	{ return mTargetDepthView.IsValid() != 0; }

		static GfxTarget2DRef				RuntimeCreate(const DirectXComRef<IDXGISwapChain>& _rSwapchain, zenConst::eTextureFormat _eTexFormat, zUInt _uBufferId);

	//protected: // @todo 1 make private
		bool								InitializeCommon(const DirectXComRef<ID3D12Resource>& _rSurface);
		DirectXComRef<ID3D12Resource>		mrResource;			//!< DirectX memory mapping for this resource
		zcGfx::DescriptorRangeRTV			mTargetColorView;
		zcGfx::DescriptorRangeDSV			mTargetDepthView;
	};
}
