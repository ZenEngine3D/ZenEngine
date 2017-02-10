#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_DX12_h__
#define __zCore_Res_Gfx_GfxIndex_DX12_h__
//SF DX12
namespace zcRes
{

class GfxIndex_DX12 : public zcExp::ExportGfxIndexDX12
{
public:	
	virtual										~GfxIndex_DX12();
	bool										Initialize();
	zU8*										Lock();
	void										Unlock(const zenGfx::zContext& _rContext);

	DirectXComRef<ID3D12Resource>				mrResource;			//!< DirectX memory mapping for this resource
	DirectXComRef<ID3D12Resource>				mrResourceUpload;	//!< Temp directX memory used to update GPU data from CPU
	D3D12_RESOURCE_STATES						meResourceState;	//!< Current GPU access to this resource (used for barrier updates)	
	typedef zcExp::ExporterGfxIndexDX12_DX12	RuntimeExporter;
};

}

#endif
