#include "libZenCore.h"

namespace EExp
{

awResourceID CreateGfxInputSignature( const awResourceID& _ParentShaderID )
{
	static CMem::PoolAllocator sMemPool("Pool CreateShaderInputSignature", sizeof(SerialInputSignature_Base::ExportInfo), 1, 5 );
	SerialInputSignature_Base::ExportInfo* pExportInfo	= AWNew(&sMemPool) SerialInputSignature_Base::ExportInfo;	
	pExportInfo->mParentShaderID						= _ParentShaderID;
	return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxInputSignature, pExportInfo );
}

}