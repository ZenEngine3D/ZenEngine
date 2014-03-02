#pragma once
#ifndef __BetlWndMain_h__
#define __BetlWndMain_h__ 

class WndMain : public wxFrame
{
public:
								WndMain(const wxString& title);
								~WndMain();
	bool						Initialize();
	class MyRenderWindow*		GetRenderPanel(){return mpWndRenderPanel;}
	class WndAssetBrowser*		GetWndAssetBrowser(){return mpWndAssetBrowser;}
	class WndAssetProperty*		GetWndAssetProperty(){return mpWndAssetProperty;}

	// event handlers (these functions should _not_ be virtual)
	void						OnQuit(wxCommandEvent& event);
	void						OnAbout(wxCommandEvent& event);	
private:
	// any class wishing to process wxWidgets events must use this macro
	DECLARE_EVENT_TABLE()
	wxAuiManager				mLayoutMgr;
	MyRenderWindow*				mpWndRenderPanel;
	class WndAssetBrowser*		mpWndAssetBrowser;
	class WndAssetProperty*		mpWndAssetProperty;
};

#endif
