#include "NaveFramework.h"
#include <Nave/NFSync.h>

using namespace std;	// std::list ��� �Ⱦ��� list ��� �����ص� �ȴ�. 
						// �Ⱦ��� �ݵ�� std::list��� ����ؾ��Ѵ�.

namespace NaveFramework {

	//////////////////////////////////////////////////////////////////////
	//
	//	ErrorMessage(WCHAR *pchMessage, ...)
	//
	//		Error Message ���
	//
	//	pchMessage	:	������ �߻� ���� ��� ��� �޽���(���ڸ� ������)
	//
	//////////////////////////////////////////////////////////////////////
	int ErrorMessage(WCHAR *pchMessage, ...)
	{
		WCHAR	chBuffer[256];
		va_list va;

		va_start(va, pchMessage);
		vswprintf(chBuffer, pchMessage, va);
		va_end(va);
			
		MessageBoxW(Nave::hMainWnd, chBuffer, L"���� �޽���", MB_ICONWARNING | MB_OK);
		PostQuitMessage(0);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////
	//
	//	InfoMessage(WCHAR *pchMessage, ...)
	//
	//		Message�� ��� �Ѵ�.
	//
	//	pchMessage	:	��� �޽���(���ڸ� ������)
	//
	//////////////////////////////////////////////////////////////////////
	int InfoMessage(WCHAR *pchMessage, ...)
	{
		WCHAR	chBuffer[256];
		va_list va;

		va_start(va, pchMessage);
		vswprintf(chBuffer, pchMessage, va);
		va_end(va);

		return MessageBoxW(Nave::hMainWnd, chBuffer, L"Ȯ�� �޽���", MB_ICONINFORMATION | MB_OK);
	}

}