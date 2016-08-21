#include "ZendoPCH.h"

#include "../ZendoApp.h"
#include "WndAssetBrowser.h"
#include "WndAssetProperty.h"

enum ControlID
{
	ID_Asset_Rdio_ViewIcon		= wxID_HIGHEST,
	ID_Asset_Rdio_ViewList		= ID_Asset_Rdio_ViewIcon + WndAssetBrowser::keAssetView_List,
	ID_Asset_Rdio_ViewDetails	= ID_Asset_Rdio_ViewIcon + WndAssetBrowser::keAssetView_Details,
	ID_Asset_ListView,
	ID_Type_Tree,
	ID_Package_Tree,
	ID_Package_Flat,
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
	EVT_TOGGLEBUTTON(ID_Package_Flat,				WndAssetBrowser::OnPackageViewFlat)
	EVT_RADIOBUTTON(ID_Asset_Rdio_ViewIcon,			WndAssetBrowser::OnAssetViewChange)
	EVT_RADIOBUTTON(ID_Asset_Rdio_ViewList,			WndAssetBrowser::OnAssetViewChange)
	EVT_RADIOBUTTON(ID_Asset_Rdio_ViewDetails,		WndAssetBrowser::OnAssetViewChange)
	EVT_LIST_ITEM_ACTIVATED(ID_Asset_ListView,		WndAssetBrowser::OnAssetViewActivated)
	EVT_LIST_KEY_DOWN(ID_Asset_ListView,			WndAssetBrowser::OnAssetKeyDown)
	EVT_DATAVIEW_ITEM_BEGIN_DRAG(ID_Package_Tree, WndAssetBrowser::OnPackageDragBegin)
	EVT_DATAVIEW_ITEM_DROP_POSSIBLE(ID_Package_Tree, WndAssetBrowser::OnPackageDragPossible)
	EVT_DATAVIEW_ITEM_DROP(ID_Package_Tree, WndAssetBrowser::OnPackageDragDrop)
END_EVENT_TABLE()

wxTreeListItem GetTreeItemFromDataview( wxTreeListCtrl& _TreeControl, wxDataViewItem _DataViewItem )
{
	wxTreeListItem ItemCur = _TreeControl.GetNextItem(_TreeControl.GetRootItem());
	while (ItemCur.IsOk())
	{
		if (ItemCur.GetID() == _DataViewItem.GetID())
			return ItemCur;

		ItemCur = _TreeControl.GetNextItem(ItemCur);
	}
	return ItemCur;
}

class wxPackageClientData : public wxClientData
{
public:
	wxPackageClientData(const zenAss::zPackageGroupRef& _rGroup, const zenAss::zPackageRef& _rPackage)
	: mrGroup(_rGroup)
	, mrPackage(_rPackage)
	{
		if( mrPackage.IsValid() )	mHashID	= zHash64(mrPackage.GetParentGroup().GetNameHashID()).Append(mrPackage.GetName());
		else if(mrGroup.IsValid())	mHashID = mrGroup.GetNameHashID();
	}
	zenAss::zPackageGroupRef	mrGroup;	//!< Group associated with tree item (nil if element is a package)
	zenAss::zPackageRef			mrPackage;	//!< Package associated with tree item (nil if element is a PackageGroup)
	zHash64						mHashID;	//!< ID of group or package we're pointing to
};

class wxAssetTypeClientData : public wxClientData
{
public:
	wxAssetTypeClientData(zenConst::eAssetType _eAssetType):meAssetType(_eAssetType){}
	zenConst::eAssetType meAssetType;
};

void WndAssetBrowser::OnPackageDragPossible(wxDataViewEvent& event)
{	
	if (event.GetDataFormat() == wxDF_PRIVATE )
	{
		wxTreeListItem ItemDst = GetTreeItemFromDataview(*mpTreePackage, event.GetItem());
		if( mPackageDrag.IsOk() && ItemDst.IsOk() )
		{
			const wxPackageClientData* pClientDataSrc	= static_cast<const wxPackageClientData*>(mpTreePackage->GetItemData(mPackageDrag.GetID()));
			const wxPackageClientData* pClientDataDst	= static_cast<const wxPackageClientData*>(mpTreePackage->GetItemData(ItemDst.GetID()));
			zenAss::zPackageGroupRef rGroupDst			= pClientDataDst->mrGroup;
			zenAss::zPackageGroupRef rGroupSrc			= pClientDataSrc->mrGroup;
			if( rGroupDst.IsValid() ) 
			{
				if( (pClientDataSrc->mrPackage.IsValid())						|| 
					(rGroupSrc.IsValid() && !rGroupSrc.IsParentOf(rGroupDst))	) 				
					return;
			}
		}
	}	
	event.Veto();	
}

void WndAssetBrowser::OnPackageDragBegin(wxDataViewEvent& event)
{
	mPackageDrag = mpTreePackage->GetSelection();
	if( !mPackageDrag.IsOk() )
		event.Veto();

	wxDataViewItem item(event.GetItem());
	wxCustomDataObject *obj = new wxCustomDataObject(wxDF_PRIVATE);
	obj->SetData(sizeof(wxDataViewItem), &item);
	event.SetDataObject(obj);
	event.SetDataFormat(wxDF_PRIVATE);
	event.SetDragFlags(wxDrag_AllowMove); // allows both copy and move
}

void WndAssetBrowser::OnPackageDragDrop(wxDataViewEvent& event)
{
	if (event.GetDataFormat() == wxDF_PRIVATE)
	{
		wxTreeListItem ItemDst = GetTreeItemFromDataview(*mpTreePackage, event.GetItem());
		if (mPackageDrag.IsOk() && ItemDst.IsOk())
		{
			const wxPackageClientData* pClientDataSrc = static_cast<const wxPackageClientData*>(mpTreePackage->GetItemData(mPackageDrag.GetID()));
			const wxPackageClientData* pClientDataDst = static_cast<const wxPackageClientData*>(mpTreePackage->GetItemData(ItemDst.GetID()));
			zenAss::zPackageGroupRef rGroupDst	= pClientDataDst->mrGroup;
			zenAss::zPackageGroupRef rGroupSrc	= pClientDataSrc->mrGroup;
			zenAss::zPackageRef rPackageSrc		= pClientDataSrc->mrPackage;
			if (rGroupDst.IsValid())
			{
				if( rPackageSrc.IsValid() )
				{
					rPackageSrc.SetParentGroup(rGroupDst);
					UpdatePackageList();
					return;
				}
				else if( rGroupSrc.IsValid() && !rGroupSrc.IsParentOf(rGroupDst) && (rGroupSrc != rGroupDst) )
				{
					rGroupSrc.SetParentGroup(rGroupDst);
					UpdatePackageList();
					return;
				}					
			}
		}
	}
	event.Veto();
}

WndAssetBrowser::WndAssetBrowser(wxWindow *parent, const wxString& title)
: wxPanel(parent, wxID_ANY)
, mLayoutMgr(this, wxAUI_MGR_ALLOW_FLOATING|wxAUI_MGR_TRANSPARENT_HINT|wxAUI_MGR_HINT_FADE|wxAUI_MGR_VENETIAN_BLINDS_HINT)
, mpPanelType(nullptr)
, mpPanelPackage(nullptr)
, mpPanelAsset(nullptr)
, mpTreeTypes(nullptr)
, mpTreePackage(nullptr)
, mpTxtPackagesFilter(nullptr)
, mpLstAsset(nullptr)
, mpTxtAssetFilter(nullptr)
, mpIconAsset(nullptr)
, mpIconPackage(nullptr)
, mbPackageViewFlat(FALSE)
, meAssetView(keAssetView_Details)
{	
	mAssetTypeMask.Reset();	
	mdCountPerPackage.Init(128);
	mdCountPerPackage.SetDefaultValue(0);
	maCountPerType.SetCount(zenConst::keAssType__Count);

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
	panelInfo.CaptionVisible(false);
	panelInfo.MinSize(100,100);

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

zenAss::zAssetItemRef WndAssetBrowser::GetFirstSelectedItem( zenType::zFlagAssetType _TypeFilter )
{
	long idx = mpLstAsset->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	while(idx != -1)
	{
		zenAss::zAssetItemRef* pAssetItem = reinterpret_cast<zenAss::zAssetItemRef*>(mpLstAsset->GetItemData(idx));
		if( _TypeFilter.Any(pAssetItem->GetType()) )
			return *pAssetItem;
		idx = mpLstAsset->GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}
	return zenAss::zAssetItemRef();
}

void WndAssetBrowser::SelectItem(zenAss::zAssetItemRef _rItem)
{
	// Makes sure the wanted Asset Type is selected
	wxTreeListItem itemCurrent = mpTreeTypes->GetFirstItem();
	while( itemCurrent.IsOk() )
	{
		wxAssetTypeClientData* pAssetTypeData = static_cast<wxAssetTypeClientData*>( mpTreeTypes->GetItemData(itemCurrent) );
		if(  pAssetTypeData->meAssetType == _rItem.GetType() )
		{
			mpTreeTypes->CheckItem(itemCurrent);
			mAssetTypeMask |= zenConst::eAssetType(pAssetTypeData->meAssetType);
			break;
		}
		itemCurrent = mpTreeTypes->GetNextItem(itemCurrent);
	}

	// Makes sure the wanted Asset Package is selected	
	zInt iGroupCount						= 0;
	const zenAss::zPackageRef& rPackage		= _rItem.GetPackage();
	zenAss::zPackageGroupRef rPkgGroup		= rPackage.GetParentGroup();
	zenAss::zPackageGroupRef rPkgGroupRoot	= zenAss::zPackageGroupRef::sGetRoot();
	zenAss::zPackageGroupRef rGroupHierarchy[256];	
	while( rPkgGroup != rPkgGroupRoot && iGroupCount < ZENArrayCount(rGroupHierarchy) )
	{
		rGroupHierarchy[iGroupCount++]	= rPkgGroup;
		rPkgGroup						= rPkgGroup.GetParentGroup();
	}
	zenAssertMsg( iGroupCount < ZENArrayCount(rGroupHierarchy), "Too many group parenting for a package, increase array size");

	itemCurrent = mpTreePackage->GetRootItem();
	while( itemCurrent.IsOk() && iGroupCount-- >= 0 )
	{
		itemCurrent							= mpTreePackage->GetFirstChild(itemCurrent);
		rPkgGroup							= iGroupCount >= 0 ? rGroupHierarchy[iGroupCount] : nullptr;
		wxPackageClientData* pPkgTypeData	= static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(itemCurrent));
		while( itemCurrent.IsOk() && pPkgTypeData->mrGroup != rPkgGroup && pPkgTypeData->mrPackage != rPackage )
			itemCurrent = mpTreePackage->GetNextSibling(itemCurrent);
	}
	if( itemCurrent.IsOk() )
	{
		mpTreePackage->CheckItem(itemCurrent);
		mpTreePackage->Select(itemCurrent);
	}
	UpdatePackageSelected();
	UpdateAssetList();

	// Select item in listview
	long idx = mpLstAsset->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
	while( idx != -1 )
	{
		zenAss::zAssetItemRef* pAssetItem = reinterpret_cast<zenAss::zAssetItemRef*>(mpLstAsset->GetItemData(idx));		
		if( *pAssetItem == _rItem)	
			mpLstAsset->SetItemState(idx, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		idx = mpLstAsset->GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);			
	}
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
	for(zUInt idx(0); idx<zenConst::keAssType__Count; ++idx)
	{		
		const char* zTypeDesc		= zenAss::AssetTypeToString( (zenConst::eAssetType)idx );
		wxTreeListItem newItemID	= mpTreeTypes->AppendItem(rootItem, zTypeDesc, idx, idx); 	
		mpTreeTypes->SetItemData( newItemID, zenNewDefault wxAssetTypeClientData( static_cast<zenConst::eAssetType>(idx)) );
	}
			
	sizer->Add( mpTreeTypes, 100, wxGROW | wxALL, 2); 	
	mpPanelType->SetSizer(sizer);
	mpPanelType->SetSize(wxSize(300,200));
}

void WndAssetBrowser::CreateSectionPackage()
{
	zUInt uIconSize(16);
	mpPanelPackage		= zenNewDefault wxPanel(this);
	wxBoxSizer* sizer	= zenNewDefault wxBoxSizer(wxVERTICAL);

	mpIconPackage		= zenNewDefault wxImageList(uIconSize, uIconSize, true);
	mpIconPackage->Add(wxArtProvider::GetIcon(wxART_FOLDER,			wxART_LIST, wxSize(uIconSize,uIconSize)));
	mpIconPackage->Add(wxArtProvider::GetIcon(wxART_FOLDER_OPEN,	wxART_LIST, wxSize(uIconSize,uIconSize)));	
	mpIconPackage->Add(wxArtProvider::GetIcon(wxART_COPY,			wxART_LIST, wxSize(uIconSize,uIconSize)));
	//mpIconPackage->Add(wxArtProvider::GetIcon(wxART_FILE_OPEN,		wxART_LIST, wxSize(uIconSize,uIconSize)));

	// TreeView of packages -------------------------------------------------------------------
	mpTreePackage = zenNewDefault wxTreeListCtrl(mpPanelPackage, ID_Package_Tree,	wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE | wxTL_CHECKBOX | wxTL_3STATE );
	mpTreePackage->AppendColumn("Packages", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	mpTreePackage->AppendColumn("#", wxCOL_WIDTH_AUTOSIZE, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	mpTreePackage->SetImageList(mpIconPackage); //@todo Urgent Issue with this on 3.10 //SF
	mpTreePackage->GetDataView()->SetIndent(16);

	mpTreePackage->GetDataView()->SetId(ID_Package_Tree) ;
	mpTreePackage->GetDataView()->EnableDragSource(wxDF_PRIVATE);
	mpTreePackage->GetDataView()->EnableDropTarget(wxDF_PRIVATE);
	
	UpdatePackageList();

	// Insert it into a sizer -----------------------------------------------------------------
	wxSizer* sizerPkgFilter = zenNewDefault wxBoxSizer(wxHORIZONTAL);
	
	wxToggleButton btnTest(mpPanelPackage, 0, wxT("Flat"));
	wxSize size					= btnTest.GetTextExtent(btnTest.GetLabel())*1.75f;
	wxToggleButton* pBtnPkgFlat	= zenNewDefault wxToggleButton(mpPanelPackage, ID_Package_Flat, btnTest.GetLabel(), wxDefaultPosition, size);
	pBtnPkgFlat->SetValue(mbPackageViewFlat);
	mpTxtPackagesFilter = zenNewDefault wxSearchCtrl(mpPanelPackage, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

	sizerPkgFilter->Add(pBtnPkgFlat, 0, wxALL | wxALIGN_CENTRE_VERTICAL, 0);
	sizerPkgFilter->Add(mpTxtPackagesFilter, 100,	wxGROW | wxALL | wxALIGN_CENTRE_VERTICAL, 0);
	
	sizer->Add(sizerPkgFilter,													0,		wxGROW | wxALL, 2);
	sizer->Add(mpTreePackage,													100,	wxGROW | wxALL, 2);
	mpPanelPackage->SetSizer(sizer);
	mpPanelPackage->SetSize(wxSize(300,200));
	//-----------------------------------------------------------------------------------------
}

void WndAssetBrowser::CreateSectionAsset()
{	
	mpPanelAsset					= zenNewDefault wxPanel(this);
	wxBoxSizer* sizer				= zenNewDefault wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizerRightOptions	= zenNewDefault wxBoxSizer(wxHORIZONTAL);

	wxRadioButton* pRdioAssetView[3];
 	pRdioAssetView[0]				= zenNewDefault wxRadioButton(mpPanelAsset, ID_Asset_Rdio_ViewIcon,  wxT("Icon"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
 	pRdioAssetView[1]				= zenNewDefault wxRadioButton(mpPanelAsset, ID_Asset_Rdio_ViewList,  wxT("List"), wxDefaultPosition, wxDefaultSize, 0);
 	pRdioAssetView[2]				= zenNewDefault wxRadioButton(mpPanelAsset, ID_Asset_Rdio_ViewDetails,  wxT("Detail"), wxDefaultPosition, wxDefaultSize, 0);
	pRdioAssetView[meAssetView]->SetValue(true);

	mpTxtAssetFilter				= zenNewDefault wxSearchCtrl(mpPanelAsset, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	//! @todo clean removed alignement, causes assert
	sizerRightOptions->Add(mpTxtAssetFilter,	100,	wxGROW | wxALL /*| wxALIGN_CENTRE_VERTICAL*/, 0);
	sizerRightOptions->AddSpacer(4);
	sizerRightOptions->Add(pRdioAssetView[0],	0,		wxGROW | wxALL /*| wxALIGN_CENTRE_VERTICAL*/, 0);
	sizerRightOptions->Add(pRdioAssetView[1],	0,		wxGROW | wxALL /*| wxALIGN_CENTRE_VERTICAL*/, 0);
	sizerRightOptions->Add(pRdioAssetView[2],	0,		wxGROW | wxALL /*| wxALIGN_CENTRE_VERTICAL*/, 0);
	sizer->Add(sizerRightOptions,				0,		wxGROW | wxALL, 2);

	mpPanelAsset->SetSizer(sizer);	
	mpPanelAsset->SetSize(wxSize(300,200));
	RecreateSectionAsset();

}

void WndAssetBrowser::RecreateSectionAsset()
{
	if( mpLstAsset )
	{
		ClearAssetList();
		mpPanelAsset->GetSizer()->Detach(mpLstAsset);
		zenDelnullptr(mpLstAsset);
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

void WndAssetBrowser::ClearAssetList()
{
	for(zInt idx(0), count(mpLstAsset->GetItemCount()); idx<count; ++idx)
		delete reinterpret_cast<zenAss::zAssetItemRef*>(mpLstAsset->GetItemData(idx));
	mpLstAsset->ClearAll();	
}

void WndAssetBrowser::UpdateAssetList()
{
	static zenMem::zAllocatorPool sPoolAsset("Asset Pool", sizeof(zenAss::zAssetItemRef), 256, 256 );
	
	// Free memory allocated, and clear list of asset
	mpLstAsset->Freeze();
	ClearAssetList();

	// Recreate Icon Image List	
	const wxImageList* pTypeIconList = wxGetApp().maIcon[ZendoApp::keIco_Asset128];
	int sizeX, sizeY;
	pTypeIconList->GetSize(0, sizeX, sizeY);
	if( mpIconAsset )	
		mpIconAsset->RemoveAll();
	else				
		mpIconAsset = zenNewDefault wxImageList(sizeX, sizeY, true);
	for(zUInt idx(0), count(pTypeIconList->GetImageCount()); idx<count; ++idx)
		mpIconAsset->Add( wxArtProvider::GetIcon(wxART_INFORMATION,wxART_LIST, wxSize(sizeX,sizeY)));

	// If only 1 asset type is selected, can displayed detailed column
	mpLstAsset->InsertColumn(0, wxT("Name"), wxLIST_FORMAT_LEFT);
	if( mAssetTypeMask.CountActive() == 1 )
	{
		//AAss::Asset::GetTypeDesc()
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
	for(zUInt idxPack(0), countPack(maPackageSelected.Count()); idxPack<countPack; ++idxPack)
	{
		zenAss::zPackageRef rPackage				= maPackageSelected[idxPack];
		zenAss::zPackageGroupRef rGroup				= rPackage.GetParentGroup();
		const zenAss::zPackageGroupRef& rGroupRoot	= zenAss::zPackageGroupRef::sGetRoot();
		if( rPackage.IsValid() && rGroup.IsValid() )
		{
			zHash64 HashPackage						= zHash64(rGroup.GetNameHashID()).Append(rPackage.GetName());
			for(zUInt typIdx(0); typIdx<zenConst::keAssType__Count; ++typIdx)
			{
				zUInt					uPackageItemCount(0);
				zenConst::eAssetType	eTypeIdx = (zenConst::eAssetType)typIdx;
				if( mAssetTypeMask.Any( eTypeIdx ) )
				{
					zenAss::zArrayAsset::Iterator it( rPackage.GetAssets(eTypeIdx) );
					while( it.IsValid() )
					{
						zenAss::zAssetItemRef* prItem	= zenNew(&sPoolAsset) zenAss::zAssetItemRef(it.GetValue());
						zUInt uIconIndex			= prItem->GetType();
						if( IsViewIcon() )
						{
							const wxIcon* pIcon	= wxGetApp().GetIcon(*prItem);
							if(  pIcon )
							{
								uIconIndex = mpIconAsset->GetImageCount();
								mpIconAsset->Add( *pIcon );						
							}
						}					

						// Add the asset to list
						long itemID = mpLstAsset->InsertItem(mpLstAsset->GetItemCount(), (const char*)prItem->GetName(), uIconIndex);
						mpLstAsset->SetItemPtrData(itemID, reinterpret_cast<wxUIntPtr>(prItem));
						if( IsViewDetail() )
							mpLstAsset->SetItem(itemID, 1, (const char*)prItem->GetDescription());
					
						++uPackageItemCount;
						++maCountPerType[prItem->GetType()];
						++it;
					}
				}	

				// Update the count of asset per package and package group												
				mdCountPerPackage.GetAdd( HashPackage )	+= uPackageItemCount;
				while( rGroup != rGroupRoot )
				{
					mdCountPerPackage.GetAdd( rGroup.GetNameHashID() )	+= uPackageItemCount;
					rGroup												= rGroup.GetParentGroup();
				}
			}	
		}
	}
	UpdateAssetCount();
	mpLstAsset->Thaw();
	mpLstAsset->Refresh();
}

//=================================================================================================
//! @brief		Update the listed Packages
//! @details	Display all packages meeting filter requirement
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::UpdatePackageList()
{
	// Save all item selected in package list	
	zMap<bool>::Key64 dSelected( 256 );	
	wxTreeListItem treeID = mpTreePackage->GetNextItem(mpTreePackage->GetRootItem());	
	while( treeID.IsOk() )
	{
		if (mpTreePackage->GetCheckedState(treeID) == wxCHK_CHECKED)
		{
			const wxPackageClientData* pClient = static_cast<const wxPackageClientData*>(mpTreePackage->GetItemData(treeID));
			dSelected.Set(pClient->mHashID, true);
		}
		treeID = mpTreePackage->GetNextItem(treeID);
	}		
	
	// Repopulate the list
	mpTreePackage->DeleteAllItems();

	UpdatePackageList( mpTreePackage->GetRootItem(), zenAss::zPackageGroupRef::sGetRoot(), dSelected );
	UpdateChildPackageChecked(mpTreePackage->GetRootItem());
	UpdateAssetList();
}

void WndAssetBrowser::UpdatePackageList(wxTreeListItem _treeParentID, const zenAss::zPackageGroupRef& _rGroupToAdd, const zMap<bool>::Key64& _dSelectedItems)
{
	wxTreeListItem treeIDGroup(_treeParentID);
	if (!mbPackageViewFlat)
	{
		wxPackageClientData* pClientData	= zenNewDefault wxPackageClientData(_rGroupToAdd, nullptr);
		treeIDGroup							= mpTreePackage->AppendItem(_treeParentID, _rGroupToAdd.GetName(), IcoPkg_Folder, IcoPkg_FolderOpen);
		mpTreePackage->SetItemData(treeIDGroup, pClientData);
		mpTreePackage->CheckItem(treeIDGroup, _dSelectedItems.Exist(pClientData->mHashID) ? wxCHK_CHECKED : wxCHK_UNCHECKED);
	}
	
	for (zUInt idx(0), count(_rGroupToAdd.GetChildren().Count()); idx < count; ++idx)
		UpdatePackageList(treeIDGroup, _rGroupToAdd.GetChildren()[idx], _dSelectedItems);

	for (zUInt idx(0), count(_rGroupToAdd.GetPackages().Count()); idx < count; ++idx)
	{
		const zenAss::zPackageRef& rPackage			= _rGroupToAdd.GetPackages()[idx];
		wxPackageClientData* pClientData			= zenNewDefault wxPackageClientData(nullptr, rPackage);		
		wxTreeListItem treeIDPackage				= mpTreePackage->AppendItem(_treeParentID, rPackage.GetName(), IcoPkg_Package, IcoPkg_Package);
		mpTreePackage->SetItemData(treeIDPackage, pClientData );
		mpTreePackage->CheckItem(treeIDPackage, _dSelectedItems.Exist(pClientData->mHashID) ? wxCHK_CHECKED : wxCHK_UNCHECKED );
	}
}

//=================================================================================================
//! @brief		Recursively update all child checked state based on parent
//! @details	Used by OnPackageItemChecked event
//-------------------------------------------------------------------------------------------------
//! @param		_TreeId - Current Tree item being processed
//! @return		- 
//=================================================================================================
void WndAssetBrowser::UpdateChildPackageChecked(wxTreeListItem _ParentTreeID)
{
	bool bChecked			= mpTreePackage->GetCheckedState(_ParentTreeID) != wxCHK_UNCHECKED;
	wxTreeListItem treeID	= mpTreePackage->GetFirstChild(_ParentTreeID);
	while( treeID.IsOk() )
	{
		if ( bChecked && mpTreePackage->GetCheckedState(treeID) == wxCHK_UNCHECKED )
			mpTreePackage->CheckItem(treeID, wxCHK_UNDETERMINED );
		else if	( !bChecked && mpTreePackage->GetCheckedState(treeID) == wxCHK_UNDETERMINED )
			mpTreePackage->CheckItem(treeID, wxCHK_UNCHECKED );
		
		UpdateChildPackageChecked(treeID);
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
	wxTreeListItem itemCurrent	= mpTreePackage ? mpTreePackage->GetFirstItem() : nullptr;
	while( itemCurrent.IsOk() )
	{
		const wxPackageClientData* pClient = static_cast<const wxPackageClientData*>(mpTreePackage->GetItemData(itemCurrent));
		mpTreePackage->SetItemText(itemCurrent, 1, wxString::Format("%i", mdCountPerPackage[pClient->mHashID]) );
		itemCurrent = mpTreePackage->GetNextItem(itemCurrent);
	}

	// Update per Type asset count ------------------------------------------------------------
	zUInt idx(0);
	itemCurrent = mpTreeTypes ? mpTreeTypes->GetFirstItem() : nullptr;
	while( itemCurrent.IsOk() )
	{
		mpTreeTypes->SetItemText(itemCurrent, 1, wxString::Format("%i", maCountPerType[idx++] ) );
		itemCurrent = mpTreeTypes->GetNextItem(itemCurrent);
	}
	//-----------------------------------------------------------------------------------------
}

//=================================================================================================
//! @brief		Called when user activate/deactivate flat view for packages
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::OnPackageViewFlat(wxCommandEvent& event)
{
	mbPackageViewFlat = event.IsChecked();
	UpdatePackageList();
}

//=================================================================================================
//! @brief		Called when user changes the current desired asset view
//! @details	Reconstruct the view with new view option
//-------------------------------------------------------------------------------------------------
//! @return		- 
//=================================================================================================
void WndAssetBrowser::OnAssetViewChange(wxCommandEvent& event)
{	
	meAssetView = static_cast<enumAssetView>(event.GetId() - ID_Asset_Rdio_ViewIcon);
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
	mAssetTypeMask.Reset();
	wxTreeListItem itemCurrent = mpTreeTypes->GetFirstItem();
	while( itemCurrent.IsOk() )
	{				
		if( mpTreeTypes->GetCheckedState(itemCurrent) == wxCHK_CHECKED )	
		{
			wxAssetTypeClientData* pAssetTypeData	= static_cast<wxAssetTypeClientData*>( mpTreeTypes->GetItemData(itemCurrent) );
			mAssetTypeMask							|= zenConst::eAssetType(pAssetTypeData->meAssetType);
		}
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
		UpdateChildPackageChecked(event.GetItem());
		UpdatePackageSelected();
		UpdateAssetList();
	}
}

void WndAssetBrowser::UpdatePackageSelected()
{
	// Update the list of selected packages
	zArrayStatic<zenAss::zPackageRef> aPackageSelected(zenAss::zPackageRef::sGetPackages().Count());
	wxTreeListItem itemCurrent = mpTreePackage->GetFirstItem();
	zUInt uSelectedCount(0);
	while (itemCurrent.IsOk())
	{
		wxPackageClientData* pClientData = static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(itemCurrent));
		if (pClientData->mrPackage.IsValid() && mpTreePackage->GetCheckedState(itemCurrent) != wxCHK_UNCHECKED)
			aPackageSelected[uSelectedCount++] = pClientData->mrPackage;
		itemCurrent = mpTreePackage->GetNextItem(itemCurrent);
	}

	maPackageSelected.Copy(aPackageSelected.First(), uSelectedCount);
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
	zenAss::zAssetItemRef* prItem = reinterpret_cast<zenAss::zAssetItemRef*>(event.GetItem().GetData());
	if( prItem )
		wxGetApp().mpFrame->GetWndAssetProperty()->AddAssetTab( *prItem );
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
		ID_Menu_SaveAll,
		ID_Menu_PackageAdd,
		ID_Menu_PackageDel,
		ID_Menu_PackageRen,
		ID_Menu_PackageMove,		
		ID_Menu_PackageGroupAdd,
		ID_Menu_PackageGroupDel,
		ID_Menu_PackageGroupRen,
		ID_Menu_PackageNewAssetFirst,
		ID_Menu_PackageNewAssetLast = ID_Menu_PackageNewAssetFirst + zenConst::keAssType__Count,
		ID_Menu_AssetGroupAdd,
		ID_Menu_AssetGroupDel,
		ID_Menu_AssetGroupRen,
		ID_Menu_AssetAdd,
		ID_Menu_AssetDel,
	};
	
	wxMenu menu;		
	const wxTreeListItem item			= event.GetItem();
	wxPackageClientData* pClientData	= static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(item));
	
	menu.Append(ID_Menu_SaveAll, "&Save");
	// Element is a package
	if( pClientData && pClientData->mrPackage.IsValid() )
	{								
		menu.AppendSeparator();
		menu.Append(ID_Menu_PackageRen,			"&Rename package");
		menu.Append(ID_Menu_PackageMove,		"&Move package");
		menu.Append(ID_Menu_PackageDel,			"&Delete package");
		
		wxMenu* pSubmenuAsset = zenNewDefault wxMenu;	
		for(zUInt idx(0); idx<zenConst::keAssType__Count; ++idx)
			pSubmenuAsset->Append(ID_Menu_PackageNewAssetFirst+idx, zenAss::AssetTypeToString( (zenConst::eAssetType)idx ) );

		menu.AppendSeparator();		
		menu.AppendSubMenu( pSubmenuAsset, "&New Asset");
	}
	// Element is a folder or empty
	else
	{
		if( pClientData && pClientData->mrGroup.IsValid() && (pClientData->mrGroup != zenAss::zPackageGroupRef::sGetRoot()) )
		{
			//wxTreeListItems selection;
			//mpTreePackage->GetSelections(selection);
			//menu.AppendSeparator();
			//if (selection.size() == 1)
			{
				menu.Append(ID_Menu_PackageGroupRen, "&Rename group");
				menu.Append(ID_Menu_PackageGroupDel, "&Delete group");
				menu.AppendSeparator();
			}
		}
		menu.Append(ID_Menu_PackageGroupAdd, "New &Group");
		menu.Append(ID_Menu_PackageAdd, "New &Package");
	}

	int IDSelected = mpTreePackage->GetPopupMenuSelectionFromUser(menu);
	switch (IDSelected)
	{
	case ID_Menu_SaveAll:			OnPackageItemContextMenu_SaveAll(item);			break;
	case ID_Menu_PackageAdd:		OnPackageItemContextMenu_PackageAdd(item);		break;
	case ID_Menu_PackageDel:		OnPackageItemContextMenu_PackageDel(item);		break;
	case ID_Menu_PackageRen:		OnPackageItemContextMenu_PackageRen(item);		break;
	case ID_Menu_PackageMove:		OnPackageItemContextMenu_PackageMove(item);		break;
	case ID_Menu_PackageGroupAdd:	OnPackageItemContextMenu_PackageGroupAdd(item);	break;
	case ID_Menu_PackageGroupDel:	OnPackageItemContextMenu_PackageGroupDel(item);	break;
	case ID_Menu_PackageGroupRen:	OnPackageItemContextMenu_PackageGroupRen(item);	break;
	case ID_Menu_AssetGroupAdd:		OnPackageItemContextMenu_AssetGroupAdd(item);	break;
	case ID_Menu_AssetGroupDel:		OnPackageItemContextMenu_AssetGroupDel(item);	break;
	case ID_Menu_AssetGroupRen:		OnPackageItemContextMenu_AssetGroupRen(item);	break;
	default:						if (IDSelected >= ID_Menu_PackageNewAssetFirst && IDSelected <= ID_Menu_PackageNewAssetLast)
										OnPackageItemContextMenu_PackageNewAsset((zenConst::eAssetType)(IDSelected - ID_Menu_PackageNewAssetFirst), item);
									break;
	}
	
}

void WndAssetBrowser::OnPackageItemContextMenu_SaveAll(const wxTreeListItem& _PackageItem)
{
	zenAss::SaveAll();
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageAdd(const wxTreeListItem& _PackageItem)
{
	//zenAss::zPackageRef::sCreate()
	wxPackageClientData* pClientData = static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(_PackageItem));
	wxString zTitle		= wxString::Format("Creating a new Package");
	wxString zMessage	= wxString::Format("Enter name");
	wxTextEntryDialog dialog(this, zMessage, zTitle, "New Package", wxTextEntryDialogStyle, wxDefaultPosition);
	if (dialog.ShowModal() == wxID_OK)
	{
		//! @todo Asset validate double names
		zenAss::zPackageGroupRef rParent = pClientData && pClientData->mrGroup.IsValid() ? pClientData->mrGroup : zenAss::zPackageGroupRef::sGetRoot();
		zenAss::zPackageRef::sCreate(dialog.GetValue().c_str().AsWChar(), rParent);
		UpdatePackageList();
	}
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageDel(const wxTreeListItem& _PackageItem)
{
	wxPackageClientData* pClientData = static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(_PackageItem));
	if (pClientData && pClientData->mrPackage.IsValid())
	{
		zUInt uAssetCount(0);
		for (zUInt uType(0); uType < zenConst::keAssType__Count; ++uType)
			uAssetCount += pClientData->mrPackage.GetAssets(static_cast<zenConst::eAssetType>(uType)).Count();
		wxString zTitle = wxString::Format("Deleting Package [%s]", pClientData->mrPackage.GetName().c_str());
		wxString zMessage = wxString::Format("Package [%s] contains %i asset(s).\nAre you certain you want to delete it?", pClientData->mrPackage.GetName().c_str(), uAssetCount);		
		wxMessageDialog dialog(this, zMessage, zTitle, wxOK | wxCANCEL | wxCANCEL_DEFAULT | wxCENTRE | wxICON_EXCLAMATION);
		if (dialog.ShowModal() == wxID_OK)
		{
			pClientData->mrPackage.Delete();
			UpdatePackageList();
		}
	}
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageRen(const wxTreeListItem& _PackageItem)
{
	wxPackageClientData* pClientData = static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(_PackageItem));
	if (pClientData && pClientData->mrPackage.IsValid())
	{
		wxString zCaption = wxString::Format("Renaming package [%s]", pClientData->mrPackage.GetName().c_str() );
		wxTextEntryDialog dialog (this, "", zCaption, pClientData->mrPackage.GetName().c_str(), wxTextEntryDialogStyle, wxDefaultPosition );
		if( dialog.ShowModal() == wxID_OK )
		{
			//! @todo Asset validate double names
			pClientData->mrPackage.SetName( dialog.GetValue().c_str().AsWChar() );
			mpTreePackage->SetItemText( _PackageItem, pClientData->mrPackage.GetName() );
		}
	}
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageMove(const wxTreeListItem& _PackageItem)
{
	/*
	const zenAss::zArrayPackage&	dAllPackages = zenAss::GetPackages();
	zenAss::zArrayPackage::Iterator it(dAllPackages);
	zArrayStatic<zenAss::zPackageRef>	aPackageSelected(zenAss::GetPackages().Count());
	zUInt							uSelectedCount(0);
	mpTreePackage->DeleteAllItems();
	while (it.IsValid())
	{
		zenAss::zPackageRef rPackage = it.GetValue();
		const zArrayStatic<zString> aGroupName = rPackage.GetGroupAndName();
		wxString zGroups = "";

		// Look for folder under which package should be added (and create it if needed)
		wxTreeListItem treeParentID = mpTreePackage->GetRootItem();
		for (zUInt idx(0), count(aGroupName.Count() - 1); idx < count && !mbPackageViewFlat; ++idx)
		{
			const zString& zGroupName = aGroupName[idx];
			wxTreeListItem treeID = mpTreePackage->GetFirstChild(treeParentID);
			zGroups += static_cast<const char*>(zGroupName);
			zGroups += wxT("\\");
			while (treeID.IsOk() && strcmp(zGroupName, mpTreePackage->GetItemText(treeID)) != 0)
			{
				treeID = mpTreePackage->GetNextSibling(treeID);
			}

			if (!treeID.IsOk())
			{
				treeID = mpTreePackage->AppendItem(treeParentID, (const char*)zGroupName, IcoPkg_Folder, IcoPkg_FolderOpen);
				mpTreePackage->SetItemData(treeID, zenNewDefault wxPackageClientData(zenAss::zPackageRef(), zGroups));
			}
			treeParentID = treeID;
		}

		// Add the package if we've found the tree id
		wxTreeListItem newItemID = mpTreePackage->AppendItem(treeParentID, (const char*)*aGroupName.Last(), IcoPkg_Package, IcoPkg_Package);

		wxTextDataObject my_data("This text will be dragged.");
		zGroups += static_cast<const char*>(*aGroupName.Last());
		zGroups += wxT("\\");
		mpTreePackage->SetItemData(newItemID, zenNewDefault wxPackageClientData(rPackage, zGroups));

		// Reselect package if previously selected
		for (zUInt idxPack(0), countPack(maPackageSelected.Count()); idxPack < countPack; ++idxPack)
		{
			if (rPackage == maPackageSelected[idxPack])
			{
				mpTreePackage->CheckItem(newItemID);
				aPackageSelected[uSelectedCount++] = rPackage;
			}
		}

		++it;
	}

	maPackageSelected.Copy(aPackageSelected.First(), uSelectedCount);
	UpdateAssetList();
	*/
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageGroupAdd(const wxTreeListItem& _PackageItem)
{
	wxPackageClientData* pClientData	= static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(_PackageItem));
	wxString zTitle						= wxString::Format("Creating a new Package Group");
	wxString zMessage					= wxString::Format("Enter name");
	wxTextEntryDialog dialog(this, zMessage, zTitle, "New Group", wxTextEntryDialogStyle, wxDefaultPosition);
	if (dialog.ShowModal() == wxID_OK)
	{
		//! @todo Asset validate double names
		if (pClientData && pClientData->mrGroup.IsValid())
			zenAss::zPackageGroupRef::sCreate(dialog.GetValue().c_str().AsWChar(), pClientData->mrGroup);
		else
			zenAss::zPackageGroupRef::sCreate(dialog.GetValue().c_str().AsWChar());
		UpdatePackageList();
	}
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageGroupDel(const wxTreeListItem& _PackageItem)
{
	wxPackageClientData* pClientData = static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(_PackageItem));
	if (pClientData && pClientData->mrGroup.IsValid())
	{
		wxString zTitle		= wxString::Format("Deleting Group [%s]", pClientData->mrGroup.GetName().c_str() );
		wxString zMessage	= wxString::Format("Group [%s] contains %i package(s).\nAre you certain you want to delete it?", pClientData->mrGroup.GetName().c_str(), pClientData->mrGroup.GetPackageCount() );
		wxMessageDialog dialog(this, zMessage, zTitle, wxOK|wxCANCEL|wxCANCEL_DEFAULT|wxCENTRE|wxICON_EXCLAMATION);
		if (dialog.ShowModal() == wxID_OK)
		{
			pClientData->mrGroup.Delete();
			UpdatePackageList();
		}
	}	
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageGroupRen(const wxTreeListItem& _PackageItem)
{
	wxPackageClientData* pClientData = static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(_PackageItem));
	if (pClientData && pClientData->mrGroup.IsValid())
	{
		wxString zTitle		= wxString::Format("Renaming Group [%s]", pClientData->mrGroup.GetName().c_str() );
		wxString zMessage	= wxString::Format("Enter new name");
		wxTextEntryDialog dialog(this, zMessage, zTitle, pClientData->mrGroup.GetName().c_str(), wxTextEntryDialogStyle, wxDefaultPosition);
		if (dialog.ShowModal() == wxID_OK)
		{
			//! @todo Asset validate double names
			pClientData->mrGroup.SetName(dialog.GetValue().c_str().AsWChar());
			mpTreePackage->SetItemText(_PackageItem, pClientData->mrGroup.GetName());
		}
	}
}

void WndAssetBrowser::OnPackageItemContextMenu_PackageNewAsset(zenConst::eAssetType _eType, const wxTreeListItem& _PackageItem)
{
	wxPackageClientData* pClientData = static_cast<wxPackageClientData*>(mpTreePackage->GetItemData(_PackageItem));
	if( pClientData )
	{	
		zenAss::zAssetItemRef rNewAsset = zenAss::AssetCreate(_eType, pClientData->mrPackage);
		UpdateAssetList();
	}
}

void WndAssetBrowser::OnPackageItemContextMenu_AssetGroupAdd(const wxTreeListItem& _PackageItem)
{

//	zenAss::zPackageGroupRef rGroup = zenAss::zPackageGroupRef::sCreate();
	
}

void WndAssetBrowser::OnPackageItemContextMenu_AssetGroupDel(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnPackageItemContextMenu_AssetGroupRen(const wxTreeListItem& _PackageItem)
{

}

void WndAssetBrowser::OnAssetKeyDown(wxListEvent& event)
{
	int key = event.GetKeyCode();
	if( key == WXK_DELETE && mpLstAsset->GetSelectedItemCount() > 0 )
	{
		wxString zMessage = wxString::Format("This will delete %i Asset(s), are you certain?", mpLstAsset->GetSelectedItemCount() );
		wxMessageDialog dialConfirm(this, zMessage, wxString("WARNING: Asset Deletion"), wxOK|wxCANCEL|wxCANCEL_DEFAULT|wxICON_WARNING|wxCENTRE);
		if( dialConfirm.ShowModal() == wxID_OK )
		{
			long idx = mpLstAsset->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			while( idx != -1 )
			{			
				zenAss::zAssetItemRef* pAssetItem = reinterpret_cast<zenAss::zAssetItemRef*>(mpLstAsset->GetItemData(idx));
				pAssetItem->Delete();
				zenDel( pAssetItem );
				mpLstAsset->DeleteItem(idx);
				idx = mpLstAsset->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			}
		}		
	}	
}
