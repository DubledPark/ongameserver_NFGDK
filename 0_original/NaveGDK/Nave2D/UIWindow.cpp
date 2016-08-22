#include "Nave2D.h"
#include "UIWindow.h"
#include <Nave/NFDeltaTimer.h>

namespace Nave2D {

	// 윈도우 메시지 Define
	#define WM_EDIT_RETURN		WM_USER + 1000
	#define WM_EDIT_TAB			WM_USER + 1001
	#define WM_EDIT_SETFOCUS	WM_USER + 1002

	UIWindow::UIWindow()
	{
		NFWindow::s_pMainWnd = this;

		m_bWindowMode = TRUE;
		m_bReady = FALSE;
		m_bActive = FALSE;
		m_bFocus = FALSE;
		m_Exit = FALSE;

		m_iWidth = 640;
		m_iHeight = 480;
	}

	UIWindow::~UIWindow()
	{
		NFWindow::s_pMainWnd = NULL;
	}

	HRESULT UIWindow::MinimizedWindows()
	{
		m_bActive = FALSE;

		return S_OK;
	}

	HRESULT UIWindow::RestoredWindows()
	{
		m_bActive = TRUE;

		return S_OK;
	}

	///////////////////////////////////////////////////////////////////////////////
	//	Descrip	: 말그대로 사용할 MsgProc , WindowProc에서 호출된다.
	//	Date	: 2001-04-11오전 10:41:02
	///////////////////////////////////////////////////////////////////////////////
	LRESULT UIWindow::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
	/*
			// Alt+F4 나 Close 메뉴로 종료햇을때 처리
		case WM_SYSKEYDOWN:
			{
				int wmId, wmEvent;
				wmId    = LOWORD(wParam); 
				wmEvent = HIWORD(wParam); 
				// Parse the menu selections:
				switch (wmId)
				{
				case VK_RETURN:
					{
						m_bReady = FALSE;
						// 화면 모드 변경.

						m_bWindowMode = !m_bWindowMode;

						Display::g_Video.ChangeVideo(m_bWindowMode);

						m_bReady = TRUE;
					}
					break;
				}
			}
			break;
	*/
		switch(uMsg)
		{
		case WM_OBJECTEVENT:
			// 사용자 커멘드를 입력받는다.
			OnObjectEvent(LOWORD(wParam), HIWORD(wParam), lParam);
			break;

		case WM_SETFOCUS:
			m_bFocus = TRUE;
			break;
		case WM_KILLFOCUS:
			m_bFocus = FALSE;
			break;
		case WM_SIZE:
			if(!m_bReady)
				break;

			if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
			{
				m_bFocus = FALSE;
				MinimizedWindows();
			}
			else if(SIZE_RESTORED == wParam)
			{
				m_bFocus = TRUE;
				RestoredWindows();
			}
			break;

		case WM_MOUSEMOVE:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			if(!m_bReady) return FALSE;
			break;

/*
		case WM_PAINT:
			if( m_bReady && m_bActive)
				OnRender();
			else 
			{
				HDC hDC = GetDC(hWnd);

				HPEN Pen = ::CreatePen(PS_SOLID, 1, RGB(0,0,0));
				HPEN OldPen = (HPEN)SelectObject(hDC, Pen);

				HBRUSH Brush = ::CreateSolidBrush(RGB(0,0,0));
				HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, Brush);
				Rectangle(hDC, 0, 0, m_iWidth, m_iHeight);

				SelectObject(hDC, (HBRUSH)OldBrush);
				DeleteObject(Brush);
				SelectObject(hDC, (HPEN)OldPen);
				DeleteObject(Pen);
				::ReleaseDC(hWnd, hDC);
			}
			break;
*/
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			EndProcess();
			return FALSE;
		}

		return NFWindow::MsgProc(hWnd, uMsg, wParam, lParam);
	}

	VOID UIWindow::ReSizeWindow(BOOL bWindowMode, int Width, int Height)
	{
		m_bWindowMode = bWindowMode;

		DWORD Style;
		if( m_bWindowMode )
		{
			if(m_bPopupMode)
				SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_BORDER|WS_SYSMENU|WS_VISIBLE|WS_MINIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
			else
			{
				Style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX|WS_THICKFRAME)|WS_CLIPCHILDREN|WS_VISIBLE|WS_CLIPSIBLINGS;
				SetWindowLong( m_hWnd, GWL_STYLE, Style);
			}
		}
		else
		{
			// Set fullscreen-mode style
			SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE|WS_MINIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		}

		Style = ::GetWindowLong(m_hWnd, GWL_STYLE);
		RECT rc;
		SetRect( &rc, 0, 0, Width, Height );

		// 메뉴가 있으면 TRUE이다.
		if(GetMenu(m_hWnd))
			AdjustWindowRect( &rc, Style, TRUE );
		else AdjustWindowRect( &rc, Style, FALSE );

		// rc모양으로 윈도우 크기변환
		int SWidth = GetSystemMetrics(SM_CXSCREEN);
		int SHeight = GetSystemMetrics(SM_CYSCREEN);
		int w= rc.right-rc.left;
		int h= rc.bottom-rc.top;
		int x= SWidth/2-w/2;
		int y= SHeight/2-h/2;

		if(m_bWindowMode)
		{
			SetWindowPos(m_hWnd, HWND_NOTOPMOST, x, y, w, h, 0);//SWP_NOZORDER | SWP_NOACTIVATE);	
		}
		else
		{
			SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, Width, Height, 0);//SWP_NOZORDER | SWP_NOACTIVATE);	
		}

		Nave::hMainWnd = m_hWnd;
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	///////////////////////////////////////////////////////////////////////////////
	//	Descrip	: 윈도우를 생성한다.
	//	Return	: 성공. 실패..
	//	Date	: 2001-04-11오전 10:41:50
	///////////////////////////////////////////////////////////////////////////////
	BOOL UIWindow::CreateWindows(HINSTANCE hInstance, int nCmdShow, LPCWSTR Title, int Width, int Height, BOOL PopupMode, BOOL WindowMode, LPCWSTR Icon)
	{
		BOOL Val = TRUE;

		// 윈도우의 초기화 이걸 안해주면 윈도우가 생성되지 않는다.
		NaveFramework::InitWindowsObject(hInstance, Title, Icon);

		m_bPopupMode = PopupMode;
		m_bWindowMode = WindowMode;
		wcscpy(m_WindowsTitle, Title);
		m_iWidth = Width;
		m_iHeight = Height;

		// 윈도우 스타일을 위한 변수
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		dwExStyle=0;	// Window Extended Style

		// 윈도우 영역 설정.
		m_WindowRect.SetRect(0, 0, Width, Height);

		//WS_CLIPCHILDREN and WS_CLIPSIBLINGS 은 기본으로 추가된다.
		if(WindowMode)
		{
			if(m_bPopupMode)
				dwStyle=WS_POPUP|WS_BORDER|WS_SYSMENU|WS_MINIMIZEBOX;	// Windows Style
			else
				dwStyle = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX|WS_THICKFRAME);

			// 이 윈도우는 윈도우 기본 생성 함수이다.
			Nave::hMainWnd = NaveFramework::CreateWindowsEx(Title, 0, dwStyle, 0, 0, Width, Height);
		}
		else
		{
			dwStyle=WS_POPUP|WS_MINIMIZEBOX|WS_SYSMENU;	// Windows Style
			// 이 윈도우는 윈도우 기본 생성 함수이다.
			Nave::hMainWnd = NaveFramework::CreateWindowsEx(Title, WS_EX_TOPMOST, dwStyle, 0, 0, Width, Height);
		}
/*
		m_hWnd = Nave::hMainWnd;

		OnSetWnd(Nave::hMainWnd);

		if(!Nave::hMainWnd)
		{
			MessageBoxW(NULL, L"윈도우 생성 에러", L"Error", MB_OK);
			return FALSE;
		}

		ReSizeWindow(m_bWindowMode, m_iWidth, m_iHeight);
		
		// 아이콘 로드 (작은 것 따로 하나)
		HICON hIcon = LoadIcon( hInstance, Icon );
		SendMessage( Nave::hMainWnd, WM_SETICON, (WPARAM)ICON_BIG,   (LPARAM)hIcon );
		SendMessage( Nave::hMainWnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon );

		if( FAILED( InitObjects() ) )
			Val = FALSE;

		ShowWindow(Nave::hMainWnd, SW_SHOW);
*/
		return Val;
	}

	HRESULT UIWindow::StartProcess(BOOL bWindow)
	{
		m_bReady = TRUE;
		m_bActive = TRUE;

		Nave::NFDeltaTimer<float> timer(0, 5);
		float dt = timer.GetTimeDelta();

		MSG		msg;
		BOOL bGotMsg;

		while( !m_Exit ) 
		{ 
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ); 

			if( bGotMsg ) 
			{ 
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} 
			else if(m_bReady)
			{
				dt = timer.GetTimeDelta();

				OnUpdate(dt);
			
				OnRender();

				Sleep(1);
			}
		}

		// 클라이언트 오브젝트를 삭제한다.
		ReleaseObjects();

		return S_OK;
	}

	HRESULT UIWindow::EndProcess()
	{
		m_Exit = TRUE;
		m_bActive = FALSE;
		m_bReady = FALSE;
		return S_OK;
	}

}