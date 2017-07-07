//#################################################################################################
// Integration of wxWidget 3.0.2 in Zen Engine
//#################################################################################################
#pragma once

#if ZEN_ENGINETOOL

#pragma warning( disable : 4996 ) //Disable window 'function security' warning

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//-------------------------------------------------------------------------------------------------
// premake defines entry function as 'main(int, char**)', 
// but wxwidget try to use 'main(int, wchar**)'
// undo wxwidget behaviour
// Same as IMPLEMENT_APP(), but for console applications.
#undef wxIMPLEMENT_WXWIN_MAIN_CONSOLE
#define wxIMPLEMENT_WXWIN_MAIN_CONSOLE	\
	int main(int argc, char **argv)		\
	{									\
		wxDISABLE_DEBUG_SUPPORT();		\
		return wxEntry(argc, argv);		\
	}
//-------------------------------------------------------------------------------------------------

// Controls
#include <wx/aui/aui.h>
#include <wx/dataview.h>
#include <wx/treectrl.h>
#include <wx/treelist.h>
#include <wx/listctrl.h>
#include <wx/tglbtn.h>
#include <wx/srchctrl.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>


#include <wx/artprov.h>
// File access
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/file.h>
#include <wx/dir.h>

#endif
