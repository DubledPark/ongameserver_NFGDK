/** 
 *  @file		NaveFramework.h
 *  @brief		NaveFramework 관련 함수
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#define WIN32_LEAN_AND_MEAN	

// 스트럭쳐 1바이트 정렬.
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

	/// 에러 메시지 박스를 띄운다.
	int ErrorMessage(WCHAR *pchMessage, ...);

	/// 메시지 박스를 띄운다.
	int InfoMessage(WCHAR *pchMessage, ...);

}