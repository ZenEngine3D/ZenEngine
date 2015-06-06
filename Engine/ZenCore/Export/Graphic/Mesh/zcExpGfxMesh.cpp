#include "zcCore.h"

namespace zcExp
{
	zResID ExportInfoGfxMesh::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_eType==zenConst::keResType_GfxMesh);
		ZENAssert( _pExportInfo );
		const ExportInfoGfxMesh* pExportInfo = static_cast<const ExportInfoGfxMesh*>(_pExportInfo);

		zResID::NameHash hName;
		for(zUInt meshStripIdx(0), meshStripCount(pExportInfo->maMeshStripID.Count()); meshStripIdx<meshStripCount; ++meshStripIdx)
			hName.Append( &(pExportInfo->maMeshStripID[meshStripIdx]), sizeof(zResID) );

		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
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
		pExportInfo->maMeshStripID			= _aMeshStripID;	
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxMesh, pExportInfo );
	}

}

