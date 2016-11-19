#include "zcCore.h"

//SF DX12 

namespace zcExp
{

ExporterGfxBuffer_DX12::ExporterGfxBuffer_DX12(const ExportDataRef& _rExportData)
: ExporterBase(_rExportData.GetSafe())
, mrExportData(_rExportData)
{
	zenAssert(mrExportData.IsValid());
}

bool ExporterGfxBuffer_DX12::ExportStart()
{		
	ExportInfoGfxBuffer* pExportInfo = static_cast<ExportInfoGfxBuffer*>(mpExportInfo);	
	mrExportData->muElementSize		= pExportInfo->muElementSize;
	mrExportData->muElementCount	= zenMath::Max<zU32>(pExportInfo->muElementCount, static_cast<zU32>(pExportInfo->maData.Count())/pExportInfo->muElementSize );
	mrExportData->muElementStride	= pExportInfo->muElementSize;
	mrExportData->mUseFlags			= pExportInfo->mUseFlags;
	zenAssert( pExportInfo->maData.Count() >= pExportInfo->maData.SizeMem());
	if( pExportInfo->maData.Count() > 0 )
	{		
		mrExportData->maData.SetCount(mrExportData->muElementStride*mrExportData->muElementCount);
		zenMem::Zero(mrExportData->maData.First(), mrExportData->maData.SizeMem() );
		if( mrExportData->muElementStride == pExportInfo->muElementSize )
		{
			zenMem::Copy(mrExportData->maData.First(), pExportInfo->maData.First(), pExportInfo->maData.SizeMem() );
		}
		else
		{			
			zU8* pSource			= pExportInfo->maData.First();
			zU8* pDest				= mrExportData->maData.First();
			const zU32 uDestStride	= mrExportData->muElementStride;
			for(zU32 idx(0); idx<pExportInfo->muElementCount; ++idx)
			{
				zenMem::Copy(pDest, pSource, pExportInfo->muElementSize);
				pDest	+= uDestStride;
				pSource	+= pExportInfo->muElementSize;
			}
		}
	}
	ExportSkipWork();
	return Super::ExportStart();		
}

}

