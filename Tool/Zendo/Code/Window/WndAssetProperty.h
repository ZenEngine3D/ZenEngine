#pragma once
#ifndef __BetlWndAssetProperty_h__
#define __BetlWndAssetProperty_h__ 

#include "../Control/CtrlAssetPropertyTab.h"

class WndAssetProperty : public wxPanel
{
public:
										WndAssetProperty(wxWindow *parent, const wxString& title);
										~WndAssetProperty();
	void								AddAssetTab(zenAss::zAssetItem rEditItem);
protected:
	
	DECLARE_EVENT_TABLE()
	wxAuiNotebook*									mpNotebook;
	zArraySparse<BCtrl::TabAssetProperty*>::Key32	mdTabPerAsset;
};

#endif