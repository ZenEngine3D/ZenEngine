#include "zcCore.h"

namespace zcExp
{

zResID CreateGfxInputSignature( const zResID& _ParentShaderID )
{
	static zenMem::zAllocatorPool sMemPool("Pool CreateShaderInputSignature", sizeof(ExportInfoGfxInputSignature), 1, 5 );
	ExportInfoGfxInputSignature* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxInputSignature;	
	pExportInfo->mParentShaderID				= _ParentShaderID;
	return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxInputSignature, pExportInfo );
}

}