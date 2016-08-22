/** 
 *  @file  		UIPublic.h
 *  @brief 		�⺻ ����
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-11
 */

#pragma once

#define WIN32_LEAN_AND_MEAN										// Exclude rarely-used stuff from Windows headers

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib") // Ŀ����Ʈ���� 

#include <Nave/Nave.h>

#include <commctrl.h>
#include <process.h>    /* _beginthread, _endthread */

#include "CmdEdit.h"
#include "CmdMsgView.h"

#define MAX_BUFSIZE		2048

#define InitRandom()	srand((UINT)timeGetTime())
#define Random(V)		(rand() % (V))


namespace NaveTest {

	/// ���� ������.
	extern HWND	hMainWnd;	
	/// ����Ʈ
	extern HWND	hEditWnd;	
	/// ����Ʈ
	extern HWND	hListWnd;	
	/// �ν��Ͻ� �ڵ�
	extern HINSTANCE hInst;
	/// ���� ��Ʈ 
	extern HFONT hMainFont;
	/// ������ �̸�
	extern WCHAR  AppName[256];


	/// Global ������Ʈ �ʱ�ȭ
	VOID InitGlobalObject();

	/// Global ������Ʈ ����
	VOID ReleaseGlobalObject();

	/// ���α׷� ���� ������
	INT GetProgramDir( HINSTANCE hinst, WCHAR *buf, INT len );

	/// ����Ʈ,����Ʈ �����츦 ���ġ ��Ų��.
	VOID ResizeWindows( VOID ) ;

	/// OutText�� Len° ���ڰ� �ѱ����� �ƴ��� Ȯ���Ѵ�. 
	INT		IsHangul(WCHAR* OutText,INT Len);

	/// String�� Width�� ���Ѵ�.
	INT		GetStringWidth(WCHAR* String, HFONT hFont);

	/// Font�� �����Ѵ�.
	HFONT	CreateFont(INT FontSize, LPCTSTR lpszFontName, INT FontWeight = 0);

	/// ���� �޽��� �ڽ��� ����.
	INT ErrorMessage(WCHAR *pchMessage, ...);

	/// �޽��� �ڽ��� ����.
	INT Message(WCHAR *pchMessage, ...);
}


