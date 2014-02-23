#include "libZenCore.h"

namespace EExp
{
	ExportTask::ExportTask(ExportItem* _pExportItem, EExp::ExportInfoBase* _pExportInfo)
	: mpExportItem(_pExportItem)
	{
		AWAssert(mpExportItem);
		mpExportItem->mpExportInfo	= _pExportInfo;
	}

	bool ExportTask::Start()
	{
		Super::Start();
		mpExportItem->mpExportInfo->mbSuccessStart = mpExportItem->ExportStart();
		return mpExportItem->mpExportInfo->mbSuccessStart;
	}

	bool ExportTask::THRTask_Execute()
	{		
		mpExportItem->mpExportInfo->mbSuccessWork = mpExportItem->ExportWork(TRUE); 
		return mpExportItem->mpExportInfo->mbSuccessWork;
	}

	bool ExportTask::End()
	{
		Super::End();
		mpExportItem->mpExportInfo->mbSuccessEnd = mpExportItem->ExportEnd();		
		return mpExportItem->mpExportInfo->mbSuccessEnd;
	}
}

