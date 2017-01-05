#pragma once
#ifndef __zCore_Res_Gfx_Texture_DX12_h__
#define __zCore_Res_Gfx_Texture_DX12_h__
//SF DX12
namespace zcRes
{
	class GfxTexture2dHAL_DX12 : public zcExp::ExportGfxTexture2d
	{
	public:
		virtual											~GfxTexture2dHAL_DX12();
		bool											Initialize();
	
		DirectXComRef<ID3D12Resource>					mrTextureResource;
		zcGfx::DescriptorSRV_UAV_CBV					mTextureView;
		//! @todo 3 Add support for UAV and Stencil view
		typedef zcExp::ExporterGfxTexture2dDX12_DX12	RuntimeExporter;
	};
	class GfxTexture2dHAL : public GfxTexture2dHAL_DX12{};

}

#endif
