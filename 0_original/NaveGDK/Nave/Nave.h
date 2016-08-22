/** 
 *  @file		Nave.h
 *  @brief		Nave Library Core Header
 *  @remarks	NaveGDK ���̺귯���� ����� Header
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

/// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
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

/// ���ڿ� �տ� L�� �����Ͽ� WCHAR Ÿ������ ��ȯ��
#define WIDEN2(x) L ## x
/// CHAR�� ���� WCHAR�� ��ȯ�ϱ� ���� ����
#define WIDEN(x) WIDEN2(x)
/// __FILE__ �� WCHAR�� ����
#define __WFILE__ WIDEN(__FILE__)
/// __FUNCSIG__ �� WCHAR�� ����
#define __WFUNCSIG__ WIDEN(__FUNCSIG__)

/// ���� ��ü DELETE ������
#ifndef _DELETE
#define _DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif
/// �迭�� ��ü DELETE ������
#ifndef _DELETE_ARRAY
#define _DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
/// RELEASE ������
#ifndef _RELEASE
#define _RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

/// MAKEINTRESOURCE ������
#define MIS(a)					MAKEINTRESOURCEW(a)
/// InvalidateRect ������
#define Redraw(hWnd, bErase)	InvalidateRect(hWnd, NULL, bErase)

/// DWORD ���� Alpha ���ϱ�
#define GetAColor(Color)      ((BYTE)((Color)>>24)&0xff)
/// DWORD ���� Red ���ϱ�
#define GetRColor(Color)      ((BYTE)((Color)>>16)&0xff)
/// DWORD ���� Green ���ϱ�
#define GetGColor(Color)      ((BYTE)(((WORD)(Color)) >> 8)&0xff)
/// DWORD ���� Blue ���ϱ�
#define GetBColor(Color)      ((BYTE)(Color)&0xff)

namespace Nave {

	/// ���� ������ �ڵ�
	extern HWND				hMainWnd;

	/// �������� �ν��Ͻ�
	extern HINSTANCE		hInstance;

	/**
	 * @brief	LFH �޸�Ǯ�� ����Ѵ�
	 * @remarks	
	 *		Low-Fragmentation Heap�� ���											\r\n
	 *		���α׷��� �����Ҷ� �ѹ�ȣ�����ش�.										\r\n
	 *		����Ű� ����Ǹ�(F5) ����� ���� ����ϹǷ� CTRL+F5�� ������			\r\n
	 *		Low-Fragmentation Heap�� ���ǰ�, TRUE�� ���ϵǴ°��� �˼��ִ�.		\r\n
	 *																				\r\n
	 *		Low-Fragmentation Heap�� ����											\r\n
	 *		Ư���� ���α׷��Ӵ� �Ű澲�� �ʾƵ�, �޸�Ǯ�� ���,					\r\n	
	 *		���α׷��� ���� ���ϼ��ִ�.												\r\n
	 *		�޸� ����ȭ�� �����Ҽ��ְ�, ���޸𸮻���, ������ ȿ���� ��������.		\r\n
	 *		Ư��, ��Ƽ�ھ� �ý��ۿ��� ���� ��������� �ִ�.							\r\n
	 *																				\r\n
	 *		Low-Fragmentation Heap���� ����										\r\n
	 *		�� �Լ��� ȣ����� �ʾƵ�, ���ڵ尡 ������ Win2000Pro Sp4�̻���			\r\n
	 *		OS������ �����ϹǷ�, Win98�� �����Ϸ��� �Ѵٸ�, �ּ�ȭ���Ѿ� �Ѵ�.		\r\n
	 *																				\r\n
	 *		��Ÿ�� ���Լ��� ȣ������ �ʾƵ� �⺻�����Ѵ�.							\r\n
	 *		16K���� ���� �޸𸮸� �Ҵ��ϸ�, LFH�� �޸𸮰� �Ҵ�,					\r\n
	 *		���̻��� ���������� �Ҵ�ȴ�.											\r\n
	 */
	void EnableLFH();

	/**
	 * @brief	���α׷��� ������� ������ �����մϴ�.
	 * @param pLocale ���� 
	 * @remarks	wprintf �� �����ڵ� ���ڿ��� �ֿܼ� ����ϱ� ���ؼ��� ������ ������ �ؾ���.	\r\n
	 *			�ѱ��� : Korean																\r\n
	 *			���� : English																\r\n
	 */
	void SetLocale(CHAR* pLocale);

	/**
	 * @brief	���ӿ��� ����� Object�� GUID�� ���´�	
	 * @return	������ GUID�� 0x00000001 ~ 0xFFFFFFFF ����
	 */
	DWORD	GetGUID();

	/**
	 * @brief	String�� ũ��Pixel�� ���Ѵ�.
	 * @param string	����� ���� ��Ʈ��
	 * @param font		����� ��Ʈ
	 * @return			����� String�� ũ��
	 */
	SIZE	GetStringSize( const WCHAR* string, HFONT font );

	/**
	 * @brief	������ Ŀ���� �����´�.
	 * @return	Main�����쿡���� ���콺 ������
	 * @warning	Nave::hMainWnd �� �����Ǿ��־���Ѵ�.
	 */
	POINT	GetMousePos();

	/**
	 * @brief	������ Ŀ���� �����´�.
	 * @param wnd	������ �ڵ�
	 * @return	�����쿡���� ���콺 ������
	 */
	POINT	GetMousePos( HWND wnd );

	/**
	 * @brief	���� ���콺�� ��� �����쿡 �ö� �ִ��� 
	 * @return	���콺�� �����ϴ� �������� �ڵ�
	 */
	HWND	IsWindowFromPoint();

	/**
	 * @brief	���� ���α׷��� �̸��� ���Ѵ�
	 * @param buf	���ø����̼��� �̸��� ������ ����
	 * @param len	������ ũ��
	 * @return	���ø����̼� �̸��� ����
	 */
	INT		GetProgramName( LPWSTR buf, INT len );	

	/**
	 * @brief	���� ���α׷��� ��θ� ���Ѵ�.
	 * @param buf	���ø����̼��� ���丮�� ������ ����
	 * @param len	������ ũ��
	 * @return	���ø����̼� ���丮�� ����
	 */
	INT		GetProgramDir( LPWSTR buf, INT len );	

	/**
	 * @brief	Path���� ��θ� ���Ѵ�
	 * @param path	��� ����
	 * @param buf	��ο��� ���� ���丮
	 * @return	���丮�� ����
	 */
	INT		GetDir( LPCWSTR path, LPWSTR buf );		


	/**
	 * @brief	���ڿ����� 1Line�� �о���δ� �Ѷ����� \n���� ���еȴ�.
	 * @param sBuf		�ҽ� ����
	 * @param maxsBuf	�ҽ� ���� ũ��
	 * @param tBuf		���� ����
	 * @param maxtBuf	���� ���� ũ��
	 * @param index		�ҽ� ���ۿ��� �о���� ������ġ
	 * @return	��������
	 */
	BOOL	GetLine( const CHAR* sBuf, INT maxsBuf, CHAR* tBuf, INT maxtBuf, LONG& index );

	/**
	 * @brief	���ڿ��� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param tBuf	���� ����
	 * @param len	���� ���� ũ��
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 */
	VOID	GetString( const CHAR* sBuf, CHAR* tBuf, INT len, LONG& index );

	/**
	 * @brief	1����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	BYTE �����
	 */
	BYTE	GetByte( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	2����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	SHORT �����
	 */
	SHORT 	GetShort( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	4����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	INT �����
	 */
	INT		GetInt( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	4����Ʈ �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	DWORD �����
	 */
	DWORD   GetDWORD( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	4����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	FLOAT �����
	 */
	FLOAT	GetFloat( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	8����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	DOUBLE �����
	 */
	DOUBLE	GetDouble( const CHAR* sBuf, LONG& index );

	/**
	 * @brief	���ڿ��� �����Ѵ�
	 * @param tBuf	���� ����
	 * @param sBuf	������ ��
	 * @param len	������ ����
	 * @param index ���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetString( CHAR* tBuf, const CHAR* sBuf, INT len, LONG& index );

	/**
	 * @brief	1����Ʈ BYTE�� �����Ѵ�.
	 * @param tBuf	���� ����
	 * @param sByte ������ ��
	 * @param index ���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetByte( CHAR* tBuf, BYTE sByte, LONG& index );

	/**
	 * @brief	2����Ʈ SHORT�� �����Ѵ�.
	 * @param tBuf		���� ����
	 * @param sShort	������ ��
	 * @param index		���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetShort( CHAR* tBuf, SHORT sShort, LONG& index );

	/**
	 * @brief	4����Ʈ INT�� �����Ѵ�.
	 * @param tBuf	���� ����
	 * @param sInt	������ ��
	 * @param index ���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetInt( CHAR* tBuf, INT sInt, LONG& index);

	/**
	 * @brief	4����Ʈ DWORD�� �����Ѵ�.
	 * @param tBuf		���� ����
	 * @param sDword	������ ��
	 * @param index		���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetDWORD( CHAR* tBuf, DWORD sDword, LONG& index );

	/**
	 * @brief	4����Ʈ FLOAT�� �����Ѵ�.
	 * @param tBuf		���� ����
	 * @param sFloat	������ ��
	 * @param index		���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetFloat( CHAR* tBuf, FLOAT sFloat, LONG& index );

	/**
	 * @brief	8����Ʈ double�� �����Ѵ�.
	 * @param tBuf		���� ����
	 * @param sDouble	������ ��
	 * @param index		���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetDouble( CHAR* tBuf, DOUBLE sDouble, LONG& index );


	/**
	 * @brief	���ڿ����� 1Line�� �о���δ� �Ѷ����� \n���� ���еȴ�.
	 * @param sBuf		�ҽ� ����
	 * @param maxsBuf	�ҽ� ���� ũ��
	 * @param tBuf		���� ����
	 * @param maxtBuf	���� ���� ũ��
	 * @param index		�ҽ� ���ۿ��� �о���� ������ġ
	 * @return	��������
	 */
	BOOL	GetLine( const WCHAR* sBuf, INT maxsBuf, WCHAR* tBuf, INT maxtBuf, LONG& index );

	/**
	 * @brief	���ڿ��� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param tBuf	���� ����
	 * @param len	���� ���� ũ��
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 */
	VOID	GetString( const WCHAR* sBuf, WCHAR* tBuf, INT len, LONG& index );

	/**
	 * @brief	1����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	BYTE �����
	 */
	BYTE	GetByte( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	2����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	SHORT �����
	 */
	SHORT 	GetShort( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	4����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	INT �����
	 */
	INT		GetInt( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	4����Ʈ �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	DWORD �����
	 */
	DWORD   GetDWORD( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	4����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	FLOAT �����
	 */
	FLOAT	GetFloat( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	8����Ʈ�� �о���δ�.
	 * @param sBuf	�ҽ� ����
	 * @param index �ҽ� ���ۿ��� �о���� ������ġ
	 * @return	DOUBLE �����
	 */
	DOUBLE	GetDouble( const WCHAR* sBuf, LONG& index );

	/**
	 * @brief	���ڿ��� �����Ѵ�
	 * @param tBuf	���� ����
	 * @param sBuf	������ ��
	 * @param len	������ ����
	 * @param index ���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetString( WCHAR* tBuf, const WCHAR* sBuf, INT len, LONG& index );

	/**
	 * @brief	1����Ʈ BYTE�� �����Ѵ�.
	 * @param tBuf	���� ����
	 * @param sByte ������ ��
	 * @param index ���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetByte( WCHAR* tBuf, BYTE sByte, LONG& index );

	/**
	 * @brief	2����Ʈ SHORT�� �����Ѵ�.
	 * @param tBuf		���� ����
	 * @param sShort	������ ��
	 * @param index		���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetShort( WCHAR* tBuf, SHORT sShort, LONG& index );

	/**
	 * @brief	4����Ʈ INT�� �����Ѵ�.
	 * @param tBuf	���� ����
	 * @param sInt	������ ��
	 * @param index ���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetInt( WCHAR* tBuf, INT sInt, LONG& index);

	/**
	 * @brief	4����Ʈ DWORD�� �����Ѵ�.
	 * @param tBuf		���� ����
	 * @param sDword	������ ��
	 * @param index		���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetDWORD( WCHAR* tBuf, DWORD sDword, LONG& index );

	/**
	 * @brief	4����Ʈ FLOAT�� �����Ѵ�.
	 * @param tBuf		���� ����
	 * @param sFloat	������ ��
	 * @param index		���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetFloat( WCHAR* tBuf, FLOAT sFloat, LONG& index );

	/**
	 * @brief	8����Ʈ double�� �����Ѵ�.
	 * @param tBuf		���� ����
	 * @param sDouble	������ ��
	 * @param index		���� ���ۿ��� ����� ������ġ
	 */
	VOID	SetDouble( WCHAR* tBuf, DOUBLE sDouble, LONG& index );


	/**
	 * @brief	���̳ʸ� ������ Read ���Դϴ�.
	 * @param sBuf	�ҽ� ����
	 * @param tBuf	���� ����
	 * @param len	���� ����
	 * @param seek	�ҽ� ���ۿ��� ���� ������ġ
	 */
	VOID	Read( const char* sBuf, void* tBuf, INT len, LONG& seek );

	/**
	 * @brief	���̳ʸ� ������ Write �մϴ�.
	 * @param sBuf	���� ����
	 * @param tBuf	������ ��
	 * @param len	������ ����
	 * @param seek	���� ���ۿ��� ����� ������ġ
	 */
	VOID	Write( const void* sBuf, char* tBuf, INT len, LONG& seek );

}