#include "zcCore.h"
#if ZEN_EXPORT_OR_RESOURCE_DX11

namespace zcExp
{

ExporterGfxTexture2dDX11_DX11::ExporterGfxTexture2dDX11_DX11(const ResDataRef& _rResData)
: ExporterBase(_rResData.GetSafe())
, mrResData(_rResData)
{
}

bool ExporterGfxTexture2dDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	ExportInfoGfxTexture2d* pExportInfo	= static_cast<ExportInfoGfxTexture2d*>(mpExportInfo);		
	mrResData->meFormat					= pExportInfo->meFormatOutput;
	mrResData->mCreationFlags			= pExportInfo->mCreationFlags;
	if( pExportInfo->mbGenerateMip )
	{
		//! @todo Missing: Mipmap creation
		ZENAssert(	(mrResData->maMipData.Count()==1) && 
					zenMath::IsPower2(mrResData->maMipData[0].mvDim.x) && 
					zenMath::IsPower2(mrResData->maMipData[0].mvDim.y) );
	}
		
	if( pExportInfo->meFormatInput != pExportInfo->meFormatOutput )
	{
		//! @todo Missing: Image conversion
		zUInt uMipCount = mrResData->maMipData.SetCount( pExportInfo->maMipData.Count() );
		for( zUInt mipIdx=0; mipIdx<uMipCount; ++mipIdx)
		{
			//..
		}					
	}
	else
	{
		zUInt uMipCount = mrResData->maMipData.SetCount( pExportInfo->maMipData.Count() );
		for(zUInt mipIdx(0); mipIdx<uMipCount; ++mipIdx)
		{
			mrResData->maMipData[mipIdx].maData		= pExportInfo->maMipData[mipIdx].maData;
			mrResData->maMipData[mipIdx].mvDim		= pExportInfo->maMipData[mipIdx].mvDim;
			mrResData->maMipData[mipIdx].muStride	= pExportInfo->maMipData[mipIdx].muStride;
		}
	}
		
	return TRUE;
}

}

#endif //ZEN_EXPORT_OR_RESOURCE_DX11
