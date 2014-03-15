#pragma once
#ifndef __LibExport_Export_Manager_h__
#define __LibExport_Export_Manager_h__

namespace EExp
{	

class ManagerExport : public zbType::Manager
{
ZENClassDeclare(ManagerExport, zbType::Manager)
public:
	typedef zResID	(*CBCreateItem)(ExportInfoBase*);
	typedef zResID	(*CBGetItemID)(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
public:
								ManagerExport();			
	bool						HasPendingTask()const;
	void						WaitExportDone()const;
	
	void						SetExportInfos(zenConst::eResPlatform _aPlatforms[zenConst::keResPlatform__Count], zenConst::eResSource _eExportSource, CBCreateItem _pExportCBCreateItem );
	zResID			CreateItem( zResID::ePlatformType _ePlatformType, zenConst::eResType _eResourceType, ExportInfoBase* _pExportInfoBase );
	zResID			GetNewResourceID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
protected:
	void						ExportDone(ExportItem* _pExportItem);

	zenConst::eResPlatform		maPlatforms[zenConst::keResPlatform__Count];	//!< Current platform renderer we're exporting for (only 1 per export program execution)
	zenConst::eResSource			meSource;										//!< Source exported item will be configured to be (runtime/offline)
	zArrayDynamic<ExportItem*>	maExportSuccess;								//!< List of successfully exported tasks
	zArrayDynamic<ExportItem*>	maExportFail;									//!< List of failed exported tasks
	zU32							muExportPending;								//!< Number of pending export tasks
	CBCreateItem				mpCallbackCreateItem;															//!< Callback to create new resource
	CBGetItemID					mpCallbackGetItemID[zenConst::keResPlatform__Count][zenConst::keResType__Count];	//!< Callback to find name assigned to new resource to exporte
	
	static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	static zU32					saNextID[zenConst::keResType__Count];			//!< NextId we can use for runtime resource name (or temp export name), for each resource type
	friend class ExportItem;
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool	Load();
	virtual	bool	Unload();

};	
}

namespace EMgr { extern EExp::ManagerExport Export; }

#endif