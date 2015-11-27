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
	ExporterBase::ExporterBase(const ResDataRef& _rResData)
	: mpExportInfo(NULL)
	, mrResData(_rResData)
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
		return _ExportInfo.IsSuccess() && mrResData->mResID.IsValid();
	}

	//=============================================================================================
	//! @brief	Always called at the start of Export, in Thread:Main
	//! @detail	
	//=============================================================================================
	bool ExporterBase::ExportStart()
	{ 
		mrResData->mResID = mpExportInfo ? mpExportInfo->mExportResID : zResID(); 		
		return mrResData->mResID.IsValid();
	}

	//=============================================================================================
	//! @brief	Always called at the end of Export, in Thread:Main
	//! @detail	
	//=============================================================================================
	bool ExporterBase::ExportEnd()
	{	
		if( mpExportInfo->IsSuccess() )
		{
			mrResData->muVersion	= zcDepot::ResourceData.GetEngineVersion(mpExportInfo->mExportResID.GetType());
			mrResData->mExportTime	= zenSys::GetTimeStamp();			
			zcDepot::ResourceData.SetItem(mrResData);
		}		
		zcMgr::Export.ExportDone(mrResData); //! @todo replug this with new system
		return true;
	}
	
	bool ExporterNone::ExportStart()
	{
		ZENAssertMsg(0, "Exporting an unsupported resource");
		return false;
	}
}

