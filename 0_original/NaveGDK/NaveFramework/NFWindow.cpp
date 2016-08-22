#include "NFWindow.h"
#include <Nave/NFSync.h>

using namespace std;	// std::list ��� �Ⱦ��� list ��� �����ص� �ȴ�. 
						// �Ⱦ��� �ݵ�� std::list��� ����ؾ��Ѵ�.

namespace NaveFramework {

	list<NFWindow*>		s_WindowList;
	NFWindow*			NFWindow::s_pMainWnd = NULL;


	///////////////////////////////////////////////////////////////////////////////
	//	Descrip	: ���״�� �������. Proc..
	//	Date	: 2001-04-11���� 10:41:02
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

		// �������� ����
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
		// WS_CLIPCHILDREN|WS_CLIPSIBLINGS  �̰� �ΰ��� �߰����Ѿ�
		// ���ϵ� �����찡 �پ������� �������� �ʴ´�.
		// �׸��� GetWindowDC�� ���� ���� GetDC�� �Ѵ�.

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
	// ������ ������
	//////////////////////////////////////////////////////////////
	NFWindow::NFWindow()
	{
		m_hWnd = NULL;		// �ڱ��ڽ��� ������ �ڵ�
		m_hParent = NULL;		// �θ� ������ �ڵ�
		m_bMouseIn = FALSE;
		m_bShow = FALSE;

		InsertWindows((NFWindow*)this);
	}

	NFWindow::~NFWindow()
	{
		DeleteWindows((NFWindow*)this);

//		if(IsWindow(m_hWnd))
//			DestroyWindow(m_hWnd);

		m_hWnd = NULL;			// �ڱ��ڽ��� ������ �ڵ�
		m_hParent = NULL;		// �θ� ������ �ڵ�
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
			// �ڱ� �ڽ��� Wheel�޽��� ó��
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
			OnTrackMouseEvent();	// Mouse Out�� ó���ϱ� ���ؼ�.
			OnMouseMove(X, Y);
			break;
		case WM_RBUTTONUP:
			OnRButtonUp(X, Y);
			break;
		case WM_RBUTTONDOWN:
			OnRButtonDown(X, Y);
			break;
			// �̸޽����� return 0�� ����� ����� ȭ�� ������ �̷�� ����.
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

	BOOL InsertWindows(NFWindow * ObjPtr)				// ����Ʈ�� ������Ʈ�� �߰�
	{
		if(ObjPtr == NULL)
			return FALSE;

		s_WindowList.push_back(ObjPtr);
		return TRUE;	
	}

	BOOL DeleteWindows(NFWindow * ObjPtr)				// ����Ʈ�� �ִ� ������Ʈ�� ����
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

	// NFWindow����Ʈ�� ��ϵ� hWnd�� �����찪�� ���´�.
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