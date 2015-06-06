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
			mrResData->muVersion		= SerialItem::sVersions[mpExportInfo->mExportResID.Type()];
			mrResData->mExportTime	= zenSys::GetTimeStamp();			
			EMgr::SerialItems.SetItem(mrResData.Get()); //! @todo urgent transform SerialItem manager to use refcount
		}		
		EMgr::Export.ExportDone(mrResData.Get()); //! @todo replug this with new system
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

