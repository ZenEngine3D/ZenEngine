#pragma once
#ifndef __LibExport_Export_Manager_h__
#define __LibExport_Export_Manager_h__

namespace EExp
{	

class ManagerExport : public zbType::Manager
{
AWClassDeclare(ManagerExport, zbType::Manager)
public:
	typedef awResourceID	(*CBCreateItem)(ExportInfoBase*);
	typedef awResourceID	(*CBGetItemID)(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
public:
								ManagerExport();			
	bool						HasPendingTask()const;
	void						WaitExportDone()const;
	
	void						SetExportInfos(awconst::eResPlatform _aPlatforms[awconst::keResPlatform__Count], awconst::eResSource _eExportSource, CBCreateItem _pExportCBCreateItem );
	awResourceID			CreateItem( awResourceID::ePlatformType _ePlatformType, awconst::eResType _eResourceType, ExportInfoBase* _pExportInfoBase );
	awResourceID			GetNewResourceID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
protected:
	void						ExportDone(ExportItem* _pExportItem);

	awconst::eResPlatform		maPlatforms[awconst::keResPlatform__Count];	//!< Current platform renderer we're exporting for (only 1 per export program execution)
	awconst::eResSource			meSource;										//!< Source exported item will be configured to be (runtime/offline)
	awArrayDynamic<ExportItem*>	maExportSuccess;								//!< List of successfully exported tasks
	awArrayDynamic<ExportItem*>	maExportFail;									//!< List of failed exported tasks
	awU32							muExportPending;								//!< Number of pending export tasks
	CBCreateItem				mpCallbackCreateItem;															//!< Callback to create new resource
	CBGetItemID					mpCallbackGetItemID[awconst::keResPlatform__Count][awconst::keResType__Count];	//!< Callback to find name assigned to new resource to exporte
	
	static awResourceID	CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	static awU32					saNextID[awconst::keResType__Count];			//!< NextId we can use for runtime resource name (or temp export name), for each resource type
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