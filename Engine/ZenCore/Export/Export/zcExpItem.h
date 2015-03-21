#pragma once
#ifndef __zCore_Exp_ExportItem_h__
#define __zCore_Exp_ExportItem_h__

namespace zcExp
{

struct ExportInfoBase
{
								ExportInfoBase();
								~ExportInfoBase();
	zResID						mExportResID;		//!< Export Resource ID (temporarily ID assigned until exported)
	void*						mpExportInfoExt;	//!< Use to extended export info, particular to one platform instead of all
	zU8							mbSuccessStart	: 1;
	zU8							mbSuccessWork	: 1;
	zU8							mbSuccessEnd	: 1;
	bool						IsSuccess(){return mbSuccessStart && mbSuccessWork && mbSuccessEnd; }
};

class ExportItem : public zcExp::SerialItem
{
public:
								ExportItem();

	bool						Export( zcExp::ExportInfoBase& _ExportInfo );	//!< @brief	Export this item, from current thread
	ExportInfoBase*				mpExportInfo;									//!< @brief	Informations needed to export this item (child class a child ExportInfo with parameters specific to them)	
protected:								
	virtual bool				ExportStart();									//!< @brief Called from Thread:Main before the start of export
	virtual bool				ExportWork(bool _bIsTHRTask){return TRUE;};		//!< @brief Called from Thread:Main or Thread:Task for main export operation (must be threadsafe)
	virtual bool				ExportEnd();									//!< @brief Called from Thread:Main, for some post export task 
	ZENInline	void			ExportSkipWork();								//!< @brief Call in ExportStart to avoid launching a job for this

	//static zResID	ValidateItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, zResID::NameHash _hName, bool& _bExistOut);

	friend class ExportTask;
	friend class ManagerExport;
};
	
ZENInline void ExportItem::ExportSkipWork()
{
	ZENAssertMsg(mpExportInfo, "Call only from ExportStart()"); 
	mpExportInfo->mbSuccessWork = TRUE; 
}

ZENInline zResID ValidateItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, zResID::NameHash _hName, bool& _bExistOut)
{
	zResID newResID(_hName, _ePlatform, _eType, _eSource);
	zcExp::SerialItem* pItem	= EMgr::SerialItems.GetItemBaseAnySource( newResID );
	_bExistOut				= pItem != NULL;
	return _bExistOut ? pItem->mResID : newResID;
}

}

#endif