#include "libZenCore.h"

namespace EExp
{
	zenResID SerialMesh_Base::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_eType==awconst::keResType_GfxMesh);
		ZENAssert( _pExportInfo );
		const ExportInfo* pExportInfo = static_cast<const ExportInfo*>(_pExportInfo);

		zenResID::NameHash hName;
		for(zenUInt meshStripIdx(0), meshStripCount(pExportInfo->maMeshStripID.Count()); meshStripIdx<meshStripCount; ++meshStripIdx)
			hName.Append( &(pExportInfo->maMeshStripID[meshStripIdx]), sizeof(zenResID) );

		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//=================================================================================================
	//! @brief		Create a new Mesh
	//! @details	Tie together all MeshStrips representing a entire Mesh
	//-------------------------------------------------------------------------------------------------
	//! @param _aMeshStripID	- List of MeshStrip this Mesh is made of
	//! @return 				- Created Mesh
	//=================================================================================================
	zenResID CreateGfxMesh(const zenArrayBase<zenResID>& _aMeshStripID)
	{
		static zenMem::AllocatorPool sMemPool("Pool CreateMesh", sizeof(SerialMesh_Base::ExportInfo), 1, 5 );
		SerialMesh_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialMesh_Base::ExportInfo;
		pExportInfo->maMeshStripID					= _aMeshStripID;	
		return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, awconst::keResType_GfxMesh, pExportInfo );
	}

}

