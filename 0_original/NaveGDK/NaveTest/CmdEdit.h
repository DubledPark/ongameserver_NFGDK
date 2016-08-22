/** 
 *  @file  		CmdEdit.h
 *  @brief 		UI�������� Edit���
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-11
 */
#pragma once

namespace NaveTest {

	/// Edit �ڽ��� �ִ� ��ũ�� ũ��
	#define MAX_EDITSTRING	1024
	/// Edit �ڽ� �����̺�Ʈ �޽�����
	#define WM_EDIT_RETURN	(WM_USER+101)
	/// Edit �ڽ��� ���� ũ��
	#define EDIT_HEIGHT		16
	/// Edit �ڽ��� ���̵� ��.
	#define ID_EDIT			200

	// Edit �ڽ� �ڵ�
	extern HWND g_hEditWnd;	

	/// Edit �ڽ� �⺻ �̺�Ʈ
	LRESULT OnEditDefault( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	/// Edit �ڽ� KeyDown �̺�Ʈ
	LRESULT OnEditKeyDown( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	/// Edit �ڽ� Char �̺�Ʈ
	LRESULT OnEditChar( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	/// Edit �ڽ� �ݹ�
	LRESULT CALLBACK EditProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	/// Edit �ڽ� �ʱ�ȭ �Լ�
	HWND InitEdit( HWND hParent, HFONT hFont );
	/// Edit �ڽ� ũ�� ������ �Լ�
	VOID ResizeEdit( HWND hParent );

}