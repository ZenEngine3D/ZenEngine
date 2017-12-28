#include "ZendoPCH.h"
#include "WndMain.h"
#include "WndAssetBrowser.h"
#include "WndAssetProperty.h"
#include "../Control/CtrlAssetPropertyTab.h"

class MyRenderWindow : public wxWindow 
{
public:
	MyRenderWindow(wxWindow* _pParent)
	: wxWindow(_pParent,-1)	
	{
		zenRes::zGfxStateRaster::Config			DefaultRasterConfig;
		zenRes::zGfxRenderPass::ConfigColorRT	ColorRTConfig;
		zenRes::zGfxRenderPass::ConfigDepthRT	DepthRTConfig;				
		mrGfxWindow							= zenRes::zGfxWindow::Create(GetHWND());		
		mrStateRaster						= zenRes::zGfxStateRaster::Create(DefaultRasterConfig);
		ColorRTConfig.mrTargetSurface		= mrGfxWindow.GetBackbuffer();
		mrRndPassGeneric					= zenRes::zGfxRenderPass::Create("Generic",0,ColorRTConfig,DepthRTConfig, mrStateRaster );
		mTimer.SetOwner(this);
		Bind(wxEVT_TIMER, &MyRenderWindow::onTimer, this);
		mTimer.Start(16);
	}
	~MyRenderWindow()
	{
		mTimer.Stop();
	}
	void onTimer(wxTimerEvent& evt)
	{
		Render();
		evt.Skip();
	}
	void Render()
	{		
		float t = static_cast<float>(zenSys::GetElapsedSec() / 3.0);	// Update our time animation
		mrGfxWindow.FrameBegin();
		zenGfx::zCommandContext rContextRoot = zenGfx::zCommandContext::Create("RenderLoop", mrRndPassGeneric);		
		zVec4F vClearColor = zenMath::TriLerp<zVec4F>( zVec4F(0.5f,0.5f,0.5f,1), zVec4F(0.1f,0.1f,0.20f,1), zVec4F(0.5f,0.5f,0.5f,1), zenMath::Fract(t) );
		zenGfx::zCommand::ClearColor(rContextRoot, mrGfxWindow.GetBackbuffer(), vClearColor);
		rContextRoot.Submit();
		//mrGfxWindow.GetBackbuffer().Clear( true, vClearColor, true, 0 );
		//aDrawcalls.Push( zenRes::zGfxDrawCall::ClearColor(mrRndPassGeneric, 0, mrGfxWindow.GetBackbuffer(), vClearColor) );
		//zenRes::zGfxDrawCall::Submit(aDrawcalls);
		mrGfxWindow.FrameEnd();
		
	}

protected:
	void OnPaint(wxPaintEvent& event)
	{
		wxPaintDC dc(this);	// must always be here
		Render();		
	}

	wxTimer							mTimer;
	zenRes::zGfxWindow				mrGfxWindow;
	zenRes::zGfxStateRaster			mrStateRaster;
	zenRes::zGfxRenderPass			mrRndPassGeneric;
	wxDECLARE_NO_COPY_CLASS(MyRenderWindow);
	DECLARE_EVENT_TABLE()
};
BEGIN_EVENT_TABLE(MyRenderWindow, wxWindow)
	EVT_PAINT(MyRenderWindow::OnPaint)
	//EVT_SIZE(MyRenderWindow::OnSize)	
	//EVT_ERASE_BACKGROUND(MyRenderWindow::OnEraseBackground)
	//EVT_MOUSE_EVENTS(MyRenderWindow::OnMouse)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------
// IDs for the controls and the menu commands
enum
{
	// menu items
	Minimal_Quit = wxID_EXIT,

	// it is important for the id corresponding to the "About" command to have
	// this standard value as otherwise it won't be handled properly under Mac
	// (where it is special and put into the "Apple" menu)
	Minimal_About = wxID_ABOUT
};

BEGIN_EVENT_TABLE(WndMain, wxFrame)
	EVT_MENU(Minimal_Quit,  WndMain::OnQuit)
	EVT_MENU(Minimal_About, WndMain::OnAbout)
END_EVENT_TABLE()

WndMain::WndMain(const wxString& title)
: wxFrame(nullptr, wxID_ANY, title)
, mLayoutMgr(this, wxAUI_MGR_DEFAULT|wxAUI_MGR_TRANSPARENT_DRAG)
{	
	// create several text controls
	mpWndRenderPanel = zenNewDefault MyRenderWindow(this);
}

WndMain::~WndMain()
{	
	mLayoutMgr.UnInit();	// deinitialize the frame manager
}

bool WndMain::Initialize()
{	
	wxAuiPaneInfo panelInfo;
	panelInfo.Left();
	panelInfo.Gripper(false);
	panelInfo.GripperTop(false);
	panelInfo.Dock();
	panelInfo.CloseButton(false);
	panelInfo.PaneBorder(false);
	panelInfo.PinButton(false);
	panelInfo.CaptionVisible(true);

	SetSize(wxRect(0, 0, 1280, 720));
		

	// add the panes to the manager
	panelInfo.Caption(wxT("Browser"));
	mpWndAssetBrowser = zenNewDefault WndAssetBrowser(this, "Main Asset Browser");
	mLayoutMgr.AddPane(mpWndAssetBrowser, panelInfo );
	
	panelInfo.Caption(wxT("Properties"));
	panelInfo.Right();
	mpWndAssetProperty = zenNewDefault WndAssetProperty(this, "Properties");
	mLayoutMgr.AddPane(mpWndAssetProperty, panelInfo);	
	
	panelInfo.Caption(wxT("Rendering"));
	panelInfo.Center();
	mLayoutMgr.AddPane(mpWndRenderPanel, panelInfo);

	//-------------------------------
	// set the frame icon
	SetIcon(wxICON(sample));

#if wxUSE_MENUS
	// create a menu bar
	wxMenu *fileMenu = zenNewDefault wxMenu;

	// the "About" item should be in the help menu
	wxMenu *helpMenu = zenNewDefault wxMenu;
	helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");
	fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

	// now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = zenNewDefault wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
	// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(2);
	SetStatusText("Welcome to wxWidgets!");
#endif // wxUSE_STATUSBAR

	// tell the manager to "commit" all the changes just made
	mLayoutMgr.Update();
	return true;

	//BackbufferView = EExp::CreateView( EExp::ManagerRender::Get().GetBackbufferColor(), EExp::ManagerRender::Get().GetBackbufferDepth() );
	//return BackbufferView.IsValid();
}

// event handlers
void WndMain::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}

void WndMain::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxString::Format
		(
		"Welcome to %s!\n"
		"\n"
		"This is the minimal wxWidgets sample\n"
		"running under %s.",
		wxVERSION_STRING,
		wxGetOsDescription()
		),
		"About wxWidgets minimal sample",
		wxOK | wxICON_INFORMATION,
		this);
}

