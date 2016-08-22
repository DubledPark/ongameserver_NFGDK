#include "NaveServer.h"
#include "UIFramework.h"

namespace NaveServer {

	//////////////////////////////////////
	UIConsol::UIConsol()
	{
		m_bCommandExit = FALSE;
	}

	UIConsol::~UIConsol()
	{
	}

	BOOL UIConsol::Init()
	{
		Nave::NFLog::SetLogHandel(UICmdMsgView::s_hListWnd);
//		Nave::NFLog::EnableLogFile(true);

		// ������Ʈ �ʱ�ȭ
		InitObject();

		// Ŀ��� �ʱ�ȭ.
		InitializeCommand();

		m_bCommandExit = FALSE;
		return TRUE;
	}

	VOID UIConsol::StartCommand()
	{
		wprintf(L"\n");
		WCHAR Command[DEF_BUFSIZE];
		while(!m_bCommandExit)
		{
			_getws(Command);

			DoCommand(Command);
		}

		ReleaseObject();
	}

	VOID UIConsol::EndCommand()
	{
		m_bCommandExit = TRUE;
	}

	static UIWindow* g_pMain = NULL;
	///////////////////////////////////////////////////////////////////////////////
	//	Descrip	: ���״�� �������. Proc..
	//	Date	: 2001-04-11���� 10:41:02
	///////////////////////////////////////////////////////////////////////////////
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(g_pMain)
			return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	UIWindow::UIWindow()
	{
		m_bCommandExit = FALSE;
		m_hFont = NULL;
		g_pMain = this;
	}

	UIWindow::~UIWindow()
	{
		g_pMain = NULL;

		if(m_hFont)
			DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	VOID UIWindow::OnEditReturn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WCHAR buff[MAX_EDITSTRING];
		ZeroMemory(buff, sizeof(buff));
		CopyMemory( buff, (WCHAR*)lParam, sizeof(buff) );

		// ���⼭ �Էµ� ���ڿ��� ������ �м��ؼ� �Է��� ���� ���� ó���Ѵ�.
		DoCommand(buff);
	}

	VOID UIWindow::ResizeWindows( VOID ) 
	{
		UICmdEdit::ResizeEdit(m_hMainWnd);
		UICmdMsgView::ResizeLog(m_hMainWnd);
	}

	///////////////////////////////////////////////////////////////////////////////
	//	Descrip	: ���״�� ����� MsgProc , WindowProc���� ȣ��ȴ�.
	//	Date	: 2001-04-11���� 10:41:02
	///////////////////////////////////////////////////////////////////////////////
	LRESULT UIWindow::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch(uMsg)
		{
		case WM_SETFOCUS:
			SetFocus( UICmdEdit::s_hEditWnd );
			break;

		case WM_ERASEBKGND:
			return FALSE;
			
		case WM_SIZE:
			ResizeWindows();
			return FALSE;
		
		case WM_COMMAND:
			OnCommand(hWnd, LOWORD(wParam), HIWORD(wParam), lParam);
			return FALSE;

		// ListView Ű���� �Է�.
		case WM_VKEYTOITEM :
			return UICmdMsgView::OnVkeyToItem(hWnd, uMsg, wParam, lParam);

		// EditBox ���� �Է�.
		case WM_EDIT_RETURN:
			OnEditReturn(hWnd, uMsg, wParam, lParam);
			return FALSE;

		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			EndCommand();
			return FALSE;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	BOOL UIWindow::Init(HINSTANCE hInstance, int nCmdShow, int iWidth, int iHeight, WCHAR* AppName, WCHAR* Icon)
	{
		// �������� ����
		WNDCLASSEX wcex;
		wcex.cbSize			= sizeof(WNDCLASSEX); 
		wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.lpfnWndProc	= (WNDPROC)WindowProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 16;
		wcex.hInstance		= (HINSTANCE)hInstance;
		wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)Icon);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= NULL;
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= AppName;
		wcex.hIconSm		= LoadIcon(wcex.hInstance, Icon);
		// �������.
		RegisterClassExW(&wcex);

		if(iHeight < INFO_HEIGHT+150)
		{
			iHeight = INFO_HEIGHT+150;
		}

		// ������ ����
		m_hMainWnd = CreateWindowExW(WS_EX_WINDOWEDGE, AppName, AppName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, iWidth, iHeight, 
		NULL, NULL, hInstance, NULL);

		if(!m_hMainWnd)
		{
			MessageBoxW(NULL, L"������ ���� ����", L"Error", MB_OK);
			return FALSE;
		}

		m_hFont = ::CreateFontW(12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH|FF_ROMAN, L"����ü");

		UICmdEdit::InitEditProc(m_hMainWnd, m_hFont);
		UICmdMsgView::InitMsgView(m_hMainWnd, m_hFont);

		Nave::NFLog::SetLogHandel(UICmdMsgView::s_hListWnd);
//		Nave::NFLog::EnableLogFile(true);

		ShowWindow(nCmdShow);					// Show The UIWindow
		SetForegroundWindow(m_hMainWnd);					// Slightly Higher Priority
		// EditBox�� ��Ŀ�� �ֱ�
		SetFocus(UICmdEdit::s_hEditWnd);								// Sets Keyboard Focus To The UIWindow

		// ������ �ε� (���� �� ���� �ϳ�)
		HICON hIcon = LoadIcon( hInstance, (LPCTSTR)Icon );
		SendMessage( m_hMainWnd, WM_SETICON, (WPARAM)ICON_BIG,   (LPARAM)hIcon );
		SendMessage( m_hMainWnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon );

		// ������Ʈ �ʱ�ȭ
		InitObject();

		// Ŀ��� �ʱ�ȭ.
		InitializeCommand();

		m_bCommandExit = FALSE;
		return TRUE;
	}

	VOID UIWindow::ShowWindow(int nCmdShow)
	{
		::ShowWindow(m_hMainWnd, nCmdShow);					// Show The UIWindow
	}

	VOID UIWindow::StartCommand()
	{
		MSG		msg;

		while( !m_bCommandExit ) 
		{ 
			if( GetMessage( &msg, NULL, 0U, 0U ) ) 
			{ 
				TranslateMessage( &msg ); 
				DispatchMessage( &msg ); 
			} 
		}

		LOG_IMPORTANT((L"Programe Quitting"));

		ReleaseObject();

		InvalidateRect(m_hMainWnd, NULL, FALSE);

		// ���� �޽��� ǥ�� �� �ֱ� ���ؼ�
		for(int i = 0; i < 10; i++)
		{
			// ������ �޽����� ȭ�鿡 ����ϱ� ���ؼ�.
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}

		Nave::NFLog::CloseLog();

		Sleep(1000);
	}

	VOID UIWindow::EndCommand()
	{
		m_bCommandExit = TRUE;
		InvalidateRect(m_hMainWnd, NULL, FALSE);
	}



}