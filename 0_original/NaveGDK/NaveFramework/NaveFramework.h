/** 
 *  @file		NaveFramework.h
 *  @brief		NaveFramework ���� �Լ�
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#define WIN32_LEAN_AND_MEAN	

// ��Ʈ���� 1����Ʈ ����.
#ifdef _WIN32
	#pragma pack( push ,1 )
#else    
	#pragma pack(1)
#endif

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"imm32.lib")

#include <Nave/Nave.h>

#include <gdiplus.h>
#pragma comment(lib, "GdiPlus")
using namespace Gdiplus;

#include "NFWindow.h"
#include "NFAdapter.h"
#include <list>

namespace NaveFramework {

	/// ���� �޽��� �ڽ��� ����.
	int ErrorMessage(WCHAR *pchMessage, ...);

	/// �޽��� �ڽ��� ����.
	int InfoMessage(WCHAR *pchMessage, ...);

}