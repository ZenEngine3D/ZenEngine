#pragma once
#ifndef __LibExport_Serializer_ExportItem_h__
#define __LibExport_Serializer_ExportItem_h__

namespace EExp
{

struct ExportInfoBase
{
								ExportInfoBase();
								~ExportInfoBase();
	zenResID			mExportResID;		//!< Export Resource ID (temporarily ID assigned until exported)
	void*						mpExportInfoExt;	//!< Use to extended export info, particular to one platform instead of all
	zenU8							mbSuccessStart	: 1;
	zenU8							mbSuccessWork	: 1;
	zenU8							mbSuccessEnd	: 1;
	bool						IsSuccess(){return mbSuccessStart && mbSuccessWork && mbSuccessEnd; }
};

class ExportItem : public EExp::SerialItem
{
public:
								ExportItem();

	bool						Export( EExp::ExportInfoBase& _ExportInfo );	//!< @brief	Export this item, from current thread
	ExportInfoBase*				mpExportInfo;									//!< @brief	Informations needed to export this item (child class a child ExportInfo with parameters specific to them)	
protected:								
	virtual bool				ExportStart();									//!< @brief Called from Thread:Main before the start of export
	virtual bool				ExportWork(bool _bIsTHRTask){return TRUE;};		//!< @brief Called from Thread:Main or Thread:Task for main export operation (must be threadsafe)
	virtual bool				ExportEnd();									//!< @brief Called from Thread:Main, for some post export task 
	inline	void				ExportSkipWork();								//!< @brief Call in ExportStart to avoid launching a job for this

	//static zenResID	ValidateItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, zenResID::NameHash _hName, bool& _bExistOut);

	friend class ExportTask;
	friend class ManagerExport;
};
	
inline void ExportItem::ExportSkipWork()
{
	ZENAssertMsg(mpExportInfo, "Call only from ExportStart()"); 
	mpExportInfo->mbSuccessWork = TRUE; 
}

inline zenResID ValidateItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, zenResID::NameHash _hName, bool& _bExistOut)
{
	zenResID newResID(_hName, _ePlatform, _eType, _eSource);
	EExp::SerialItem* pItem	= EMgr::SerialItems.GetItemBaseAnySource( newResID );
	_bExistOut				= pItem != NULL;
	return _bExistOut ? pItem->mResID : newResID;
}

}

#endif