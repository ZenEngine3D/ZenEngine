#pragma once
#ifndef __BetlTabAssetProperty_h__
#define __BetlTabAssetProperty_h__ 

namespace BCtrl
{

class TabAssetPropertyGridPage : public wxPropertyGridPage
{
public:
	// Return false here to indicate unhandled events should be
	// propagated to manager's parent, as normal.
	virtual bool			IsHandlingAllEvents() const { return false; }
	void					RefreshModifiedItems();
protected:
	void					OnPropertyChange( wxPropertyGridEvent& event );
	
private:
	DECLARE_EVENT_TABLE()
};


class TabAssetProperty : public wxPanel
{
public:
								TabAssetProperty(wxWindow* _pParent, const zenAss::zAssetItem& _rEditAsset);
								~TabAssetProperty();
	void						ApplyChanges();
	const zenAss::zAssetItem&	GetAsset();
protected:
	//void						UpdatePropertiesState();
	void						OnToolbarDefault	( wxCommandEvent& event );
	void						OnToolbarOriginal	( wxCommandEvent& event );
	void						OnToolbarApply		( wxCommandEvent& event );
	
	zenAss::zAssetItem		mrAsset;
	wxPropertyGridManager*	mpPropertyGrid;
	DECLARE_EVENT_TABLE()
};



}
#endif