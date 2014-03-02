#pragma once
#ifndef __BetlWndResourceBrowser_h__
#define __BetlWndResourceBrowser_h__ 

class WndAssetBrowser : public wxPanel
{
public:
					WndAssetBrowser(wxWindow *parent, const wxString& title);
					~WndAssetBrowser();

						
	inline bool		IsViewIcon()const	{ return mpRdioAssetView[0]->GetValue(); }
	inline bool		IsViewList()const	{ return mpRdioAssetView[1]->GetValue(); }
	inline bool		IsViewDetail()const	{ return mpRdioAssetView[2]->GetValue(); }
protected:	
	void			CreateSectionType();
	void			CreateSectionPackage();
	void			CreateSectionAsset();	
	void			RecreateSectionAsset();
	void			AddPackageDir( wxTreeListItem _PackageParent, const wxDir& _Directory );
	void			AddPackages();
	void			UpdateAssetList();
	void			UpdatePackageChecked(wxTreeListItem _ParentTreeID);
	void			UpdateAssetCount();

	DECLARE_EVENT_TABLE()
	void			OnAssetViewChange(wxCommandEvent& event);
	void			OnAssetViewActivated(wxListEvent& event);
	void			OnTypeItemChecked(wxTreeListEvent& event);
	void			OnPackageItemChecked(wxTreeListEvent& event);
	void			OnPackageItemActivated(wxTreeListEvent& event);
	void			OnPackageItemContextMenu(wxTreeListEvent& event);
	void			OnPackageItemContextMenu_PackageAdd(const wxTreeListItem& _PackageItem);
	void			OnPackageItemContextMenu_PackageDel(const wxTreeListItem& _PackageItem);
	void			OnPackageItemContextMenu_PackageRen(const wxTreeListItem& _PackageItem);
	void			OnPackageItemContextMenu_PackageGroupAdd(const wxTreeListItem& _PackageItem);
	void			OnPackageItemContextMenu_PackageGroupDel(const wxTreeListItem& _PackageItem);
	void			OnPackageItemContextMenu_PackageGroupRen(const wxTreeListItem& _PackageItem);
	void			OnPackageItemContextMenu_AssetGroupAdd(const wxTreeListItem& _PackageItem);
	void			OnPackageItemContextMenu_AssetGroupDel(const wxTreeListItem& _PackageItem);
	void			OnPackageItemContextMenu_AssetGroupRen(const wxTreeListItem& _PackageItem);
	
	wxAuiManager					mLayoutMgr;
	wxPanel*						mpPanelType;
	wxPanel*						mpPanelPackage;
	wxPanel*						mpPanelAsset;
	wxTreeListCtrl*					mpTreeTypes;
	wxTreeListCtrl*					mpTreePackage;
	wxSearchCtrl*					mpTxtPackagesFilter;
	wxListView*						mpLstAsset;
	wxSearchCtrl*					mpTxtAssetFilter;
	wxRadioButton*					mpRdioAssetView[3];
	wxSizer*						mpSizerAsset;	
	wxImageList*					mpIconAsset;			//!< Icon array for each resource preview
	wxImageList*					mpIconPackage;			//!< Icon array for packages
	zenArrayStatic<zeAss::Package*>	maPackageSelected;		//!< List of selected package to display content for
	zenMap<zenUInt>::Key32			mdCountPerPackage;		//!< Updated on 'UpdateAssetList()' to reflect number of asset items per package
	zenArrayStatic<zenUInt>			maCountPerType;			//!< Updated on 'UpdateAssetList()' to reflect number of asset items per type
	zenU64							mAssetTypeMask;			//!< Mask of selected Asset Type	
};

#endif