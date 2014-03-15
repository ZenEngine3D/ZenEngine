#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
	bool SerialGfxMesh_DX11::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;
		
		ExportInfo* pExportInfo	= static_cast<ExportInfo*>(mpExportInfo);
		mSerial.maMeshStripID = pExportInfo->maMeshStripID;

		// Make sure all MeshStrip are valid
		for(zUInt stripIdx(0), stripCount(mSerial.maMeshStripID.Count()); stripIdx<stripCount; ++stripIdx)
		{
			zResID resID = mSerial.maMeshStripID[stripIdx];
			if( resID.Type()!=zenConst::keResType_GfxMeshStrip || !EMgr::SerialItems.GetItem<EExp::SerialGfxMeshStrip_DX11>(resID) )
				return false;
		}

		//! @todo Optimn: Parse all strip, concatenate all vertex together to quickly draw object in 1 call (for depth pass)
		ExportSkipWork();
		return true;
	}

	bool SerialGfxMesh_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		if( Super::Serialize(_Serializer) )
			return false;

		return false;
	}
}

#endif //AW_EXPORT_OR_RESOURCE_DX11
