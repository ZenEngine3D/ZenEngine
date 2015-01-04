#pragma once
#ifndef __BetlWndResourceBrowser_h__
#define __BetlWndResourceBrowser_h__ 

class WndAssetBrowser : public wxPanel
{
public:
							WndAssetBrowser(wxWindow *parent, const wxString& title);
							~WndAssetBrowser();
	enum enumAssetView{ keAssetView_Icon, keAssetView_List, keAssetView_Details, keAssetView__Count, keAssetView__Invalid };	
	ZENInline bool			IsViewIcon()const	{ return meAssetView == keAssetView_Icon; }
	ZENInline bool			IsViewList()const	{ return meAssetView == keAssetView_List; }
	ZENInline bool			IsViewDetail()const	{ return meAssetView == keAssetView_Details; }
	
	zenAss::zAssetItemRef	GetFirstSelectedItem(zenType::zFlagAssetType _TypeFilter = zenType::zFlagAssetType(true) );
	void					SelectItem(zenAss::zAssetItemRef _rItem);
protected:	
	void					CreateSectionType();
	void					CreateSectionPackage();
	void					CreateSectionAsset();
	void					RecreateSectionAsset();
	void					AddPackageDir(wxTreeListItem _PackageParent, const wxDir& _Directory);
	void					UpdatePackageList();
	void					UpdatePackageList(wxTreeListItem _treeParentID, const zenAss::zPackageGroupRef& _rGroupToAdd, const zMap<bool>::Key64& _dSelectedItems);
	void					UpdateAssetList();
	void					UpdateChildPackageChecked(wxTreeListItem _ParentTreeID);
	void					UpdatePackageSelected();
	void					UpdateAssetCount();
	void					ClearAssetList();

	void					OnAssetViewChange(wxCommandEvent& event);
	void					OnAssetViewActivated(wxListEvent& event);
	void					OnAssetKeyDown(wxListEvent& event);
	void					OnTypeItemChecked(wxTreeListEvent& event);
	//void					OnPackageViewFlat(wxTreeListEvent& event);
	void					OnPackageItemChecked(wxTreeListEvent& event);
	void					OnPackageItemActivated(wxTreeListEvent& event);
	void					OnPackageViewFlat(wxCommandEvent& event);
	void					OnPackageItemContextMenu(wxTreeListEvent& event);
	void					OnPackageItemContextMenu_SaveAll(const wxTreeListItem& _PackageItem);
	void					OnPackageDragPossible(wxDataViewEvent& event);
	void					OnPackageDragBegin(wxDataViewEvent& event);
	void					OnPackageDragDrop(wxDataViewEvent& event);

	void					OnPackageItemContextMenu_PackageAdd(const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_PackageDel(const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_PackageRen(const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_PackageMove(const wxTreeListItem& _PackageItem);

	void					OnPackageItemContextMenu_PackageGroupAdd(const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_PackageGroupDel(const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_PackageGroupRen(const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_PackageNewAsset(zenConst::eAssetType _eType, const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_AssetGroupAdd(const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_AssetGroupDel(const wxTreeListItem& _PackageItem);
	void					OnPackageItemContextMenu_AssetGroupRen(const wxTreeListItem& _PackageItem);
	void					OnKeyUp(wxKeyEvent& event);


	DECLARE_EVENT_TABLE()

	wxAuiManager						mLayoutMgr;
	wxPanel*							mpPanelType;
	wxPanel*							mpPanelPackage;
	wxPanel*							mpPanelAsset;
	wxTreeListCtrl*						mpTreeTypes;
	wxTreeListCtrl*						mpTreePackage;
	wxSearchCtrl*						mpTxtPackagesFilter;
	wxListView*							mpLstAsset;
	wxSearchCtrl*						mpTxtAssetFilter;
	wxSizer*							mpSizerAsset;	
	wxImageList*						mpIconAsset;			//!< Icon array for each resource preview
	wxImageList*						mpIconPackage;			//!< Icon array for packages
	zArrayStatic<zenAss::zPackageRef>	maPackageSelected;		//!< List of selected package to display content for
	zMap<zUInt>::Key64					mdCountPerPackage;		//!< Updated on 'UpdateAssetList()' to reflect number of asset items per package
	zArrayStatic<zUInt>					maCountPerType;			//!< Updated on 'UpdateAssetList()' to reflect number of asset items per type
	zFlagAssetType						mAssetTypeMask;			//!< Mask of selected Asset Type	
	bool								mbPackageViewFlat;
	enumAssetView						meAssetView;
	wxTreeListItem						mPackageDrag;			//!< TreeItem (for Package or group) selected during drag & drop
};

wxTreeListItem GetTreeItemFromDataview( wxTreeListCtrl& _TreeControl, wxDataViewItem _DataViewItem );

#endif