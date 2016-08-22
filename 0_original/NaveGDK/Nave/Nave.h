/** 
 *  @file		Nave.h
 *  @brief		Nave Library Core Header
 *  @remarks	NaveGDK 라이브러리의 선언부 Header
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

/// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN		

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"imm32.lib")

#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <WCHAR.h>
#include <stddef.h>	
#include <conio.h>

#include "NFTypes.h"
#include "NFObject.h"
#include "NFDeltaTimer.h"
#include "NFSingleton.h"
#include "NFSmartPtr.h"
#include "NFException.h"

/// 문자열 앞에 L을 연결하여 WCHAR 타입으로 변환함
#define WIDEN2(x) L ## x
/// CHAR의 값을 WCHAR로 변환하기 위한 정의
#define WIDEN(x) WIDEN2(x)
/// __FILE__ 의 WCHAR형 정의
#define __WFILE__ WIDEN(__FILE__)
/// __FUNCSIG__ 의 WCHAR형 정의
#define __WFUNCSIG__ WIDEN(__FUNCSIG__)

/// 단일 객체 DELETE 디파인
#ifndef _DELETE
#define _DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif
/// 배열형 객체 DELETE 디파인
#ifndef _DELETE_ARRAY
#define _DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
/// RELEASE 디파인
#ifndef _RELEASE
#define _RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

/// MAKEINTRESOURCE 재정의
#define MIS(a)					MAKEINTRESOURCEW(a)
/// InvalidateRect 재정의
#define Redraw(hWnd, bErase)	InvalidateRect(hWnd, NULL, bErase)

/// DWORD 에서 Alpha 구하기
#define GetAColor(Color)      ((BYTE)((Color)>>24)&0xff)
/// DWORD 에서 Red 구하기
#define GetRColor(Color)      ((BYTE)((Color)>>16)&0xff)
/// DWORD 에서 Green 구하기
#define GetGColor(Color)      ((BYTE)(((WORD)(Color)) >> 8)&0xff)
/// DWORD 에서 Blue 구하기
#define GetBColor(Color)      ((BYTE)(Color)&0xff)

namespace Nave {

	/// 메인 윈도우 핸들
	extern HWND				hMainWnd;

	/// 윈도으의 인스턴스
	extern HINSTANCE		hInstance;

	/**
	 * @brief	LFH 메모리풀을 사용한다
	 * @remarks	
	 *		Low-Fragmentation Heap의 사용											\r\n
	 *		프로그램이 시작할때 한번호출해준다.										\r\n
	 *		디버거가 연결되면(F5) 디버그 힙을 사용하므로 CTRL+F5를 눌러야			\r\n
	 *		Low-Fragmentation Heap이 사용되고, TRUE가 리턴되는것을 알수있다.		\r\n
	 *																				\r\n
	 *		Low-Fragmentation Heap의 장점											\r\n
	 *		특별히 프로그래머는 신경쓰지 않아도, 메모리풀을 사용,					\r\n	
	 *		프로그램의 질을 높일수있다.												\r\n
	 *		메모리 단편화를 예방할수있고, 힙메모리생성, 삭제시 효율이 높아진다.		\r\n
	 *		특히, 멀티코어 시스템에서 높은 성능향상이 있다.							\r\n
	 *																				\r\n
	 *		Low-Fragmentation Heap사용시 참고										\r\n
	 *		이 함수가 호출되지 않아도, 이코드가 있으면 Win2000Pro Sp4이상의			\r\n
	 *		OS에서만 동작하므로, Win98을 지원하려고 한다면, 주석화시켜야 한다.		\r\n
	 *																				\r\n
	 *		비스타는 이함수를 호출하지 않아도 기본동작한다.							\r\n
	 *		16K보다 작은 메모리를 할당하면, LFH에 메모리가 할당,					\r\n
	 *		그이상은 기존힙에서 할당된다.											\r\n
	 */
	void EnableLFH();

	/**
	 * @brief	프로그램의 국가언어 정보를 설정합니다.
	 * @param pLocale 언어명 
	 * @remarks	wprintf 등 유니코드 문자열을 콘솔에 출력하기 위해서는 로케일 설정을 해야함.	\r\n
	 *			한국어 : Korean																\r\n
	 *			영어 : English																\r\n
	 */
	void SetLocale(CHAR* pLocale);

	/**
	 * @brief	게임에서 사용할 Object의 GUID를 얻어온다	
	 * @return	생성된 GUID값 0x00000001 ~ 0xFFFFFFFF 까지
	 */
	DWORD	GetGUID();

	/**
	 * @brief	String의 크기Pixel을 구한다.
	 * @param string	사이즈를 구할 스트링
	 * @param font		사용할 폰트
	 * @return			얻어진 String의 크기
	 */
	SIZE	GetStringSize( const WCHAR* string, HFONT font );

	/**
	 * @brief	윈도우 커서를 가져온다.
	 * @return	Main윈도우에서의 마우스 포지션
	 * @warning	Nave::hMainWnd 가 설정되어있어야한다.
	 */
	POINT	GetMousePos();

	/**
	 * @brief	윈도우 커서를 가져온다.
	 * @param wnd	윈도우 핸들
	 * @return	윈도우에서의 마우스 포지션
	 */
	POINT	GetMousePos( HWND wnd );

	/**
	 * @brief	현재 마우스가 어느 윈도우에 올라가 있는지 
	 * @return	마우스가 존재하는 윈도우의 핸들
	 */
	HWND	IsWindowFromPoint();

	/**
	 * @brief	현재 프로그램의 이름을 구한다
	 * @param buf	어플리케이션의 이름을 저장할 버퍼
	 * @param len	버퍼의 크기
	 * @return	어플리케이션 이름의 길이
	 */
	INT		GetProgramName( LPWSTR buf, INT len );	

	/**
	 * @brief	현재 프로그램의 경로를 구한다.
	 * @param buf	어플리케이션의 디렉토리를 저장할 버퍼
	 * @param len	버퍼의 크기
	 * @return	어플리케이션 디렉토리의 길이
	 */
	INT		GetProgramDir( LPWSTR buf, INT len );	

	/**
	 * @brief	Path에서 경로를 구한다
	 * @param path	경로 정보
	 * @param buf	경로에서 구한 디렉토리
	 * @return	디렉토리의 길이
	 */
	INT		GetDir( LPCWSTR path, LPWSTR buf );		


	/**
	 * @brief	문자열에서 1Line을 읽어들인다 한라인은 \n으로 구분된다.
	 * @param sBuf		소스 버퍼
	 * @param maxsBuf	소스 버퍼 크기
	 * @param tBuf		저장 버퍼
	 * @param maxtBuf	저장 버퍼 크기
	 * @param index		소스 버퍼에서 읽어들일 시작위치
	 * @return	성공여부
	 */
	BOOL	GetLine( const CHAR* sBuf, INT maxsBuf, CHAR* tBuf, INT maxtBuf, LONG& index );

	/**
	 * @brief	문자열을 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param tBuf	저장 버퍼
	 * @param len	저장 버퍼 크기
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 */
	VOID	GetString( const CHAR* sBuf, CHAR* tBuf, INT len, LONG& index );

	/**
	 * @brief	1바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	BYTE 결과값
	 */
	BYTE	GetByte( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	2바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	SHORT 결과값
	 */
	SHORT 	GetShort( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	4바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	INT 결과값
	 */
	INT		GetInt( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	4바이트 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	DWORD 결과값
	 */
	DWORD   GetDWORD( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	4바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	FLOAT 결과값
	 */
	FLOAT	GetFloat( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	8바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	DOUBLE 결과값
	 */
	DOUBLE	GetDouble( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	문자열을 셋팅한다
	 * @param tBuf	저장 버퍼
	 * @param sBuf	저장할 값
	 * @param len	저장할 길이
	 * @param index 저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetString( CHAR* tBuf, const CHAR* sBuf, INT len, LONG& index );

	/**
	 * @brief	1바이트 BYTE를 셋팅한다.
	 * @param tBuf	저장 버퍼
	 * @param sByte 저장할 값
	 * @param index 저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetByte( CHAR* tBuf, BYTE sByte, LONG& index );

	/**
	 * @brief	2바이트 SHORT를 셋팅한다.
	 * @param tBuf		저장 버퍼
	 * @param sShort	저장할 값
	 * @param index		저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetShort( CHAR* tBuf, SHORT sShort, LONG& index );

	/**
	 * @brief	4바이트 INT를 셋팅한다.
	 * @param tBuf	저장 버퍼
	 * @param sInt	저장할 값
	 * @param index 저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetInt( CHAR* tBuf, INT sInt, LONG& index);

	/**
	 * @brief	4바이트 DWORD를 셋팅한다.
	 * @param tBuf		저장 버퍼
	 * @param sDword	저장할 값
	 * @param index		저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetDWORD( CHAR* tBuf, DWORD sDword, LONG& index );

	/**
	 * @brief	4바이트 FLOAT를 셋팅한다.
	 * @param tBuf		저장 버퍼
	 * @param sFloat	저장할 값
	 * @param index		저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetFloat( CHAR* tBuf, FLOAT sFloat, LONG& index );

	/**
	 * @brief	8바이트 double를 셋팅한다.
	 * @param tBuf		저장 버퍼
	 * @param sDouble	저장할 값
	 * @param index		저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetDouble( CHAR* tBuf, DOUBLE sDouble, LONG& index );


	/**
	 * @brief	문자열에서 1Line을 읽어들인다 한라인은 \n으로 구분된다.
	 * @param sBuf		소스 버퍼
	 * @param maxsBuf	소스 버퍼 크기
	 * @param tBuf		저장 버퍼
	 * @param maxtBuf	저장 버퍼 크기
	 * @param index		소스 버퍼에서 읽어들일 시작위치
	 * @return	성공여부
	 */
	BOOL	GetLine( const WCHAR* sBuf, INT maxsBuf, WCHAR* tBuf, INT maxtBuf, LONG& index );

	/**
	 * @brief	문자열을 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param tBuf	저장 버퍼
	 * @param len	저장 버퍼 크기
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 */
	VOID	GetString( const WCHAR* sBuf, WCHAR* tBuf, INT len, LONG& index );

	/**
	 * @brief	1바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	BYTE 결과값
	 */
	BYTE	GetByte( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	2바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	SHORT 결과값
	 */
	SHORT 	GetShort( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	4바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	INT 결과값
	 */
	INT		GetInt( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	4바이트 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	DWORD 결과값
	 */
	DWORD   GetDWORD( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	4바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	FLOAT 결과값
	 */
	FLOAT	GetFloat( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	8바이트를 읽어들인다.
	 * @param sBuf	소스 버퍼
	 * @param index 소스 버퍼에서 읽어들일 시작위치
	 * @return	DOUBLE 결과값
	 */
	DOUBLE	GetDouble( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	문자열을 셋팅한다
	 * @param tBuf	저장 버퍼
	 * @param sBuf	저장할 값
	 * @param len	저장할 길이
	 * @param index 저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetString( WCHAR* tBuf, const WCHAR* sBuf, INT len, LONG& index );

	/**
	 * @brief	1바이트 BYTE를 셋팅한다.
	 * @param tBuf	저장 버퍼
	 * @param sByte 저장할 값
	 * @param index 저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetByte( WCHAR* tBuf, BYTE sByte, LONG& index );

	/**
	 * @brief	2바이트 SHORT를 셋팅한다.
	 * @param tBuf		저장 버퍼
	 * @param sShort	저장할 값
	 * @param index		저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetShort( WCHAR* tBuf, SHORT sShort, LONG& index );

	/**
	 * @brief	4바이트 INT를 셋팅한다.
	 * @param tBuf	저장 버퍼
	 * @param sInt	저장할 값
	 * @param index 저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetInt( WCHAR* tBuf, INT sInt, LONG& index);

	/**
	 * @brief	4바이트 DWORD를 셋팅한다.
	 * @param tBuf		저장 버퍼
	 * @param sDword	저장할 값
	 * @param index		저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetDWORD( WCHAR* tBuf, DWORD sDword, LONG& index );

	/**
	 * @brief	4바이트 FLOAT를 셋팅한다.
	 * @param tBuf		저장 버퍼
	 * @param sFloat	저장할 값
	 * @param index		저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetFloat( WCHAR* tBuf, FLOAT sFloat, LONG& index );

	/**
	 * @brief	8바이트 double를 셋팅한다.
	 * @param tBuf		저장 버퍼
	 * @param sDouble	저장할 값
	 * @param index		저장 버퍼에서 저장될 시작위치
	 */
	VOID	SetDouble( WCHAR* tBuf, DOUBLE sDouble, LONG& index );


	/**
	 * @brief	바이너리 정보를 Read 들입니다.
	 * @param sBuf	소스 버퍼
	 * @param tBuf	저장 버퍼
	 * @param len	읽을 길이
	 * @param seek	소스 버퍼에서 읽을 시작위치
	 */
	VOID	Read( const char* sBuf, void* tBuf, INT len, LONG& seek );

	/**
	 * @brief	바이너리 정보를 Write 합니다.
	 * @param sBuf	저장 버퍼
	 * @param tBuf	저장할 값
	 * @param len	저장할 길이
	 * @param seek	저장 버퍼에서 저장될 시작위치
	 */
	VOID	Write( const void* sBuf, char* tBuf, INT len, LONG& seek );

}