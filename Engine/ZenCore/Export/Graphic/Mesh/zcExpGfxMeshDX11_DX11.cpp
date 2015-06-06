#include "zcCore.h"

namespace zcExp
{
	ExporterGfxMeshDX11_DX11::ExporterGfxMeshDX11_DX11(const ExportDataRef& _rExportData)
	: Super(_rExportData.GetSafe())
	, mrExportData(_rExportData)
	{
	}

	bool ExporterGfxMeshDX11_DX11::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;
	
		ExportInfoGfxMesh* pExportInfo	= static_cast<ExportInfoGfxMesh*>(mpExportInfo);
		mrExportData->maMeshStripID			= pExportInfo->maMeshStripID;

		// Make sure all MeshStrip are valid
		for(zUInt stripIdx(0), stripCount(mrExportData->maMeshStripID.Count()); stripIdx<stripCount; ++stripIdx)
		{
			zResID resID = mrExportData->maMeshStripID[stripIdx];
			if( resID.Type()!=zenConst::keResType_GfxMeshStrip || !EMgr::SerialItems.GetItem<ExportDataGfxMeshStripDX11>(resID) )
				return false;
		}

		//! @todo Optimn: Parse all strip, concatenate all vertex together to quickly draw object in 1 call (for depth pass)
		ExportSkipWork();
		return true;
	}

}
