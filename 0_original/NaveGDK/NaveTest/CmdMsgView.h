/** 
 *  @file  		CmdMsgView.h
 *  @brief 		UI윈도우중 MsgView 모듈
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
 *  @date  		2009-05-11
 */
#pragma once

namespace NaveTest {

	/// Info 윈도우의 세로 크기
	#define INFO_HEIGHT		180
	/// Info 윈도우의 아이디
	#define ID_INFO			201
	/// Message의 아이디
	#define ID_MSGVIEW		202

	/// Info 윈도우 핸들
	extern HWND g_hInfoWnd; 
	/// Message 윈도우 핸들
	extern HWND g_hListWnd;	

	/// Message 및 Info 윈도우를 초기화한다.
	HWND InitLogView( HWND hParent, HFONT hFont );
	/// Message 윈도우를 클리어한다.
	VOID ClearMsgView( void );

	/// Info 정보를 해당 텍스트로 초기화한다.
	VOID UpdateInfo(WCHAR* strInfo);

	/// Message 및 Info 윈도우의 크기를 조정한다.
	VOID ResizeLogView( HWND hParent );

	/// Message 윈도우 키 이벤트
	LRESULT OnVkeyToItem( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

}