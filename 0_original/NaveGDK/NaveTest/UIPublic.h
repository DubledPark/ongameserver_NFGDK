/** 
 *  @file  		UIPublic.h
 *  @brief 		기본 정의
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
 *  @date  		2009-05-11
 */

#pragma once

#define WIN32_LEAN_AND_MEAN										// Exclude rarely-used stuff from Windows headers

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib") // 커먼컨트럴용 

#include <Nave/Nave.h>

#include <commctrl.h>
#include <process.h>    /* _beginthread, _endthread */

#include "CmdEdit.h"
#include "CmdMsgView.h"

#define MAX_BUFSIZE		2048

#define InitRandom()	srand((UINT)timeGetTime())
#define Random(V)		(rand() % (V))


namespace NaveTest {

	/// 메인 윈도우.
	extern HWND	hMainWnd;	
	/// 에디트
	extern HWND	hEditWnd;	
	/// 리스트
	extern HWND	hListWnd;	
	/// 인스턴스 핸들
	extern HINSTANCE hInst;
	/// 메인 폰트 
	extern HFONT hMainFont;
	/// 윈도우 이름
	extern WCHAR  AppName[256];


	/// Global 오브젝트 초기화
	VOID InitGlobalObject();

	/// Global 오브젝트 삭제
	VOID ReleaseGlobalObject();

	/// 프로그램 폴더 얻어오기
	INT GetProgramDir( HINSTANCE hinst, WCHAR *buf, INT len );

	/// 리스트,에디트 윈도우를 재배치 시킨다.
	VOID ResizeWindows( VOID ) ;

	/// OutText의 Len째 글자가 한글인지 아닌지 확인한다. 
	INT		IsHangul(WCHAR* OutText,INT Len);

	/// String의 Width를 구한다.
	INT		GetStringWidth(WCHAR* String, HFONT hFont);

	/// Font를 생성한다.
	HFONT	CreateFont(INT FontSize, LPCTSTR lpszFontName, INT FontWeight = 0);

	/// 에러 메시지 박스를 띄운다.
	INT ErrorMessage(WCHAR *pchMessage, ...);

	/// 메시지 박스를 띄운다.
	INT Message(WCHAR *pchMessage, ...);
}


