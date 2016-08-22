#include "UIPublic.h"
#include "CmdEdit.h"
#include "CmdMsgView.h"

namespace NaveTest {

	HWND g_hEditWnd = NULL;	// 에디트,
	static CHAR EditStr[MAX_EDITSTRING];
	static WNDPROC OldEditProc;

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	LRESULT OnEditDefault( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		return CallWindowProc( OldEditProc, hWnd, uMsg, wParam, lParam );
	}

	LRESULT OnEditKeyDown( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch( wParam ) 
		{
		case VK_TAB:
			// ListView에 포커스 넘김
			SetFocus(g_hListWnd);
			return FALSE;
		}
		return OnEditDefault(hWnd, uMsg, wParam, lParam);
	}

	LRESULT OnEditChar( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch( wParam ) 
		{
		case 13:	// EditBox에서 return을 했을때
			SendMessage( hWnd, WM_GETTEXT, MAX_EDITSTRING, (LPARAM)(LPSTR )EditStr);
			if(strlen(EditStr) == 0)
				break;
			// 입력한것에 대한 처리 셀을 선택
			SendMessage( hWnd, EM_SETSEL, 0, (LPARAM )-1 );
			// 부모에게 Return을 통지
			SendMessage( GetParent(hWnd), WM_EDIT_RETURN, strlen(EditStr), (LPARAM)(LPSTR )EditStr );
			return FALSE;
		}
		return OnEditDefault(hWnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK EditProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch( uMsg ) 
		{
		case WM_KEYDOWN:
			return OnEditKeyDown( hWnd, uMsg, wParam, lParam );
		case WM_CHAR :
			return OnEditChar( hWnd, uMsg, wParam, lParam );
		default :
			return OnEditDefault( hWnd, uMsg, wParam, lParam );
		}
	}

	HWND InitEdit( HWND hParent, HFONT hFont )
	{
		// 메인 윈도우로.. 처리
		ZeroMemory(EditStr, sizeof(EditStr));

		RECT rect;
		GetClientRect( hParent, &rect );

		g_hEditWnd = CreateWindowW( L"EDIT", L"",
			WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | WS_VISIBLE | WS_BORDER | ES_WANTRETURN | ES_AUTOHSCROLL,
			0, rect.bottom-EDIT_HEIGHT, rect.right, EDIT_HEIGHT,
			hParent, (HMENU )ID_EDIT,
			(HINSTANCE )GetWindowLongW( hParent, GWL_HINSTANCE ), NULL );

		ShowWindow( g_hEditWnd, SW_SHOW );
		UpdateWindow( g_hEditWnd );

		// 메인 폰트로 폰트를 셋팅한다.
		::SendMessage(g_hEditWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

		// 여기서 에디트 박스에 폰트를 셋팅하게 한다.

		OldEditProc = (WNDPROC )GetWindowLong( g_hEditWnd, GWL_WNDPROC );
		SetWindowLong( g_hEditWnd, GWL_WNDPROC, (LONG )EditProc );

		return g_hEditWnd;
	}

	VOID ResizeEdit( HWND hParent )
	{
		RECT r;
		GetClientRect( hParent, &r );

		MoveWindow( g_hEditWnd, 0, r.bottom-EDIT_HEIGHT, r.right, EDIT_HEIGHT, TRUE );
	}


}