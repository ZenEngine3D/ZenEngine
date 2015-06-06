#include "zcCore.h"

namespace zcExp
{
	ExportInfoBase::ExportInfoBase()
	: mExportResID()
	, mpExportInfoExt(NULL)
	, mbSuccessStart(TRUE)
	, mbSuccessWork(TRUE)
	, mbSuccessEnd(TRUE)
	{
	}
	
	ExportInfoBase::~ExportInfoBase()
	{
		zenDelNull(mpExportInfoExt);
	}

	//=================================================================
	ExporterBase::ExporterBase(const ExportDataRef& _rExportData)
	: mpExportInfo(NULL)
	, mrExportData(_rExportData)
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
		mpExportInfo					= NULL;
		return _ExportInfo.IsSuccess() && mrExportData->mResID.IsValid();
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
			mrExportData->muVersion		= SerialItem::sVersions[mpExportInfo->mExportResID.Type()];
			mrExportData->mExportTime	= zenSys::GetTimeStamp();			
			EMgr::SerialItems.SetItem(mrExportData.Get()); //! @todo urgent transform SerialItem manager to use refcount
		}		
		EMgr::Export.ExportDone(mrExportData.Get()); //! @todo replug this with new system
		return true;
	/*	
		if( mpExportInfo->IsSuccess() )
		{
			muVersion		= sVersions[mpExportInfo->mExportResID.Type()];
			mExportTime		= zenSys::GetTimeStamp();			
			EMgr::SerialItems.SetItem(this);
		}		
		EMgr::Export.ExportDone(this);
		return true;
		*/
	}
	
	bool ExporterNone::ExportStart()
	{
		ZENAssertMsg(0, "Exporting an unsupported resource");
		return false;
	}
}

