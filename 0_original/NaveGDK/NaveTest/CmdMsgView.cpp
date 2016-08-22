#include "UIPublic.h"
#include "CmdMsgView.h"
#include "CmdEdit.h"
//#include "Richedit.h"

namespace NaveTest {

	HWND g_hInfoWnd = NULL; // 리스트
	HWND g_hListWnd = NULL;	// 리스트

/*
		Len = (int)SendMessage( g_hListWnd, LB_GETCOUNT, 0, 0L );
		while( Len > MAX_MSGLIST )
			Len = (int)SendMessage( g_hListWnd, LB_DELETESTRING, 0, 0L );

		// 전체 카운드
		int Top = (int)SendMessage( g_hListWnd, LB_GETTOPINDEX, 0, 0L );
		int ItemHeight = (int)SendMessage( g_hListWnd, LB_GETITEMHEIGHT, 0, 0L );
		RECT rect;
		GetClientRect(g_hListWnd, &rect);
		int PageCnt = abs(rect.bottom-rect.top)/ItemHeight;
		int Total = abs(Len-Top);

		if(Top+PageCnt+1 == Len)
			SendMessage( g_hListWnd, LB_SETTOPINDEX, Top+1, 0L );
*/
//	HINSTANCE g_hInstRichEdit;

	
	LRESULT OnVkeyToItem( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
	{
		if( (HWND )lParam == g_hListWnd ) 
		{
			if(LOWORD( wParam ) == VK_TAB )
				SetFocus( GetParent(g_hListWnd) );
		}
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	VOID ClearMsgView( void )
	{
		SendMessage(g_hListWnd, LB_RESETCONTENT, 0, 0);
	}

	VOID UpdateInfo(WCHAR* strInfo)
	{
		SendMessage(g_hInfoWnd, WM_SETTEXT, MAX_EDITSTRING, (LPARAM)strInfo);
	}

	HWND InitLogView( HWND hParent, HFONT hFont ) 
	{
		RECT rect;
		GetClientRect( hParent, &rect );

		g_hInfoWnd = CreateWindowW( L"EDIT", L"Infomation",
			WS_CHILD | WS_CLIPSIBLINGS | WS_TABSTOP | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL,
			0, 0, rect.right, INFO_HEIGHT,
			hParent, (HMENU )ID_INFO,
			(HINSTANCE )GetWindowLongW( hParent, GWL_HINSTANCE ), NULL );

		ShowWindow( g_hInfoWnd, SW_SHOW );
		UpdateWindow( g_hInfoWnd );

		// 메인 폰트로 폰트를 셋팅한다.
		::SendMessage(g_hInfoWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));


		g_hListWnd = CreateWindowW( L"LISTBOX", L"Message List",
			WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_BORDER | WS_VSCROLL
			| LBS_DISABLENOSCROLL | LBS_NOINTEGRALHEIGHT | LBS_WANTKEYBOARDINPUT | LBS_USETABSTOPS,
			0, INFO_HEIGHT, rect.right, rect.bottom - EDIT_HEIGHT - INFO_HEIGHT,
			hParent, (HMENU )ID_MSGVIEW,
			(HINSTANCE )GetWindowLongW( hParent, GWL_HINSTANCE ),
			NULL );

		ShowWindow( g_hListWnd, SW_SHOW );
		UpdateWindow( g_hListWnd );

		// 메인 폰트로 폰트를 셋팅한다.
		::SendMessage(g_hListWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));
		return g_hListWnd;
	}

	VOID ResizeLogView( HWND hParent )
	{
		RECT r;
		GetClientRect( hParent, &r );

		MoveWindow( g_hInfoWnd, 0, 0, r.right, INFO_HEIGHT, TRUE );
		MoveWindow( g_hListWnd, 0, INFO_HEIGHT, r.right, r.bottom - EDIT_HEIGHT - INFO_HEIGHT, TRUE );
	}


}