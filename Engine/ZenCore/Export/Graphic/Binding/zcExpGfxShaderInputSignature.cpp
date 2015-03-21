#include "zcCore.h"

namespace zcExp
{

zResID CreateGfxInputSignature( const zResID& _ParentShaderID )
{
	static zenMem::zAllocatorPool sMemPool("Pool CreateShaderInputSignature", sizeof(SerialInputSignature_Base::ExportInfo), 1, 5 );
	SerialInputSignature_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialInputSignature_Base::ExportInfo;	
	pExportInfo->mParentShaderID						= _ParentShaderID;
	return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxInputSignature, pExportInfo );
}

}