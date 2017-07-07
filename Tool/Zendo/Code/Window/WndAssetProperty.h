#pragma once
#pragma once
#include "../Control/CtrlAssetPropertyTab.h"

class WndAssetProperty : public wxPanel
{
public:
										WndAssetProperty(wxWindow *parent, const wxString& title);
										~WndAssetProperty();
	void								AddAssetTab(zenAss::zAssetItemRef rEditItem);
	void								AssetTabRemoved(BCtrl::TabAssetProperty& _AssetTab);
protected:	
	DECLARE_EVENT_TABLE()
	wxAuiNotebook*									mpNotebook;
	zArraySparse<BCtrl::TabAssetProperty*>::Key64	mdTabPerAsset;
};
