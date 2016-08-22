///////////////////////////////////////////////////////////////////////////////
//	Filename:	UIPublic.cpp
//	Coder	:	Edith (Edithe@chollian.net) 
//	Comp.	:	EDITH
//	Compiler:	Visual C++ 6.0
//	Date	:	2003-05-01오후 2:16:45
//	Title	:	UIBase용 공용 변수 및 함수 정의
///////////////////////////////////////////////////////////////////////////////
#include "UIPublic.h"
#include <Nave/NFStringUtil.h>

namespace NaveTest {

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	HWND	hMainWnd = NULL;	// 메인 윈도우.
	HWND	hEditWnd = NULL;	// 에디트,
	HWND	hListWnd = NULL;	// 리스트
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
	// 한글의 첫번째 바이트면 1 두번째바이트면 2 영문자 이면 0을 반환한다.
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

		// 랜덤 초기화..
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
	//		Error Message 출력
	//
	//	pchMessage	:	에러가 발생 했을 경우 출력 메시지(인자를 포함함)
	//
	//////////////////////////////////////////////////////////////////////
	INT ErrorMessage(WCHAR *pchMessage, ...)
	{
		WCHAR	chBuffer[256];
		va_list va;

		va_start(va, pchMessage);
		_vstprintf(chBuffer, pchMessage, va);
		va_end(va);
			
		MessageBox(GetActiveWindow(), chBuffer, L"에러 메시지", MB_ICONWARNING | MB_OK);
		PostQuitMessage(0);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////
	//
	//	Message(WCHAR *pchMessage, ...)
	//
	//		Message를 출력 한다.
	//
	//	pchMessage	:	출력 메시지(인자를 포함함)
	//
	//////////////////////////////////////////////////////////////////////
	INT Message(WCHAR *pchMessage, ...)
	{
		WCHAR	chBuffer[256];
		va_list va;

		va_start(va, pchMessage);
		_vstprintf(chBuffer, pchMessage, va);
		va_end(va);

		return MessageBox(GetActiveWindow(), chBuffer, L"확인 메시지", MB_ICONINFORMATION | MB_OK);
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