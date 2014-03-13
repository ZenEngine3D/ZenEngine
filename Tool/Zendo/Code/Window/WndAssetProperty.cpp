#include "ZendoPCH.h"
#include "WndAssetProperty.h"
#include "../Control/CtrlAssetPropertyTab.h"

//=================================================================================================
// EVENTS TABLE
//=================================================================================================
BEGIN_EVENT_TABLE(WndAssetProperty, wxPanel)
END_EVENT_TABLE()

WndAssetProperty::WndAssetProperty(wxWindow *parent, const wxString& title)
: wxPanel(parent, wxID_ANY)
, mpNotebook(NULL)
{	
	mdTabPerAsset.Init(50);
	mdTabPerAsset.SetDefaultValue(NULL);


	wxBoxSizer* sizer		= zenNewDefault wxBoxSizer(wxVERTICAL);
	mpNotebook				= zenNewDefault wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);
	sizer->Add(mpNotebook, 100,	wxGROW | wxALL, 0);
	SetSizer(sizer);
	SetSize(wxSize(400,200));
}

WndAssetProperty::~WndAssetProperty()
{
}

void WndAssetProperty::AddAssetTab(zHash64 _hAssetID)
{	
	zeAss::AssetItem* pEditItem	= zeMgr::Asset.AssetGet(_hAssetID);
	if( pEditItem )
	{
		BCtrl::TabAssetProperty* pTabAsset	= mdTabPerAsset[_hAssetID];
		if( !pTabAsset )
		{
			pTabAsset = zenNewDefault BCtrl::TabAssetProperty(mpNotebook, pEditItem);
			mpNotebook->AddPage(pTabAsset, static_cast<const char*>(pEditItem->GetName()) );
			mdTabPerAsset.Set(_hAssetID, pTabAsset);
		}
		mpNotebook->SetSelection( mpNotebook->GetPageIndex(pTabAsset) );
	}
}

