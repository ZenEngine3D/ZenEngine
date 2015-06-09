#include "zcCore.h"

namespace zcExp
{
	ExporterGfxMeshDX11_DX11::ExporterGfxMeshDX11_DX11(const ResDataRef& _rResData)
	: Super(_rResData.GetSafe())
	, mrResData(_rResData)
	{
	}

	bool ExporterGfxMeshDX11_DX11::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;
	
		ExportInfoGfxMesh* pExportInfo	= static_cast<ExportInfoGfxMesh*>(mpExportInfo);
		mrResData->maMeshStripID		= pExportInfo->maMeshStripID;

		// Make sure all MeshStrip are valid
		for(zUInt stripIdx(0), stripCount(mrResData->maMeshStripID.Count()); stripIdx<stripCount; ++stripIdx)
		{
			zResID resID = mrResData->maMeshStripID[stripIdx];
			if( resID.GetType()!=zenConst::keResType_GfxMeshStrip || zcDepot::ResourceData.GetItem<ResDataGfxMeshStripDX11>(resID).IsValid()==false )
				return false;
		}

		//! @todo Optimn: Parse all strip, concatenate all vertex together to quickly draw object in 1 call (for depth pass)
		ExportSkipWork();
		return true;
	}

}
