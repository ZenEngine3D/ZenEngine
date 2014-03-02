#include "ZendoPCH.h"

#include "../ZendoApp.h"
#include "WndAssetBrowser.h"
#include "WndAssetProperty.h"

enum ControlID
{
	ID_Asset_Rdio_View = wxID_HIGHEST,
	ID_Asset_ListView,
	ID_Type_Tree,
	ID_Package_Tree,	
};

enum IconPackage
{
	IcoPkg_Folder,
	IcoPkg_FolderOpen,
	IcoPkg_Package
};

//=================================================================================================
// EVENTS TABLE
//=================================================================================================
BEGIN_EVENT_TABLE(WndAssetBrowser, wxPanel)
	EVT_TREELIST_ITEM_CHECKED(ID_Type_Tree,			WndAssetBrowser::OnTypeItemChecked)
	EVT_TREELIST_ITEM_CHECKED(ID_Package_Tree,		WndAssetBrowser::OnPackageItemChecked)
	EVT_TREELIST_ITEM_ACTIVATED(ID_Package_Tree,	WndAssetBrowser::OnPackageItemActivated)
	EVT_TREELIST_ITEM_CONTEXT_MENU(ID_Package_Tree, WndAssetBrowser::OnPackageItemContextMenu)
	EVT_RADIOBUTTON(ID_Asset_Rdio_View,				WndAssetBrowser::OnAssetViewChange)
	EVT_LIST_ITEM_ACTIVATED(ID_Asset_ListView,		WndAssetBrowser::OnAssetViewActivated)
END_EVENT_TABLE()

class wxPackageClientData : public wxClientData
{
public:
	wxPackageClientData(zeAss::Package* _pPackage, const wxString& _zGroups)
	: mpPackage(_pPackage)
	, mzGroups(_zGroups)
	, mhGroupID( static_cast<const char*>(_zGroups) )
	{
	}
	zeAss::Package*	mpPackage;	//!< Package associated with tree item (if null, then haven't reach a package yet)
	wxString		mzGroups;	//!< List of package/asset groups (separated by '\') to reach current tree item
	zenHash32		mhGroupID;	//!< Unique ID for this tree item (based on mzGroups). Used to get fill tree with assets stats
};

WndAssetBrowser::WndAssetBrowser(wxWindow *parent, const wxString& title)
: wxPanel(parent, wxID_ANY)
, mLayoutMgr(this, wxAUI_MGR_ALLOW_FLOATING|wxAUI_MGR_TRANSPARENT_HINT|wxAUI_MGR_HINT_FADE|wxAUI_MGR_VENETIAN_BLINDS_HINT)
, mpPanelType(NULL)
, mpPanelPackage(NULL)
, mpPanelAsset(NULL)
, mpTreeTypes(NULL)
, mpTreePackage(NULL)
, mpTxtPackagesFilter(NULL)
, mpLstAsset(NULL)
, mpTxtAssetFilter(NULL)
, mpIconAsset(NULL)
, mpIconPackage(NULL)
{	
	mAssetTypeMask = 0;	
	mdCountPerPackage.Init(128);
	mdCountPerPackage.SetDefaultValue(0);
	maCountPerType.SetCount(zeAss::AssetItem::keType__Count);

	CreateSectionAsset();
	CreateSectionPackage();
	CreateSectionType();
	
	wxAuiPaneInfo panelInfo;
	panelInfo.Left();
	panelInfo.Gripper(false);
	panelInfo.GripperTop(false);
	panelInfo.Dock();
	panelInfo.CloseButton(false);
	panelInfo.PaneBorder(false);
	panelInfo.PinButton(false);
	panelInfo.CaptionVisible(true);

	if( mpPanelType ) 
	{
		panelInfo.Caption("Asset Types");
		mLayoutMgr.AddPane(mpPanelType, panelInfo);
	}
	if( mpPanelPackage ) 
	{
		panelInfo.Caption("Packages");
		mLayoutMgr.AddPane(mpPanelPackage, panelInfo);
	}

	panelInfo.Center();
	panelInfo.Caption("Assets");
	mLayoutMgr.AddPane(mpPanelAsset, panelInfo);
	SetSize(wxSize(500,200));
	mLayoutMgr.Update();

}

WndAssetBrowser::~WndAssetBrowser()
{
	mLayoutMgr.UnInit();	// deinitialize the frame manager
	zenDel(mpIconAsset);
	zenDel(mpIconPackage);
}

void WndAssetBrowser::CreateSectionType()
{
	mpPanelType			= zenNewDefault wxPanel(this);
	wxBoxSizer* sizer	= zenNewDefault wxBoxSizer(wxVERTICAL);
		
	mpTreeTypes = zenNewDefault wxTreeListCtrl(mpPanelType, ID_Type_Tree,	wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE | wxTL_CHECKBOX);
	mpTreeTypes->AppendColumn("Types", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	mpTreeTypes->AppendColumn("#", wxCOL_WIDTH_AUTOSIZE, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	mpTreeTypes->AssignImageList(wxGetApp().maIcon[ZendoApp::keIco_Asset16]);
	//mpTreeTypes->GetDataView()->SetRowHeight(33);

	wxTreeListItem rootItem = mpTreeTypes->GetRootItem();
	for(zenUInt idx(0); idx<zeAss::AssetItem::keType__Count; ++idx)
	{		
		const char* zTypeDesc =zeAss::AssetItem::GetTypeDesc( (zeAss::AssetItem::enumType)idx );
		wxTreeListItem newItemID = mpTreeTypes->AppendItem(rootItem, zTypeDesc, idx, idx); 	
	}
			
	sizer->Add( mpTreeTypes, 100, wxGROW | wxALL, 2); 	
	mpPanelType->SetSizer(sizer);
	mpPanelType->SetSize(wxSize(300,200));
}

void WndAssetBrowser::CreateSectionPackage()
{
	zenUInt uIconSize(16);
	mpPanelPackage		= zenNewDefault wxPanel(this);
	wxBoxSizer* sizer	= zenNewDefault wxBoxSizer(wxVERTICAL);
	mpIconPackage		= zenNewDefault wxImageList(uIconSize, uIconSize, true);
	mpIconPackage->Add(wxArtProvider::GetIcon(wxART_FOLDER,			wxART_LIST, wxSize(uIconSize,uIconSize)));
	mpIconPackage->Add(wxArtProvider::GetIcon(wxART_FOLDER_OPEN,	wxART_LIST, wxSize(uIconSize,uIconSize)));	
	mpIconPackage->Add(wxArtProvider::GetIcon(wxART_COPY,			wxART_LIST, wxSize(uIconSize,uIconSize)));
	//mpIconPackage->Add(wxArtProvider::GetIcon(wxART_FILE_OPEN,		wxART_LIST, wxSize(uIconSize,uIconSize)));

	// TreeView of packages -------------------------------------------------------------------
	mpTreePackage = zenNewDefault wxTreeListCtrl(mpPanelPackage, ID_Package_Tree,	wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE | wxTL_CHECKBOX | wxTL_3STATE);
	mpTreePackage->AppendColumn("Packages", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	mpTreePackage->AppendColumn("#", wxCOL_WIDTH_AUTOSIZE, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	mpTreePackage->SetImageList(mpIconPackage);
	mpTreePackage->GetDataView()->SetIndent(16);
	//AddPackageDir( mpTreePackage->GetRootItem(), wxDir((const char*)zeMgr::Asset.GetPathPackage()) );
	AddPackages();

	// Insert it into a sizer -----------------------------------------------------------------
	wxSizer* sizerPkgFilter = zenNewDefault wxBoxSizer(wxHORIZONTAL);
	mpTxtPackagesFilter = zenNewDefault wxSearchCtrl(mpPanelPackage, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	
	sizerPkgFilter->Add(mpTxtPackagesFilter,									100,	wxGROW | wxALL | wxALIGN_CENTRE_VERTICAL, 0);
	sizer->Add(sizerPkgFilter,													0,		wxGROW | wxALL, 2);
	sizer->Add(mpTreePackage,													100,	wxGROW | wxALL, 2);
	mpPanelPackage->SetSizer(sizer);
	mpPanelPackage->SetSize(wxSize(300,200));
	//-----------------------------------------------------------------------------------------
}

void WndAssetBrowser::CreateSectionAsset()
{	
	mpPanelAsset				= zenNewDefault wxPanel(this);
	wxBoxSizer* sizer			= zenNewDefault wxBoxSizer(wxVERTICAL);
	wxSizer* sizerRightOptions	= zenNewDefault wxBoxSizer(wxHORIZONTAL);

 	mpRdioAssetView[0]			= zenNewDefault wxRadioButton(mpPanelAsset, ID_Asset_Rdio_View,  wxT("Icon"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
 	mpRdioAssetView[1]			= zenNewDefault wxRadioButton(mpPanelAsset, ID_Asset_Rdio_View,  wxT("List"), wxDefaultPosition, wxDefaultSize, 0);
 	mpRdioAssetView[2]			= zenNewDefault wxRadioButton(mpPanelAsset, ID_Asset_Rdio_View,  wxT("Detail"), wxDefaultPosition, wxDefaultSize, 0);
	mpTxtAssetFilter			= zenNewDefault wxSearchCtrl(mpPanelAsset, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

	sizerRightOptions->Add(mpTxtAssetFilter,	100,	wxGROW | wxALL | wxALIGN_CENTRE_VERTICAL, 0);
	sizerRightOptions->AddSpacer(4);
	sizerRightOptions->Add(mpRdioAssetView[0],	0,		wxGROW | wxALL | wxALIGN_CENTRE_VERTICAL, 0);
	sizerRightOptions->Add(mpRdioAssetView[1],	0,		wxGROW | wxALL | wxALIGN_CENTRE_VERTICAL, 0);
	sizerRightOptions->Add(mpRdioAssetView[2],	0,		wxGROW | wxALL | wxALIGN_CENTRE_VERTICAL, 0);
	sizer->Add(sizerRightOptions,				0,		wxGROW | wxALL, 2);

	mpPanelAsset->SetSizer(sizer);	
	mpPanelAsset->SetSize(wxSize(300,200));
	RecreateSectionAsset();
}

void WndAssetBrowser::RecreateSectionAsset()
{
	if( mpLstAsset )
	{
		mpPanelAsset->GetSizer()->Detach(mpLstAsset);
		zenDelNull(mpLstAsset);
	}

	int styleFlags	= wxBORDER_THEME | wxLC_AUTOARRANGE;
	styleFlags		|=  IsViewIcon()	? wxLC_ICON		: 0;
	styleFlags		|=  IsViewList()	? wxLC_LIST		: 0;
	styleFlags		|=  IsViewDetail()	? wxLC_REPORT	: 0;
	
	mpLstAsset = zenNewDefault wxListView(mpPanelAsset, ID_Asset_ListView, wxDefaultPosition, wxDefaultSize, styleFlags);
 	mpLstAsset->SetImageList(wxGetApp().maIcon[ZendoApp::keIco_Asset32], wxIMAGE_LIST_SMALL);
	mpLstAsset->SetImageList(mpIconAsset, wxIMAGE_LIST_NORMAL);
	
	UpdateAssetList();
	mpPanelAsset->GetSizer()->Add(mpLstAsset,	100,	wxGROW | wxALL, 2);
}

void WndAssetBrowser::UpdateAssetList()
{
	mpLstAsset->Freeze();
	mpLstAsset->DeleteAllItems();	
	// Recreate Icon Image List
	
	const wxImageList* pTypeIconList = wxGetApp().maIcon[ZendoApp::keIco_Asset128];
	int sizeX, sizeY;
	pTypeIconList->GetSize(0, sizeX, sizeY);
	if( mpIconAsset )	
		mpIconAsset->RemoveAll();
	else				
		mpIconAsset = zenNewDefault wxImageList(sizeX, sizeY, true);
	for(zenUInt idx(0), count(pTypeIconList->GetImageCount()); idx<count; ++idx)
		mpIconAsset->Add( wxArtProvider::GetIcon(wxART_INFORMATION,wxART_LIST, wxSize(sizeX,sizeY)));

	// If only 1 asset type is selected, can displayed detailed column
	mpLstAsset->InsertColumn(0, wxT("Name"), wxLIST_FORMAT_LEFT);
	if( zenMath::BitsCount(mAssetTypeMask) == 1 )
	{
		//AAss::AssetItem::GetTypeDesc()
		mpLstAsset->InsertColumn(1, wxT("Description"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	}
	// Else display a descriptive string
	else
	{
		mpLstAsset->InsertColumn(1, wxT("Description"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	}
	
	// Reset count	
	maCountPerType.SetAll(0);
	mdCountPerPackage.SetAll(0);
	// Add all Asset item in selected type/packages to the list
	for(zenUInt idxPack(0), countPack(maPackageSelected.Count()); idxPack<countPack; ++idxPack)
	{
		zeAss::Package* pPackage = maPackageSelected[idxPack];
		for(zenUInt typIdx(0); typIdx<zeAss::AssetItem::keType__Count; ++typIdx)
		{
			zenUInt uPackageItemCount(0);
			if( mAssetTypeMask & zenU64(1)<<typIdx )
			{
				zenMap<zeAss::AssetItem*>::Key64::Iterator it;
				pPackage->AssetGet( zeAss::AssetItem::enumType(typIdx) ).GetFirst(it);
				while( it.IsValid() )
				{
					const zeAss::AssetItem* pItem	= it.GetValue();
					zenUInt uIconIndex					= pItem->GetType();
					if( IsViewIcon() )
					{
						const wxIcon* pIcon	= wxGetApp().GetIcon(*pItem);
						if(  pIcon )
						{
							uIconIndex = mpIconAsset->GetImageCount();
							mpIconAsset->Add( *pIcon );						
						}
					}					

					// Add the asset to list
					long itemID = mpLstAsset->InsertItem(mpLstAsset->GetItemCount(), (const char*)pItem->GetName(), uIconIndex);
					mpLstAsset->SetItemPtrData(itemID, reinterpret_cast<wxUIntPtr>(pItem));
					if( IsViewDetail() )
						mpLstAsset->SetItem(itemID, 1, (const char*)pItem->GetDescription());
					
					++uPackageItemCount;
					++maCountPerType[pItem->GetType()];
					++it;
				}
			}	

			// Update the count of asset per package
			const zenArrayStatic<zenString>& aPackageGroup = pPackage->GetGroupAndName();
			wxString zGroups="";
			for(zenUInt idx(0), count(aPackageGroup.Count()); idx<count; ++idx)
			{
				zGroups += aPackageGroup[idx];
				zGroups += wxT("\\");
				mdCountPerPackage.GetAdd(zenHash32(static_cast<const char*>(zGroups))) += uPackageItemCount;
			}
		}		
	}
	UpdateAssetCount();
	mpLstAsset->Thaw();
	mpLstAsset->Refresh();
}

void WndAssetBrowser::AddPackages()
{
	const zenMap<zeAss::Package*>::Key32& dAllPackages = zeMgr::Asset.PackageGet();
	zenMap<zeAss::Package*>::Key32::Iterator it;	
	mpTreePackage->DeleteAllItems();

	zenUInt packIdx(0);
	dAllPackages.GetFirst(it);	
	while( it.IsValid() )
	{
		zeAss::Package* pPackage					= it.GetValue();
		const zenArrayStatic<zenString> aGroupName	= pPackage->GetGroupAndName();		
		wxString zGroups						= "";

		// Look for folder under which package should be added (and create it if needed)
		wxTreeListItem treeParentID			= mpTreePackage->GetRootItem();
		for(zenUInt idx(0), count(aGroupName.Count()-1); idx<count; ++idx)
		{
			const zenString& zGroupName	= aGroupName[idx];
			wxTreeListItem treeID		= mpTreePackage->GetFirstChild(treeParentID);
			zGroups						+= static_cast<const char*>(zGroupName);
			zGroups						+= wxT("\\");
			while( treeID.IsOk() && strcmp(zGroupName, mpTreePackage->GetItemText(treeID))!=0 )
			{
				treeID = mpTreePackage->GetNextSibling(treeID);
			}
			
			if( !treeID.IsOk() )
			{
				treeID = mpTreePackage->AppendItem(treeParentID, (const char*)zGroupName, IcoPkg_Folder, IcoPkg_FolderOpen); 	
				mpTreePackage->SetItemData(treeID, zenNewDefault wxPackageClientData(NULL, zGroups));
			}
			treeParentID = treeID;
		}

		// Add the package if we've found the tree id
		wxTreeListItem newItemID = mpTreePackage->AppendItem(treeParentID, (const char*)*aGroupName.Last(), IcoPkg_Package, IcoPkg_Package);
		zGroups						+= static_cast<const char*>(*aGroupName.Last());
		zGroups						+= wxT("\\");
		mpTreePackage->SetItemData(newItemID, zenNewDefault wxPackageClientData(pPackage, zGroups));

		++it;
	}
	UpdateAssetList();
}

//=================================================================================================
//! @brief		Recursively update all child checked state based on parent
//! @details	Used by OnPackageItemChecked event
//-------------------------------------------------------------------------------------------------
//! @param		_TreeId - Current Tree item being processed
//! @return		- 
//=================================================================================================
void WndAssetBrowser::UpdatePackageChecked(wxTreeListItem _ParentTreeID)
{
	bool bChecked			= mpTreePackage->GetCheckedState(_ParentTreeID) != wxCHK_UNCHECKED;
	wxTreeListItem treeID	= mpTreePackage->GetFirstChild(_ParentTreeID);
	while( treeID.IsOk() )
	{
		if ( bChecked && mpTreePackage->GetCheckedState(treeID) == wxCHK_UNCHECKED )
			mpTreePackage->CheckItem(treeID, wxCHK_UNDETERMINED );
		else if	( !bChecked && mpTreePackage->GetCheckedState(treeID) == wxCHK_UNDETERMINED )
			mpTreePackage->CheckItem(treeID, wxCHK_UNCHECKED );
		
		UpdatePackageChecked(treeID);
		treeID = mpTreePackage->GetNextSibling(treeID);
	}
}

//=================================================================================================
//! @brief		Update the asset count in Package and Type Tree views
//! @details	Used by 'UpdateAssetList()'
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::UpdateAssetCount()
{
	// Update per Packages asset count --------------------------------------------------------
	wxTreeListItem itemCurrent	= mpTreePackage ? mpTreePackage->GetFirstItem() : NULL;
	while( itemCurrent.IsOk() )
	{
		const wxPackageClientData* pClient = static_cast<const wxPackageClientData*>(mpTreePackage->GetItemData(itemCurrent));
		zenUInt uCount = mdCountPerPackage[pClient->mhGroupID];
		mpTreePackage->SetItemText(itemCurrent, 1, wxString::Format("%i", uCount) );
		itemCurrent = mpTreePackage->GetNextItem(itemCurrent);
	}
	// Update per Type asset count ------------------------------------------------------------
	zenUInt idx(0);
	itemCurrent = mpTreeTypes ? mpTreeTypes->GetFirstItem() : NULL;
	while( itemCurrent.IsOk() )
	{
		mpTreeTypes->SetItemText(itemCurrent, 1, wxString::Format("%i", maCountPerType[idx++] ) );
		itemCurrent = mpTreeTypes->GetNextItem(itemCurrent);
	}
	//-----------------------------------------------------------------------------------------
}

//=================================================================================================
//! @brief		Called when user changes the current desired asset view
//! @details	Reconstruct the view with new view option
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::OnAssetViewChange(wxCommandEvent& event)
{
	RecreateSectionAsset();
	GetSizer()->Layout();
}

//=================================================================================================
//! @brief		User changed the current type wanted for display
//! @details	Repopulate the view with supported types
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::OnTypeItemChecked(wxTreeListEvent& event)
{		
	mAssetTypeMask = 0;
	zenUInt uTypeIndex(0);
	wxTreeListItem itemCurrent = mpTreeTypes->GetFirstItem();
	while( itemCurrent.IsOk() )
	{
		mAssetTypeMask |= mpTreeTypes->GetCheckedState(itemCurrent) == wxCHK_CHECKED ? zenU64(1)<<uTypeIndex : 0;
		itemCurrent = mpTreeTypes->GetNextItem(itemCurrent);
	}
	UpdateAssetList();
}

//=================================================================================================
//! @brief		User changed the current packages wanted for display
//! @details	Update tree children state, and repopulate the view with desired packages
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::OnPackageItemChecked(wxTreeListEvent& event)
{	
	bool bUpdate(true);

	// Cannot set item to 'unchecked' if parent is checked
	wxTreeListItem parentTreeID = mpTreePackage->GetItemParent(event.GetItem());
	if( parentTreeID.IsOk() && mpTreePackage->GetCheckedState(parentTreeID) != wxCHK_UNCHECKED )
	{
		bUpdate = false;
		if( event.GetOldCheckedState() == wxCHK_UNDETERMINED )
			mpTreePackage->CheckItem(event.GetItem(), wxCHK_CHECKED );
		else
			mpTreePackage->CheckItem(event.GetItem(), wxCHK_UNDETERMINED );
	}
	
	// There was a change to checked status, update children and asset displayed
	if( bUpdate )
	{
		UpdatePackageChecked(event.GetItem());

		// Update the list of selected packages
		zenArrayStatic<zeAss::Package*>	aPackageSelected(zeMgr::Asset.PackageGet().Count());
		wxTreeListItem itemCurrent = mpTreePackage->GetFirstItem();
		zenUInt uSelectedCount(0);
		while( itemCurrent.IsOk() )
		{
			wxPackageClientData* pClientData = static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(itemCurrent));
			if( pClientData->mpPackage && mpTreePackage->GetCheckedState(itemCurrent) != wxCHK_UNCHECKED )
				aPackageSelected[uSelectedCount++] = pClientData->mpPackage;
			itemCurrent = mpTreePackage->GetNextItem(itemCurrent);
		}
		
		maPackageSelected.Copy(aPackageSelected.First(), uSelectedCount);
		UpdateAssetList();
	}
}

//=================================================================================================
//! @brief		User 2x clicked a package, expand or collapse the item
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::OnPackageItemActivated(wxTreeListEvent& event)
{
	if( mpTreePackage->IsExpanded(event.GetItem()) )
		mpTreePackage->Collapse(event.GetItem());
	else
		mpTreePackage->Expand(event.GetItem());
}

//=================================================================================================
//! @brief		Request a new AssetTab for edition
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::OnAssetViewActivated(wxListEvent& event)
{
	zeAss::AssetItem* pItem = reinterpret_cast<zeAss::AssetItem*>(event.GetItem().GetData());
	if( pItem )
		wxGetApp().mpFrame->GetWndAssetProperty()->AddAssetTab( pItem->GetID() );
}

//=================================================================================================
//! @brief		User 2x clicked a package, expand or collapse the item
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::OnPackageItemContextMenu(wxTreeListEvent& event)
{
	enum
	{
		ID_Menu_PackageAdd,
		ID_Menu_PackageDel,
		ID_Menu_PackageRen,
		ID_Menu_PackageGroupAdd,
		ID_Menu_PackageGroupDel,
		ID_Menu_PackageGroupRen,
		ID_Menu_AssetGroupAdd,
		ID_Menu_AssetGroupDel,
		ID_Menu_AssetGroupRen,
	};

	wxMenu menu;
	const wxTreeListItem item			= event.GetItem();
	wxPackageClientData* pClientData	= static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(item));
	
	// Element is a package
	if( pClientData->mpPackage )
	{		
		menu.Append(ID_Menu_PackageRen,			"&Rename Package");
		menu.Append(ID_Menu_PackageDel,			"&Delete Package");
		menu.AppendSeparator();
		menu.Append(ID_Menu_AssetGroupAdd,		"&Create Asset Group");
	}
	// Element is a folder
	else
	{
		wxTreeListItem children = mpTreePackage->GetFirstChild(item);
		menu.Append(ID_Menu_PackageGroupAdd,	"&Create Package Group");
		menu.Append(ID_Menu_PackageGroupRen,	"&Rename Package Group");
		menu.Append(ID_Menu_PackageGroupDel,	"&Delete Package Group");		
		menu.AppendSeparator();
		menu.Append(ID_Menu_PackageAdd,			"&Create new Package");
	}

	switch ( mpTreePackage->GetPopupMenuSelectionFromUser(menu) )
	{
	case ID_Menu_PackageAdd:		OnPackageItemContextMenu_PackageAdd(item);		break;
	case ID_Menu_PackageDel:		OnPackageItemContextMenu_PackageDel(item);		break;
	case ID_Menu_PackageRen:		OnPackageItemContextMenu_PackageRen(item);		break;
	case ID_Menu_PackageGroupAdd:	OnPackageItemContextMenu_PackageGroupAdd(item);	break;
	case ID_Menu_PackageGroupDel:	OnPackageItemContextMenu_PackageGroupDel(item);	break;
	case ID_Menu_PackageGroupRen:	OnPackageItemContextMenu_PackageGroupRen(item);	break;
	case ID_Menu_AssetGroupAdd:		OnPackageItemContextMenu_AssetGroupAdd(item);	break;
	case ID_Menu_AssetGroupDel:		OnPackageItemContextMenu_AssetGroupDel(item);	break;
	case ID_Menu_AssetGroupRen:		OnPackageItemContextMenu_AssetGroupRen(item);	break;
	default:																		break;
	}
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageAdd(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_PackageDel(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_PackageRen(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_PackageGroupAdd(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_PackageGroupDel(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_PackageGroupRen(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_AssetGroupAdd(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_AssetGroupDel(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_AssetGroupRen(const wxTreeListItem& _PackageItem)
{

}


/*
void WndAssetBrowser::OnPackageItemContextMenu_AddPackage(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_DelPackage(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_RenamePackage(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_AddPackageGroup(const wxTreeListItem& _PackageItem)
{
	// Find path to selected folder
	wxString zDirName, zFullPathDir, zPartialPathDir;
	GetPackagePath(_PackageItem, zPartialPathDir);

	// Find first unused New DirName
	int iNewDirId(0);
	do
	{
		zDirName		= wxString::Format("NewGroup%03i", iNewDirId++);
		zFullPathDir	= zPartialPathDir + zDirName;
	}
	while( wxDirExists(zFullPathDir) );

	// Add the new folder
	if( wxMkDir(zFullPathDir) >= 0 )
	{
		wxTreeListItem newItemID = mpTreePackage->AppendItem(_PackageItem, zDirName, IcoPkg_Folder, IcoPkg_FolderOpen);
	}
	else
	{
		wxMessageBox("Failed to create folder. (Can't find parent folder?)", "Confirm", wxOK, this);
	}
}

void WndAssetBrowser::OnPackageItemContextMenu_DelFolder(const wxTreeListItem& _PackageItem)
{
	
}

void WndAssetBrowser::OnPackageItemContextMenu_RenameFolder(const wxTreeListItem& _PackageItem)
{
	//mpTreePackage->itemre
}
*/