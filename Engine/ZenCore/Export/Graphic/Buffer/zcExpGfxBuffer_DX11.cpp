#include "zcCore.h"

namespace zcExp
{

ExporterGfxBuffer_DX11::ExporterGfxBuffer_DX11(const ExportDataRef& _rExportData)
: ExporterBase(_rExportData.GetSafe())
, mrExportData(_rExportData)
{
	zenAssert(mrExportData.IsValid());
}

bool ExporterGfxBuffer_DX11::ExportStart()
{		
	ExportInfoGfxBuffer* pExportInfo = static_cast<ExportInfoGfxBuffer*>(mpExportInfo);	
	mrExportData->muElementSize		= pExportInfo->muElementSize;
	mrExportData->muElementCount	= zenMath::Max<zU32>(pExportInfo->muElementCount, static_cast<zU32>(pExportInfo->maData.size())/pExportInfo->muElementSize );
	mrExportData->muElementStride	= pExportInfo->muElementSize;
	mrExportData->mUseFlags			= pExportInfo->mUseFlags;
	zenAssert( pExportInfo->maData.size() >= pExportInfo->maData.SizeMem());
	if( pExportInfo->maData.size() > 0 )
	{				
		if( mrExportData->muElementStride == pExportInfo->muElementSize )
		{
			mrExportData->maData.Copy(pExportInfo->maData);
		}
		else
		{			
			zU8* pSource			= pExportInfo->maData.Data();
			zU8* pDest				= mrExportData->maData.Data();
			const zU32 uDestStride	= mrExportData->muElementStride;
			mrExportData->maData.resize(mrExportData->muElementStride*mrExportData->muElementCount, 0);
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

