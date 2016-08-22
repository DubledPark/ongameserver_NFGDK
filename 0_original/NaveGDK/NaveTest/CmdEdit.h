/** 
 *  @file  		CmdEdit.h
 *  @brief 		UI윈도우의 Edit모듈
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
 *  @date  		2009-05-11
 */
#pragma once

namespace NaveTest {

	/// Edit 박스의 최대 스크링 크기
	#define MAX_EDITSTRING	1024
	/// Edit 박스 리턴이벤트 메시지값
	#define WM_EDIT_RETURN	(WM_USER+101)
	/// Edit 박스의 세로 크기
	#define EDIT_HEIGHT		16
	/// Edit 박스의 아이디 값.
	#define ID_EDIT			200

	// Edit 박스 핸들
	extern HWND g_hEditWnd;	

	/// Edit 박스 기본 이벤트
	LRESULT OnEditDefault( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	/// Edit 박스 KeyDown 이벤트
	LRESULT OnEditKeyDown( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	/// Edit 박스 Char 이벤트
	LRESULT OnEditChar( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	/// Edit 박스 콜백
	LRESULT CALLBACK EditProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	/// Edit 박스 초기화 함수
	HWND InitEdit( HWND hParent, HFONT hFont );
	/// Edit 박스 크기 재조정 함수
	VOID ResizeEdit( HWND hParent );

}