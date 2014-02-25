#include "libZenCore.h"

namespace EExp
{

zenResID CreateGfxInputSignature( const zenResID& _ParentShaderID )
{
	static zbMem::PoolAllocator sMemPool("Pool CreateShaderInputSignature", sizeof(SerialInputSignature_Base::ExportInfo), 1, 5 );
	SerialInputSignature_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialInputSignature_Base::ExportInfo;	
	pExportInfo->mParentShaderID						= _ParentShaderID;
	return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, awconst::keResType_GfxInputSignature, pExportInfo );
}

}