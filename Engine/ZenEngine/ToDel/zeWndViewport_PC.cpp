#include "zeEngine.h"

namespace FWnd
{	
	/*
	Viewport_PC::Viewport_PC()
	: mhMainWindow(NULL)
	{
	}

	void Viewport_PC::Initialize(HWND _hWndHandle, zVec2U16 _vSize)
	{
		mhMainWindow	= _hWndHandle;
		mvSize			= _vSize;
	}
	*/
	Window::Window(const WCHAR* _zWindowName, zVec2U16 _ClientSize)
	: mhMainWindowThread(NULL)
	, mhWindowInstance(NULL)
	, meMainWindowThreadStatus(keThread_Ended)
	{
		wcscpy_s(mzWindowName, _countof(mzWindowName), _zWindowName);
		mvSize = _ClientSize;
	}

	bool Window::Initialize()
	{
		meMainWindowThreadStatus = keThread_Starting;
		mhMainWindowThread		= CreateThread( NULL, 0,  WndThread, this, 0, NULL);  
		while( meMainWindowThreadStatus == keThread_Starting )
			SleepEx(1, false);
		return meMainWindowThreadStatus == keThread_Running;
	}
	
	//==================================================================================================
	//! @brief		Receives Windows OS messages and process them
	//! @details	[MainWindowThread] 
	//--------------------------------------------------------------------------------------------------
	//! @return		
	//==================================================================================================
	LRESULT CALLBACK Window::WndEventCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* pParentWindow = (Window*)GetWindowLongPtr(hwnd, 0);
		switch(msg)
		{
		case WM_CLOSE:	{ DestroyWindow(hwnd);	break;	}
		case WM_DESTROY:{ pParentWindow->meMainWindowThreadStatus = keThread_Ended; break; }
		case WM_SIZING:	{ 
			RECT* pSize = (RECT*)lParam; 
			pParentWindow->SetSize(zVec2U16(zU16(pSize->right-pSize->left), zU16(pSize->bottom-pSize->top)));
			break; }
		case WM_PAINT:	{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint( hwnd, &ps );	
				EndPaint( hwnd, &ps );
				break; }		
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}

	//==================================================================================================
	//! @brief		Thread for the update of the Main Window of the application
	//! @details	[MainWindowThread] Create a new window, and process the messages
	//!				received, in an infinite loop, until user close the window 
	//!				or the thread is killed
	//--------------------------------------------------------------------------------------------------
	//! @return		
	//==================================================================================================
	DWORD WINAPI Window::WndThread( LPVOID lpParam ) 
	{		
		WNDCLASSEX	wc;
		MSG			Msg;
		HANDLE		hStdout(NULL);
		Window* pParentWindow = (Window*)lpParam;

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		if( hStdout == INVALID_HANDLE_VALUE )  
		{
			pParentWindow->meMainWindowThreadStatus = keThread_Error;
			return false;
		}

		//Step 1: Registering the Window Class
		const TCHAR zClassName[] = L"AWWindowClass";
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = WndEventCallback;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = sizeof(Window*);
		wc.hInstance     = pParentWindow->mhWindowInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = zClassName;
		wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
		
		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL, L"Window Registration Failed!", L"Error!",	MB_ICONEXCLAMATION | MB_OK);
			pParentWindow->meMainWindowThreadStatus = keThread_Error;
			return false;
		}

		pParentWindow->mhMainWindow = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			zClassName,
			L"Zen Engine",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, pParentWindow->mvSize.x, pParentWindow->mvSize.y,
			NULL, NULL, pParentWindow->mhWindowInstance, NULL);

		if(pParentWindow->mhMainWindow == NULL)
		{
			MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			pParentWindow->meMainWindowThreadStatus = keThread_Error;
			return false;
		}

		SetWindowLongPtr( pParentWindow->mhMainWindow, 0, (LONG_PTR)pParentWindow);
			
		ShowWindow(pParentWindow->mhMainWindow, SW_SHOWNORMAL);
		UpdateWindow(pParentWindow->mhMainWindow);

		pParentWindow->meMainWindowThreadStatus = keThread_Running;
		while(GetMessage(&Msg, NULL, 0, 0) > 0 && pParentWindow->meMainWindowThreadStatus == keThread_Running )
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		return true;
	}

}