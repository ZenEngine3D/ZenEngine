#pragma once
#ifndef __zCore_Exp_ExportItem_h__
#define __zCore_Exp_ExportItem_h__

namespace zcExp
{

//! @todo clean move all the data in this class, directly in exporter?
struct ExportInfoBase
{
								ExportInfoBase();
								~ExportInfoBase();
	zResID						mExportResID;		//!< Export Resource ID (temporarily ID assigned until exported)
	zU8							mbSuccessStart	: 1;
	zU8							mbSuccessWork	: 1;
	zU8							mbSuccessEnd	: 1;
	bool						IsSuccess(){return mbSuccessStart && mbSuccessWork && mbSuccessEnd; }
};

typedef zEngineRef<zenRes::zExportData>		ExportDataRef;

//! @todo Clean Make this use fiber/task system
class ExporterBase
{
public:
								ExporterBase(const ExportDataRef& _rExportData);
	bool						Export( zcExp::ExportInfoBase& _ExportInfo );	//!< @brief	Export this item, from current thread
	
protected:
	
	virtual bool				ExportStart();									//!< @brief Called from Thread:Main before the start of export
	virtual bool				ExportWork(bool _bIsTHRTask){return TRUE;};		//!< @brief Called from Thread:Main or Thread:Task for main export operation (must be threadsafe)
	virtual bool				ExportEnd();									//!< @brief Called from Thread:Main, for some post export task 
	ZENInline	void			ExportSkipWork();								//!< @brief Call in ExportStart to avoid launching a job for this
	ExportDataRef				mrExportData	= nullptr;
	ExportInfoBase*				mpExportInfo	= nullptr;						//!< @brief	Informations needed to export this item (child class a child ExportInfo with parameters specific to them)	
	
	friend class ExportTask;
	friend class ManagerExport;
};

class ExporterNone : public ExporterBase
{
ZENClassDeclare(ExporterNone, ExporterBase)
public:
	virtual bool ExportStart();
};

ZENInline void ExporterBase::ExportSkipWork()
{
	zenAssertMsg(mpExportInfo, "Call only from ExportStart()"); 
	mpExportInfo->mbSuccessWork = TRUE;
}

ZENInline zResID ValidateItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, zResID::NameHash _hName, bool& _bExistOut)
{
	zResID newResID(_hName, _ePlatform, _eType, _eSource);
	zEngineConstRef<zenRes::zExportData> rExportData	= zcDepot::ExportData.GetAnySource( newResID );
	_bExistOut											= rExportData.IsValid();
	return _bExistOut ? rExportData->mResID : newResID;
}

}

#endif