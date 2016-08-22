/** 
 *  @file  		CmdMsgView.h
 *  @brief 		UI�������� MsgView ���
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-11
 */
#pragma once

namespace NaveTest {

	/// Info �������� ���� ũ��
	#define INFO_HEIGHT		180
	/// Info �������� ���̵�
	#define ID_INFO			201
	/// Message�� ���̵�
	#define ID_MSGVIEW		202

	/// Info ������ �ڵ�
	extern HWND g_hInfoWnd; 
	/// Message ������ �ڵ�
	extern HWND g_hListWnd;	

	/// Message �� Info �����츦 �ʱ�ȭ�Ѵ�.
	HWND InitLogView( HWND hParent, HFONT hFont );
	/// Message �����츦 Ŭ�����Ѵ�.
	VOID ClearMsgView( void );

	/// Info ������ �ش� �ؽ�Ʈ�� �ʱ�ȭ�Ѵ�.
	VOID UpdateInfo(WCHAR* strInfo);

	/// Message �� Info �������� ũ�⸦ �����Ѵ�.
	VOID ResizeLogView( HWND hParent );

	/// Message ������ Ű �̺�Ʈ
	LRESULT OnVkeyToItem( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

}