#pragma once
#ifndef __zCore_Res_Gfx_GfxRenderTarget_DX12_h__
#define __zCore_Res_Gfx_GfxRenderTarget_DX12_h__
//SF DX12

namespace zcRes
{

	class GfxTarget2DHAL_DX12 : public zcExp::ExportGfxTarget2D
	{
	zenClassDeclare(GfxTarget2DHAL_DX12, zcExp::ExportGfxTarget2D)
	public:
		typedef zcExp::ExporterGfxTarget2D	RuntimeExporter;

		virtual								~GfxTarget2DHAL_DX12();
		virtual bool						Initialize();
		void								ReleaseBackbuffer();

		zenInline bool						IsDepth()const	{ return mTargetDepthView.IsValid() != 0; }
		void								Clear(const zVec4F& _vRGBA);
		void								Clear(float _fDepth=1, zU8 _uStencil=0, bool _bClearDepth=true, bool _bClearStencil=false);
		
		static GfxTarget2DRef				RuntimeCreate(const DirectXComRef<IDXGISwapChain>& _rSwapchain, zenConst::eTextureFormat _eTexFormat, zUInt _uBufferId);

	//protected: // @todo 1 make private
		bool								InitializeCommon(const DirectXComRef<ID3D12Resource>& _rSurface);
		DirectXComRef<ID3D12Resource>		mrTextureResource;
		zcGfx::DescriptorRTV				mTargetColorView;
		zcGfx::DescriptorDSV				mTargetDepthView;
	};
	class GfxTarget2DHAL : public GfxTarget2DHAL_DX12{};

}
#endif
