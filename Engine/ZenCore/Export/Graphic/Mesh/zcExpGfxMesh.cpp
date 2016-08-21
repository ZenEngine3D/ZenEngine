#include "zcCore.h"

namespace zcExp
{
	zResID ExportInfoGfxMesh::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		zenAssert(_eType==zenConst::keResType_GfxMesh);
		zenAssert( _pExportInfo );
		const ExportInfoGfxMesh* pExportInfo = static_cast<const ExportInfoGfxMesh*>(_pExportInfo);

		zResID::NameHash hName;
		for(zUInt meshStripIdx(0), meshStripCount(pExportInfo->maMeshStripID.Count()); meshStripIdx<meshStripCount; ++meshStripIdx)
			hName.Append( &(pExportInfo->maMeshStripID[meshStripIdx]), sizeof(zResID) );

		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	ExporterGfxMesh::ExporterGfxMesh(const ExportResultRef& _rExportOut)
	: Super(_rExportOut.GetSafe())
	, mrExport(_rExportOut)
	{
		zenAssert(mrExport.IsValid());
	}

	bool ExporterGfxMesh::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;
	
		ExportInfoGfxMesh* pExportInfo	= static_cast<ExportInfoGfxMesh*>(mpExportInfo);
		mrExport->maMeshStripID			= pExportInfo->maMeshStripID;

		// Make sure all MeshStrip are valid
		for(zUInt stripIdx(0), stripCount(mrExport->maMeshStripID.Count()); stripIdx<stripCount; ++stripIdx)
		{
			zResID resID = mrExport->maMeshStripID[stripIdx];
			if( resID.GetType() != zenConst::keResType_GfxMeshStrip || zcDepot::ExportData.GetTyped<ExportGfxMeshStrip>(resID).IsValid()==false )
				return false;
		}

		//! @todo Optimn: Parse all strip, concatenate all vertex together to quickly draw object in 1 call (for depth pass)
		ExportSkipWork();
		return true;
	}

	//=================================================================================================
	//! @brief		Create a new Mesh
	//! @details	Tie together all MeshStrips representing a entire Mesh
	//-------------------------------------------------------------------------------------------------
	//! @param _aMeshStripID	- List of MeshStrip this Mesh is made of
	//! @return 				- Created Mesh
	//=================================================================================================
	zResID CreateGfxMesh(const zArrayBase<zResID>& _aMeshStripID)
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateMesh", sizeof(ExportInfoGfxMesh), 1, 5 );
		ExportInfoGfxMesh* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxMesh;
		pExportInfo->maMeshStripID		= _aMeshStripID;	
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxMesh, pExportInfo );
	}

}

