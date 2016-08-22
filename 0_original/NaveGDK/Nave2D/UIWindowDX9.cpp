#include "Nave2D.h"
#include "UIWindowDX9.h"
#include <Nave/NFDeltaTimer.h>

using namespace NaveFramework;

namespace Nave2D { 

	UIWindowDX9::UIWindowDX9(void)
	{
		m_pVideo = new NFVideoDX9();
		m_pInputManager = new NFInputManager();
		m_pFontManager = new NFFontManager();
		m_pTextureManager = new NFTextureManager();
		m_pImageManager = new NFImageManager();
		m_pDialogManager = new UIDialogManager();
	}

	UIWindowDX9::~UIWindowDX9(void)
	{
		_DELETE(m_pDialogManager);
		_DELETE(m_pImageManager);
		_DELETE(m_pTextureManager);

		_DELETE(m_pFontManager);
		_DELETE(m_pInputManager);
		_DELETE(m_pVideo);
	}

	VOID UIWindowDX9::OnUpdate(float dt)
	{
		UpdateMsg();

		Update(dt);
	}

	VOID UIWindowDX9::OnRender()
	{
		if(NFVideoDX9::GetInstance().IsLoadDevice())
		{
			NFVideoDX9::GetInstance().ResizeD3D();
			return;
		}

		// 전체모드로 바꾸기
		if(NFVideoDX9::GetInstance().TestCooperativeLevel()==E_FAIL)
		{
			RestoredWindows();
		}

		// 3D 그리기 시작
		LPD3DDEVICE lp3dDevice = NFVideoDX9::GetInstance().RenderStart();

		NFVideoDX9::GetInstance().ClearScreen(D3DCOLOR_XRGB(0,0,255));

		RenderObject();

		// 일반 UI를 출력 UI매니져에서 렌더링 걸게 수정
		Render();

		// 일반 UI보다 상위에 위치할 것.
		RenderTopmost();

		// 3D 그리기 종료
		NFVideoDX9::GetInstance().RenderEnd();

		NFVideoDX9::GetInstance().Present();
	}

	HRESULT UIWindowDX9::MinimizedWindows()
	{
		return UIWindow::MinimizedWindows();
	}

	HRESULT UIWindowDX9::RestoredWindows()
	{
		RestoredObject();

		HRESULT hr;
		// 전체모드로 바꾸기
		if( FAILED( hr = NFVideoDX9::GetInstance().Reset() ) )
			return hr;

		return UIWindow::RestoredWindows();
	}

	LRESULT UIWindowDX9::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		// UIDialogManager의 MsgProc를 호출한다.
		MessageProc(uMsg, wParam, lParam);

		return UIWindow::MsgProc(hWnd, uMsg, wParam, lParam);
	}

	VOID UIWindowDX9::ChangeWindowMode(BOOL bWindowsMode)
	{
		m_bReady = FALSE;

		m_bWindowMode = bWindowsMode;

		// 전체모드로 바꾸기
		NFVideoDX9::GetInstance().ChangeVideo(bWindowsMode);

		ReSizeWindow(bWindowsMode, m_iWidth, m_iHeight);

		m_bReady = TRUE;
	}

	///////////////////////////////////////////////////////////////////////////////
	//	Descrip	: 윈도우를 생성한다.
	//	Return	: 성공. 실패..
	//	Date	: 2001-04-11오전 10:41:50
	///////////////////////////////////////////////////////////////////////////////
	BOOL UIWindowDX9::CreateWindows(HINSTANCE hInstance, int nCmdShow, LPCWSTR Title, int Width, int Height, BOOL PopupMode, BOOL WindowMode, LPCTSTR Icon)
	{
		// 윈도우의 초기화 이걸 안해주면 윈도우가 생성되지 않는다.
		InitWindowsObject(hInstance, Title, Icon);

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
			Nave::hMainWnd = CreateWindowsEx(Title, 0, dwStyle, 0, 0, Width, Height);
		}
		else
		{
			dwStyle=WS_POPUP|WS_MINIMIZEBOX|WS_SYSMENU;	// Windows Style
			// 이 윈도우는 윈도우 기본 생성 함수이다.
			Nave::hMainWnd = CreateWindowsEx(Title, WS_EX_TOPMOST, dwStyle, 0, 0, Width, Height);
		}

		m_hWnd = Nave::hMainWnd;
		   
		OnSetWnd(Nave::hMainWnd);

		if(!Nave::hMainWnd)
		{
			InfoMessage(L"윈도우 생성 에러");
			return FALSE;
		}

		ReSizeWindow(m_bWindowMode, m_iWidth, m_iHeight);
		
		// 아이콘 로드 (작은 것 따로 하나)
		HICON hIcon = LoadIcon( hInstance, Icon );
		SendMessage( Nave::hMainWnd, WM_SETICON, (WPARAM)ICON_BIG,   (LPARAM)hIcon );
		SendMessage( Nave::hMainWnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon );

		if(NFVideoDX9::GetInstance().InitVideo(Nave::hMainWnd, m_iWidth, m_iHeight, m_bWindowMode) != S_OK)
		{
			InfoMessage(L"DirectX 초기화에 실패하였습니다.");
			return FALSE;
		}

		BOOL Val = TRUE;
		if( FAILED( InitObjects() ) )
			Val = FALSE;

		ShowWindow(Nave::hMainWnd, SW_SHOW);

		return Val;
	}

	HRESULT UIWindowDX9::StartProcess(BOOL bWindow)
	{
		m_bReady = TRUE;
		m_bActive = TRUE;

		Nave::NFDeltaTimer<float> timer;
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

		m_bReady = FALSE;
		m_bActive = FALSE;

		// 클라이언트 오브젝트를 삭제한다.
		ReleaseObjects();

		NFVideoDX9::GetInstance().Release();

		return S_OK;
	}

	HRESULT UIWindowDX9::EndProcess()
	{
		m_Exit = TRUE;
		m_bActive = FALSE;
		m_bReady = FALSE;
		return S_OK;
	}
}