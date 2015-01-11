#include "ZendoPCH.h"
#include "../ZendoApp.h"
#include "../Window/WndAssetProperty.h"
#include "CtrlAssetPropertyTab.h"
#include "CtrlProperty.h"

namespace BCtrl
{
	
//=================================================================================================
// EVENTS TABLE
//=================================================================================================
enum CONTROLID
{
	CtrlId_Toolbar_Default = wxID_HIGHEST,
	CtrlId_Toolbar_Original,
	CtrlId_Toolbar_Apply,
};

BEGIN_EVENT_TABLE(TabAssetProperty, wxPanel)
	EVT_TOOL(CtrlId_Toolbar_Default,	TabAssetProperty::OnToolbarDefault)
	EVT_TOOL(CtrlId_Toolbar_Original,	TabAssetProperty::OnToolbarOriginal)
	EVT_TOOL(CtrlId_Toolbar_Apply,	TabAssetProperty::OnToolbarApply)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(TabAssetPropertyGridPage, wxPropertyGridPage)
	EVT_PG_CHANGED( wxID_ANY, TabAssetPropertyGridPage::OnPropertyChange )
END_EVENT_TABLE()


//=================================================================================================
// GRID PROPERTY PAGE 
//=================================================================================================

//=================================================================================================
//! @brief		Check if items should be reflag as untouched or modified
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		
//=================================================================================================
void TabAssetPropertyGridPage::OnPropertyChange( wxPropertyGridEvent& event )
{
	RefreshModifiedItems();
}

//=================================================================================================
//! @brief		Reconfigure all item to untouched or modified, based on 
//!				their current value vs default
//! @details	Since an item remain 'modified' forever, we have to clear the whole GridPage
//!				status, when an object is reset to default value, and re configure each item
//!				for modified. A 'modified' item appear bold.
//-------------------------------------------------------------------------------------------------
//! @return		
//=================================================================================================
void TabAssetPropertyGridPage::RefreshModifiedItems()
{
	wxPropertyGridIterator itProperty = GetIterator(wxPG_ITERATE_PROPERTIES);	
	while( !itProperty.AtEnd() )
	{
		wxPGProperty* pProperty		= *itProperty;
		PropertyMetaData* pMetaData = static_cast<PropertyMetaData*>( (*itProperty)->GetClientData() );
		if( pMetaData )
			pMetaData->UpdateControlState();	
		itProperty++;
	}
}

//=================================================================================================
// ASSET PROPERTIES TABS
//=================================================================================================
TabAssetProperty::TabAssetProperty(wxWindow *_pParent, const zenAss::zAssetItemRef& _rEditAsset)
: wxPanel(_pParent, wxID_ANY)
, mrAsset(_rEditAsset)
{
	ZENAssert(mrAsset.IsValid());
	wxBoxSizer* sizer = zenNewDefault wxBoxSizer(wxVERTICAL);	
	const int margin(3);
	const long flags(wxGROW | wxALL);

	
	long style =	wxPG_BOLD_MODIFIED |
					wxPG_SPLITTER_AUTO_CENTER |
					//wxPG_AUTO_SORT |
					//wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
					//wxPG_TOOLTIPS |
					//wxPG_HIDE_CATEGORIES |
					//wxPG_LIMITED_EDITING |
					//wxPG_NO_INTERNAL_BORDER |
					wxPG_TOOLBAR |
					wxPG_DESCRIPTION;

	long extraStyle =	wxPG_EX_MODE_BUTTONS |
						wxPG_EX_MULTIPLE_SELECTION;
					//	wxPG_EX_HIDE_PAGE_BUTTONS;
					//| wxPG_EX_AUTO_UNSPECIFIED_VALUES
					//| wxPG_EX_GREY_LABEL_WHEN_DISABLED
					//| wxPG_EX_NATIVE_DOUBLE_BUFFERING
					//| wxPG_EX_HELP_AS_TOOLTIPS

	mpPropertyGrid = zenNewDefault wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );
	mpPropertyGrid->SetExtraStyle(extraStyle);
	mpPropertyGrid->GetGrid()->SetSelectionBackgroundColour(wxColour(60,235,60,255)/*wxColour(255,255,255,255)*/);
	mpPropertyGrid->GetGrid()->SetSelectionTextColour(wxColour(0,0,0,255));
 	//mpPropertyGrid->GetGrid()->SetLineColour(bgColors[0]/*wxColour(255,255,255,255)*/);

	// Add every property of this asset
	TabAssetPropertyGridPage* pPage = zenNewDefault TabAssetPropertyGridPage();
	for(zUInt propIdx(0), propCount(mrAsset.GetValueCount()); propIdx<propCount; ++propIdx)
		CreateAssetValueControl(*pPage, mrAsset, mrAsset.GetValue(propIdx));
		
	mpPropertyGrid->AddPage("All", wxNullBitmap, pPage);
	pPage->RefreshModifiedItems();

	// Add top PropertyEditor toolbar
	wxToolBar* pToolbar = mpPropertyGrid->GetToolBar();
 	if( pToolbar )
	{
		pToolbar->AddSeparator();
		pToolbar->AddTool(CtrlId_Toolbar_Default, wxT(""), wxArtProvider::GetBitmap(wxART_GOTO_FIRST),	wxT("Reset default values of selected items"));
		pToolbar->AddTool(CtrlId_Toolbar_Original, wxT(""), wxArtProvider::GetBitmap(wxART_GO_BACK),	wxT("Restore saved values of selected items"));
		pToolbar->AddTool(CtrlId_Toolbar_Apply, wxT(""), wxArtProvider::GetBitmap(wxART_TICK_MARK),		wxT("Apply settings"));
		pToolbar->Realize();
	}

	sizer->Add(mpPropertyGrid, 100, flags, margin);
	SetSizer( sizer );
}


TabAssetProperty::~TabAssetProperty()
{
	wxGetApp().mpFrame->GetWndAssetProperty()->AssetTabRemoved(*this);
}

const zenAss::zAssetItemRef& TabAssetProperty::GetAsset()
{
	return mrAsset;
}

void TabAssetProperty::ApplyChanges()
{	
	TabAssetPropertyGridPage* pPage		= static_cast<TabAssetPropertyGridPage*>(mpPropertyGrid->GetCurrentPage());	
	wxPropertyGridIterator itProperty	= pPage->GetIterator(wxPG_ITERATE_PROPERTIES);
	while( !itProperty.AtEnd() )
	{
		wxPGProperty* pProperty		= *itProperty;
		PropertyMetaData* pMetaData = static_cast<PropertyMetaData*>(pProperty->GetClientData());
		pMetaData->Save();
		itProperty++;
	}
}

void TabAssetProperty::OnToolbarDefault( wxCommandEvent& event )
{
	TabAssetPropertyGridPage*	pPage		= static_cast<TabAssetPropertyGridPage*>(mpPropertyGrid->GetCurrentPage());
	const wxArrayPGProperty&	aSelected 	= pPage->GetSelectedProperties();
	for(zUInt propIdx(0), propCount(aSelected.Count()); propIdx<propCount; ++propIdx)
	{
		wxPGProperty* pProp			= aSelected[propIdx];
		PropertyMetaData* pMetaData = (PropertyMetaData*)pProp->GetClientData();
		if ( pMetaData )
			pMetaData->SetDefaultValue();
	}
	pPage->RefreshModifiedItems();
}

void TabAssetProperty::OnToolbarOriginal( wxCommandEvent& event )
{
	TabAssetPropertyGridPage*	pPage		= static_cast<TabAssetPropertyGridPage*>(mpPropertyGrid->GetCurrentPage());
	const wxArrayPGProperty&	aSelected 	= pPage->GetSelectedProperties();
	for(zUInt propIdx(0), propCount(aSelected.Count()); propIdx<propCount; ++propIdx)
	{
		wxPGProperty* pProp			= aSelected[propIdx];
		PropertyMetaData* pMetaData = (PropertyMetaData*)pProp->GetClientData();
		if (pMetaData)
			pMetaData->SetOriginalValue();		
	}
	pPage->RefreshModifiedItems();
}

void TabAssetProperty::OnToolbarApply( wxCommandEvent& event )
{
	ApplyChanges();
}

}