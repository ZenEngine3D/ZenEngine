#pragma once
#ifndef __BetlWndAssetProperty_h__
#define __BetlWndAssetProperty_h__ 

#include "../Control/CtrlAssetPropertyTab.h"

class WndAssetProperty : public wxPanel
{
public:
										WndAssetProperty(wxWindow *parent, const wxString& title);
										~WndAssetProperty();
	void								AddAssetTab(zenHash64 _hAssetID);
protected:
	
	DECLARE_EVENT_TABLE()
	wxAuiNotebook*							mpNotebook;
	zenMap<BCtrl::TabAssetProperty*>::Key64	mdTabPerAsset;
};

#endif