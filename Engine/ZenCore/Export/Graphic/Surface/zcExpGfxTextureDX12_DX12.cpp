#include "zcCore.h"
#if ZEN_EXPORT_OR_RESOURCE_DX12

//SF DX12

namespace zcExp
{

ExporterGfxTexture2dDX12_DX12::ExporterGfxTexture2dDX12_DX12(const ExportResultRef& _rExportOut)
: ExporterBase(_rExportOut.GetSafe())
, mrExport(_rExportOut)
{
	zenAssert(mrExport.IsValid());
}

bool ExporterGfxTexture2dDX12_DX12::ExportWork(bool _bIsTHRTask)
{
	ExportInfoGfxTexture2d* pExportInfo	= static_cast<ExportInfoGfxTexture2d*>(mpExportInfo);		
	mrExport->meFormat					= pExportInfo->meFormatOutput;
	mrExport->mCreationFlags			= pExportInfo->mCreationFlags;
	if( pExportInfo->mbGenerateMip )
	{
		//! @todo Missing: Mipmap creation
		zenAssert(	(mrExport->maMipData.Count()==1) && 
					zenMath::IsPower2(mrExport->maMipData[0].mvDim.x) && 
					zenMath::IsPower2(mrExport->maMipData[0].mvDim.y) );
	}
		
	if( pExportInfo->meFormatInput != pExportInfo->meFormatOutput )
	{
		//! @todo Missing: Image conversion
		zUInt uMipCount = mrExport->maMipData.SetCount( pExportInfo->maMipData.Count() );
		for( zUInt mipIdx=0; mipIdx<uMipCount; ++mipIdx)
		{
			//..
		}					
	}
	else
	{
		zUInt uMipCount = mrExport->maMipData.SetCount( pExportInfo->maMipData.Count() );
		for(zUInt mipIdx(0); mipIdx<uMipCount; ++mipIdx)
		{
			mrExport->maMipData[mipIdx].maData		= pExportInfo->maMipData[mipIdx].maData;
			mrExport->maMipData[mipIdx].mvDim		= pExportInfo->maMipData[mipIdx].mvDim;
			mrExport->maMipData[mipIdx].muStride	= pExportInfo->maMipData[mipIdx].muStride;
		}
	}
	return TRUE;
}

}

#endif //ZEN_EXPORT_OR_RESOURCE_DX12
