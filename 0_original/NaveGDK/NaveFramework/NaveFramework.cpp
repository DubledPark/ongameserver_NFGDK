#include "NaveFramework.h"
#include <Nave/NFSync.h>

using namespace std;	// std::list 라고 안쓰고 list 라고만 선언해도 된다. 
						// 안쓰면 반드시 std::list라고 사용해야한다.

namespace NaveFramework {

	//////////////////////////////////////////////////////////////////////
	//
	//	ErrorMessage(WCHAR *pchMessage, ...)
	//
	//		Error Message 출력
	//
	//	pchMessage	:	에러가 발생 했을 경우 출력 메시지(인자를 포함함)
	//
	//////////////////////////////////////////////////////////////////////
	int ErrorMessage(WCHAR *pchMessage, ...)
	{
		WCHAR	chBuffer[256];
		va_list va;

		va_start(va, pchMessage);
		vswprintf(chBuffer, pchMessage, va);
		va_end(va);
			
		MessageBoxW(Nave::hMainWnd, chBuffer, L"에러 메시지", MB_ICONWARNING | MB_OK);
		PostQuitMessage(0);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////
	//
	//	InfoMessage(WCHAR *pchMessage, ...)
	//
	//		Message를 출력 한다.
	//
	//	pchMessage	:	출력 메시지(인자를 포함함)
	//
	//////////////////////////////////////////////////////////////////////
	int InfoMessage(WCHAR *pchMessage, ...)
	{
		WCHAR	chBuffer[256];
		va_list va;

		va_start(va, pchMessage);
		vswprintf(chBuffer, pchMessage, va);
		va_end(va);

		return MessageBoxW(Nave::hMainWnd, chBuffer, L"확인 메시지", MB_ICONINFORMATION | MB_OK);
	}

}