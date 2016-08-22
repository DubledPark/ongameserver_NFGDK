#include "NFWindow.h"
#include <Nave/NFSync.h>

using namespace std;	// std::list 라고 안쓰고 list 라고만 선언해도 된다. 
						// 안쓰면 반드시 std::list라고 사용해야한다.

namespace NaveFramework {

	list<NFWindow*>		s_WindowList;
	NFWindow*			NFWindow::s_pMainWnd = NULL;


	///////////////////////////////////////////////////////////////////////////////
	//	Descrip	: 말그대로 윈도우용. Proc..
	//	Date	: 2001-04-11오전 10:41:02
	///////////////////////////////////////////////////////////////////////////////
	LRESULT CALLBACK LibraryProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		list<NFWindow*>::iterator Window;
		for( Window = s_WindowList.begin(); Window != s_WindowList.end(); ++Window )
		{
			NFWindow* tmp = (*Window);
			if((*Window) && (*Window)->GetWND() == hWnd)
			{
				return (*Window)->MsgProc(hWnd, uMsg, wParam, lParam);
			}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	VOID InitWindowsObject(HINSTANCE hInst, LPCWSTR Title, LPCWSTR Icon)
	{
		Nave::hInstance = hInst;

		// 윈도우의 생성
		WNDCLASSEX wcex;

		wcex.cbSize			= sizeof(WNDCLASSEX); 
		wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc	= (WNDPROC)LibraryProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 16;
		wcex.hInstance		= (HINSTANCE)Nave::hInstance;
		wcex.hIcon			= LoadIcon(hInst, (LPCWSTR)Icon);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= Title;
		wcex.hIconSm		= LoadIcon(Nave::hInstance, (LPCWSTR)Icon);

		::RegisterClassExW(&wcex);
	}

	HWND CreateWindowsEx(LPCWSTR Title, DWORD dwExStyle, DWORD dwStyle, int Sx, int Sy, int Width, int Height, HWND Parent)
	{
		// WS_CLIPCHILDREN|WS_CLIPSIBLINGS  이거 두개를 추가시켜야
		// 차일드 윈도우가 붙어있을때 깜빡이지 않는다.
		// 그리고 GetWindowDC를 하지 말고 GetDC를 한다.

		// Create The Window
		HWND hWnd = ::CreateWindowExW(
				dwExStyle,							// Extended Style For The Window
				Title,								// Class Name
				Title,								// Window Title
				dwStyle|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
				Sx, Sy,								// Window Position
				Width,								// Calculate Window Width
				Height,								// Calculate Window Height
				Parent,								// No Parent Window
				NULL,								// No Menu
				Nave::hInstance,						// Instance
				NULL);								// Dont Pass Anything To WM_CREATE

		return hWnd;								// Return FALSE
	}


	//////////////////////////////////////////////////////////////
	// 윈도우 관리자
	//////////////////////////////////////////////////////////////
	NFWindow::NFWindow()
	{
		m_hWnd = NULL;		// 자기자신의 윈도우 핸들
		m_hParent = NULL;		// 부모 윈도우 핸들
		m_bMouseIn = FALSE;
		m_bShow = FALSE;

		InsertWindows((NFWindow*)this);
	}

	NFWindow::~NFWindow()
	{
		DeleteWindows((NFWindow*)this);

//		if(IsWindow(m_hWnd))
//			DestroyWindow(m_hWnd);

		m_hWnd = NULL;			// 자기자신의 윈도우 핸들
		m_hParent = NULL;		// 부모 윈도우 핸들
	}

	VOID NFWindow::OnTrackMouseEvent()
	{
		if(!m_bMouseIn) 
		{ 
			TRACKMOUSEEVENT tme; 
			tme.cbSize = sizeof(TRACKMOUSEEVENT); 
			tme.dwFlags = TME_LEAVE; 
			tme.hwndTrack = m_hWnd; 
			tme.dwHoverTime = HOVER_DEFAULT; 
			TrackMouseEvent(&tme); 
			m_bMouseIn = TRUE; 
		} 
	}

	LRESULT NFWindow::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		POINT Point = Nave::GetMousePos();
		int X = Point.x;
		int Y = Point.y;

		switch(uMsg)
		{
		case WM_MOUSEWHEEL:
			// 자기 자신의 Wheel메시지 처리
			OnMouseWheel(X, Y, GET_WHEEL_DELTA(wParam));
			return 0;
		case WM_COMMAND:
			OnCommand(hWnd, LOWORD(wParam), HIWORD(wParam), lParam);
			break;
		case WM_LBUTTONUP:
			OnLButtonUp(X, Y);
			break;
		case WM_LBUTTONDOWN:
			OnLButtonDown(X, Y);
			break;
		case WM_LBUTTONDBLCLK:
			OnLButtonDClick(X, Y);
			break;
		case WM_MOUSELEAVE:	// Mouse Out
			m_bMouseIn = FALSE;
			OnMouseMove(X, Y);
			break;
		case WM_MOUSEHOVER:	// Mouse Over
			OnMouseOver(X, Y);
			break;
		case WM_MOUSEMOVE:
			OnTrackMouseEvent();	// Mouse Out을 처리하기 위해서.
			OnMouseMove(X, Y);
			break;
		case WM_RBUTTONUP:
			OnRButtonUp(X, Y);
			break;
		case WM_RBUTTONDOWN:
			OnRButtonDown(X, Y);
			break;
			// 이메시지는 return 0을 해줘야 제대로 화면 갱신이 이루어 진다.
		case WM_ERASEBKGND:
			return 0;
		case WM_PAINT:
			{
				HDC hDC = GetDC(hWnd);
				OnPaint(hWnd, hDC);
				ReleaseDC(hWnd, hDC);
			}
			break;
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	BOOL InsertWindows(NFWindow * ObjPtr)				// 리스트에 오브젝트를 추가
	{
		if(ObjPtr == NULL)
			return FALSE;

		s_WindowList.push_back(ObjPtr);
		return TRUE;	
	}

	BOOL DeleteWindows(NFWindow * ObjPtr)				// 리스트에 있는 오브젝트를 삭제
	{
		list<NFWindow*>::iterator obj;

		for( obj = s_WindowList.begin(); obj != s_WindowList.end(); ++obj )
		{
			if((*obj) == ObjPtr)
			{
				s_WindowList.erase(obj);
				return TRUE;
			}
		}
		return FALSE;
	}

	// NFWindow리스트에 등록된 hWnd의 윈도우값을 얻어온다.
	NFWindow* GetWINDOW(HWND hWnd)
	{
		list<NFWindow*>::iterator Window;

		for( Window = s_WindowList.begin(); Window != s_WindowList.end(); ++Window )
		{
			if((*Window) && (*Window)->GetWND() == hWnd)
			{
				return (*Window);
			}
		}
		return NULL;
	}

}