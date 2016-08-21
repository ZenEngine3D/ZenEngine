#include "zcCore.h"

namespace zcExp
{
	ExportInfoBase::ExportInfoBase()
	: mExportResID()
	, mbSuccessStart(TRUE)
	, mbSuccessWork(TRUE)
	, mbSuccessEnd(TRUE)
	{
	}
	
	ExportInfoBase::~ExportInfoBase()
	{
	}

	//=================================================================
	ExporterBase::ExporterBase(const ExportDataRef& _rExportData)
	: mrExportData(_rExportData)
	{
	}

	//=============================================================================================
	//! @brief	Export this item, from current thread
	//! @detail	
	//---------------------------------------------------------------------------------------------
	//! @param _ExportInfo - Parameters received with export infos
	//=============================================================================================
	bool ExporterBase::Export( zcExp::ExportInfoBase& _ExportInfo )
	{
		mpExportInfo					= &_ExportInfo;
		mpExportInfo->mbSuccessWork		= false;

		_ExportInfo.mbSuccessStart		= ExportStart();
		if( _ExportInfo.mbSuccessStart && !mpExportInfo->mbSuccessWork )	
			_ExportInfo.mbSuccessWork	= ExportWork(FALSE);
		_ExportInfo.mbSuccessEnd		= ExportEnd();
		mpExportInfo					= nullptr;

		return _ExportInfo.IsSuccess() && mrExportData.IsValid() && mrExportData->mResID.IsValid();
	}

	//=============================================================================================
	//! @brief	Always called at the start of Export, in Thread:Main
	//! @detail	
	//=============================================================================================
	bool ExporterBase::ExportStart()
	{ 
		mrExportData->mResID = mpExportInfo ? mpExportInfo->mExportResID : zResID();
		return mrExportData->mResID.IsValid();
	}

	//=============================================================================================
	//! @brief	Always called at the end of Export, in Thread:Main
	//! @detail	
	//=============================================================================================
	bool ExporterBase::ExportEnd()
	{	
		if( mpExportInfo->IsSuccess() )
		{
			mrExportData->muVersion		= zcDepot::ExportData.GetEngineVersion(mpExportInfo->mExportResID.GetType());
			mrExportData->mExportTime	= zenSys::GetTimeStamp();			
			zcDepot::ExportData.Set(mrExportData.Get());
		}		
		
		return true;
	}
	
	bool ExporterNone::ExportStart()
	{
		zenAssertMsg(0, "Exporting an unsupported resource");
		return false;
	}
}

