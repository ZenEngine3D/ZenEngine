#pragma once

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
	virtual bool					OnInit();
	virtual int						OnExit();
	//virtual int				MainLoop();
	//void onIdle(wxIdleEvent& evt);

	bool							InitResources();
	const wxIcon*					GetIcon(const zenAss::zAssetItemRef& _rAsset);
	WndMain*						mpFrame;

	wxImageList*					maIcon[keIco__Count];

protected:
	zArraySparse<wxIcon*>::Key32	mdAssetIcon;	//!< Buffer asset icon so they only get built once
};

ZendoApp& wxGetApp();
