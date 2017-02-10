#pragma once

namespace zcRes
{
	class GfxTexture2D_DX12 : public zcExp::ExportGfxTexture2D
	{
	public:
		virtual											~GfxTexture2D_DX12();
		bool											Initialize();
		void*											Lock();	//!< @todo 2 Support mipmap
		void											Unlock(const zenGfx::zContext& _rContext);
		//! @todo 1 package these 3 members in 1 object with common methods for data upload
		DirectXComRef<ID3D12Resource>					mrResource;			//!< DirectX memory mapping for this resource
		DirectXComRef<ID3D12Resource>					mrResourceUpload;	//!< Upload memory allocated to stream-in a texture
		D3D12_RESOURCE_STATES							meResourceState;	//!< Current GPU access to this resource (used for barrier updates)	
		
		zcGfx::DescriptorSRV_UAV_CBV					mTextureView;		
		
		//! @todo 3 Add support for UAV and Stencil view
		typedef zcExp::ExporterGfxTexture2DDX12_DX12	RuntimeExporter;
	};
}
