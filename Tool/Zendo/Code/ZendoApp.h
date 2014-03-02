#pragma once
#ifndef __ZendoApp_h__
#define __ZendoApp_h__ 

#include "Window/WndMain.h"

// Define a new application type, each program should derive a class from wxApp
class ZendoApp : public wxApp
{
public:
	enum enumIcon
	{
		keIco_Asset16,
		keIco_Asset32,
		keIco_Asset128,
		keIco__Count
	};
							ZendoApp();
	virtual bool			OnInit();
	virtual int				OnExit();
	//virtual int				MainLoop();
	//void onIdle(wxIdleEvent& evt);

	bool					InitResources();
	const wxIcon*			GetIcon(const zeAss::AssetItem& _Item);
	WndMain*				mpFrame;

	wxImageList*			maIcon[keIco__Count];

protected:
	zenMap<wxIcon*>::Key64	mdAssetIcon;	//!< Buffer asset icon so they only get built once
};

ZendoApp& wxGetApp();

#endif
