#include "zeEngine.h"

namespace zen { namespace zenWnd
{	
	Window::Window(const WCHAR* _zWindowName, zVec2U16 _ClientSize)
	: mhMainWindowThread(nullptr)
	, mhWindowInstance(nullptr)
	, meMainWindowThreadStatus(keThread_Ended)
	{
		wcscpy_s(mzWindowName, _countof(mzWindowName), _zWindowName);
		mvSize = _ClientSize;
	}

	bool Window::Initialize()
	{
		meMainWindowThreadStatus = keThread_Starting;
		mhMainWindowThread		= CreateThread( nullptr, 0,  WndThread, this, 0, nullptr);  
		while( meMainWindowThreadStatus == keThread_Starting )
			SleepEx(1, false);
		return meMainWindowThreadStatus == keThread_Running;
	}
	
	void Window::GetInput(WindowInputState& _WindowInputOut, zU8 _uMaxKeyProcessed)
	{
		mWindowMutex.lock();
		_WindowInputOut = mWindowInput;		
		mWindowInput.Reset(_uMaxKeyProcessed);
		mWindowMutex.unlock();

		if (_WindowInputOut.maCharacterPressed.Count() > _uMaxKeyProcessed)
			_WindowInputOut.maCharacterPressed.SetCount(_uMaxKeyProcessed);
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
		if( pParentWindow )
		{
			switch(msg)
			{
			case WM_CLOSE:			
				DestroyWindow(hwnd); 
				pParentWindow->meMainWindowThreadStatus = keThread_Ended;
				break;
			case WM_DESTROY:		
				pParentWindow->meMainWindowThreadStatus = keThread_Ended; 
				break; 
			case WM_SIZE:			
				pParentWindow->SetSize(*(zVec2U16*)&lParam); 
				break; 
			
			// Input processing
			case WM_LBUTTONDOWN:
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.mbIsMouseDown[WindowInputState::keMouseBtn_Left] = true;
				pParentWindow->mWindowMutex.unlock();
				break;
			case WM_LBUTTONUP:
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.mbIsMouseDown[WindowInputState::keMouseBtn_Left] = false;
				pParentWindow->mWindowMutex.unlock();
				break;
			case WM_RBUTTONDOWN:
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.mbIsMouseDown[WindowInputState::keMouseBtn_Right] = true;
				pParentWindow->mWindowMutex.unlock();				
				break;
			case WM_RBUTTONUP:
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.mbIsMouseDown[WindowInputState::keMouseBtn_Right] = false;
				pParentWindow->mWindowMutex.unlock();
				break;
			case WM_MBUTTONDOWN:
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.mbIsMouseDown[WindowInputState::keMouseBtn_Middle] = true;
				pParentWindow->mWindowMutex.unlock();
				break;
			case WM_MBUTTONUP:
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.mbIsMouseDown[WindowInputState::keMouseBtn_Middle] = false;
				pParentWindow->mWindowMutex.unlock();
				break;
			case WM_MOUSEWHEEL:
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.mfMouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
				pParentWindow->mWindowMutex.unlock();
				break;
			case WM_MOUSEMOVE:
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.mvMousePos = zVec2S16((zI16)(lParam&0xFFFF), (zI16)((lParam >> 16)&0xFFFF));
				pParentWindow->mWindowMutex.unlock();
				break;
			case WM_KEYDOWN:				
				if(wParam <= 0xFF)
				{
					pParentWindow->mWindowMutex.lock();
					pParentWindow->mWindowInput.mbIsKeyDown[wParam] = true;
					pParentWindow->mWindowInput.mbIsKeyPressed[wParam] = (lParam & 0x40000000) == 0;
					pParentWindow->mWindowMutex.unlock();
				}			
				break;
			case WM_KEYUP:
				if(wParam <= 0xFF)
				{
					pParentWindow->mWindowMutex.lock();
					pParentWindow->mWindowInput.mbIsKeyDown[wParam] = false;
					pParentWindow->mWindowInput.mbIsKeyReleased[wParam] = true;
					pParentWindow->mWindowMutex.unlock();
				}
				break;
			case WM_CHAR:
				// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
				pParentWindow->mWindowMutex.lock();
				pParentWindow->mWindowInput.maCharacterPressed.Push((zU16)wParam);
				pParentWindow->mWindowMutex.unlock();				
				break;

			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
		}
		
		return DefWindowProc(hwnd, msg, wParam, lParam);
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
		HANDLE		hStdout(nullptr);
		Window* pParentWindow = (Window*)lpParam;

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		if( hStdout == INVALID_HANDLE_VALUE )  
		{
			pParentWindow->meMainWindowThreadStatus = keThread_Error;
			return false;
		}

		//Step 1: Registering the Window Class
		const TCHAR zClassName[] = L"ZenWindowClass";
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = WndEventCallback;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = sizeof(Window*);
		wc.hInstance     = pParentWindow->mhWindowInstance;
		wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName  = nullptr;
		wc.lpszClassName = zClassName;
		wc.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);
		
		if(!RegisterClassEx(&wc))
		{
			MessageBox(nullptr, L"Window Registration Failed!", L"Error!",	MB_ICONEXCLAMATION | MB_OK);
			pParentWindow->meMainWindowThreadStatus = keThread_Error;
			return false;
		}

		zVec2U16 vSize = pParentWindow->mvSize;
		pParentWindow->mhMainWindow = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			zClassName,
			L"Zen Engine",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, vSize.x, vSize.y,
			nullptr, nullptr, pParentWindow->mhWindowInstance, nullptr);

		if(pParentWindow->mhMainWindow == nullptr)
		{
			MessageBox(nullptr, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			pParentWindow->meMainWindowThreadStatus = keThread_Error;
			return false;
		}

		SetWindowLongPtr( pParentWindow->mhMainWindow, 0, (LONG_PTR)pParentWindow);
			
		ShowWindow(pParentWindow->mhMainWindow, SW_SHOWNORMAL);
		UpdateWindow(pParentWindow->mhMainWindow);

		pParentWindow->meMainWindowThreadStatus = keThread_Running;
		while(GetMessage(&Msg, nullptr, 0, 0) > 0 && pParentWindow->meMainWindowThreadStatus == keThread_Running )
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		return true;
	}

}}