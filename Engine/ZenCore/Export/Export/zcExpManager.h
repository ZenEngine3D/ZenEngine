#pragma once

namespace zcExp
{	

class ManagerExport : public zbType::Manager
{
zenClassDeclare(ManagerExport, zbType::Manager)
public:
	typedef zResID				(*CBCreateItem)(ExportInfoBase*);
	typedef zResID				(*CBGetItemID)(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
public:
								ManagerExport();			
		
	void						SetExportInfos(ePlatform _aPlatforms[kePlatform__Count], zenConst::eResSource _eExportSource, CBCreateItem _pExportCBCreateItem );
	zResID						CreateItem( zResID::ePlatformType _ePlatformType, zenConst::eResType _eResourceType, ExportInfoBase* _pExportInfoBase );
	zResID						GetNewResourceID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	zResID						GetNewResourceID(zenConst::eResType _eType);
protected:
	ePlatform					maPlatforms[kePlatform__Count];	//!< Current platform renderer we're exporting for (only 1 per export program execution)
	zenConst::eResSource		meSource;										//!< Source exported item will be configured to be (runtime/offline)
	
	CBCreateItem				mpCallbackCreateItem;							//!< Callback to create new resource
	CBGetItemID					mpCallbackGetItemID[kePlatform__Count][keResType__Count];	//!< Callback to find name assigned to new resource to exporte
	
	static zResID				CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	static zU32					saNextID[keResType__Count];			//!< NextId we can use for runtime resource name (or temp export name), for each resource type
	friend class ExportItem;
	friend class ExporterBase;

#if 0 //! @todo refactor re-implement this
	zArrayDynamic<ResDataRef>	maExportSuccess;								//!< List of successfully exported tasks
	zArrayDynamic<ResDataRef>	maExportFail;									//!< List of failed exported tasks
	zU32						muExportPending = 0;							//!< Number of pending export tasks
public:
	bool						HasPendingTask() const;
	void						WaitExportDone()const;
#endif

//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool	Load();
	virtual	bool	Unload();

};	
}

namespace zcMgr { extern zcExp::ManagerExport Export; }
