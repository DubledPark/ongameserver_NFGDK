///////////////////////////////////////////////////////////////////////////////
//	Filename:	UIPublic.cpp
//	Coder	:	Edith (Edithe@chollian.net) 
//	Comp.	:	EDITH
//	Compiler:	Visual C++ 6.0
//	Date	:	2003-05-01���� 2:16:45
//	Title	:	UIBase�� ���� ���� �� �Լ� ����
///////////////////////////////////////////////////////////////////////////////
#include "UIPublic.h"
#include <Nave/NFStringUtil.h>

namespace NaveTest {

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	HWND	hMainWnd = NULL;	// ���� ������.
	HWND	hEditWnd = NULL;	// ����Ʈ,
	HWND	hListWnd = NULL;	// ����Ʈ
	HINSTANCE hInst = NULL;
	HFONT	hMainFont;
	WCHAR	AppName[256];

	VOID ResizeWindows( VOID ) 
	{
		RECT r;

		GetClientRect( hMainWnd, &r );
		MoveWindow( hListWnd, 0, 0, r.right, r.bottom - EDIT_HEIGHT, TRUE );
		MoveWindow( hEditWnd, 0, r.bottom - EDIT_HEIGHT, r.right, r.bottom, TRUE );
	}

	//------------------------------------------------------------------------------
	// �ѱ��� ù��° ����Ʈ�� 1 �ι�°����Ʈ�� 2 ������ �̸� 0�� ��ȯ�Ѵ�.
	//------------------------------------------------------------------------------
	INT IsHangul(WCHAR* OutText, INT Len)
	{
		if((INT)wcslen(OutText) < Len)
			return -1;

		enum TCodeType { ctError = -1, ctEng, ctFirst, ctLast };
		INT i;
		TCodeType type = ctEng;

		for(i=0; OutText[i] != L'\0'; i++)
		{
			if(type == ctFirst)
				type = ctLast;
			else
			{
				if(OutText[i] & 0x80)
					type = ctFirst;
				else type = ctEng;
			}
			if(i == Len) 
				return type;
		}
		return -1;
	}

	INT GetStringWidth(WCHAR* String, HFONT hFont)
	{
		HDC hDC = ::GetDC(hMainWnd);

		SIZE size;
		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

		GetTextExtentPoint32(hDC, String, wcslen(String), &size);

		SelectObject(hDC, hOldFont);
		::ReleaseDC(hMainWnd, hDC);

		return size.cx;
	}

	HFONT CreateFont(INT FontSize, LPCTSTR lpszFontName, INT FontWeight)
	{
		HFONT Font = ::CreateFont(FontSize, 0, 0, 0, FontWeight, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH|FF_ROMAN, lpszFontName);
		return Font;
	}

	VOID InitGlobalObject()
	{
		hMainFont = NULL;
		hMainFont = CreateFont(16, L"Fixedsys");

		// ���� �ʱ�ȭ..
		srand( timeGetTime() );
	}

	VOID ReleaseGlobalObject()
	{
		if(hMainFont)
			DeleteObject(hMainFont);
		hMainFont = NULL;
	}



	//////////////////////////////////////////////////////////////////////
	//
	//	ErrorMessage(WCHAR *pchMessage, ...)
	//
	//		Error Message ���
	//
	//	pchMessage	:	������ �߻� ���� ��� ��� �޽���(���ڸ� ������)
	//
	//////////////////////////////////////////////////////////////////////
	INT ErrorMessage(WCHAR *pchMessage, ...)
	{
		WCHAR	chBuffer[256];
		va_list va;

		va_start(va, pchMessage);
		_vstprintf(chBuffer, pchMessage, va);
		va_end(va);
			
		MessageBox(GetActiveWindow(), chBuffer, L"���� �޽���", MB_ICONWARNING | MB_OK);
		PostQuitMessage(0);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////
	//
	//	Message(WCHAR *pchMessage, ...)
	//
	//		Message�� ��� �Ѵ�.
	//
	//	pchMessage	:	��� �޽���(���ڸ� ������)
	//
	//////////////////////////////////////////////////////////////////////
	INT Message(WCHAR *pchMessage, ...)
	{
		WCHAR	chBuffer[256];
		va_list va;

		va_start(va, pchMessage);
		_vstprintf(chBuffer, pchMessage, va);
		va_end(va);

		return MessageBox(GetActiveWindow(), chBuffer, L"Ȯ�� �޽���", MB_ICONINFORMATION | MB_OK);
	}

	INT GetProgramDir( HINSTANCE hinst, WCHAR *buf, INT len ) 
	{
		WCHAR *ProgDir;
		INT result;

		result = GetModuleFileName( hinst, buf, len - 1 );
		if( result == 0 )
			return 0;

		WCHAR pgmPtr[1024];
		wcscpy(pgmPtr, Nave::ToUnicode(_pgmptr).c_str());

		buf[len-1] = 0;
		wcscpy( buf, pgmPtr );
		ProgDir = buf;
		ProgDir += wcslen( pgmPtr ) - 1;
		while( *ProgDir != L'\\' && ProgDir != pgmPtr )
			ProgDir--;

		*ProgDir = 0;
		return (INT)wcslen( buf );
	}


}