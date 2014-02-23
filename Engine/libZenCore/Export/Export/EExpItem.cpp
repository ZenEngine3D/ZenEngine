#include "libZenCore.h"

namespace EExp
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
		AWDelNull(mpExportInfoExt);
	}

	ExportItem::ExportItem()
	: mpExportInfo(NULL)
	{		
	}

	//=============================================================================================
	//! @brief	Export this item, from current thread
	//! @detail	
	//---------------------------------------------------------------------------------------------
	//! @param _ExportInfo - Parameters received with export infos
	//=============================================================================================
	bool ExportItem::Export( EExp::ExportInfoBase& _ExportInfo )
	{
		mpExportInfo					= &_ExportInfo;
		mpExportInfo->mbSuccessWork		= false;

		_ExportInfo.mbSuccessStart		= ExportStart();
		if( _ExportInfo.mbSuccessStart && !mpExportInfo->mbSuccessWork )	
			_ExportInfo.mbSuccessWork	= ExportWork(FALSE);
		_ExportInfo.mbSuccessEnd		= ExportEnd();
		mpExportInfo					= NULL;
		return _ExportInfo.IsSuccess() && mResID.IsValid();
	}

	//=============================================================================================
	//! @brief	Always called at the start of Export, in Thread:Main
	//! @detail	
	//=============================================================================================
	bool ExportItem::ExportStart()
	{ 
		mResID = mpExportInfo ? mpExportInfo->mExportResID : zenResID(); 
		return mResID.IsValid();
	}

	//=============================================================================================
	//! @brief	Always called at the end of Export, in Thread:Main
	//! @detail	
	//=============================================================================================
	bool ExportItem::ExportEnd()
	{		
		if( mpExportInfo->IsSuccess() )
		{
			mVersion		= sVersions[mResID.Type()];
			mExportTime		= zenSys::GetTimeStamp();			
			EMgr::SerialItems.SetItem(this);
		}		
		EMgr::Export.ExportDone(this);
		return true;
	}
}