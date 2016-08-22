/** 
 *  @file		NFHook.h
 *  @brief		키보드를 후킹하여 윈도우 키등을 제어한다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

#include <winuser.h>
#include <Windows.h>

namespace NaveFramework { 
	
	/** 
	 *  @class        NFHook
	 *  @brief        Keyboard Hook Class
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFHook 
	{
	public:
		// Hook 변수
		static HHOOK	s_hHook = NULL;
		static HWND		s_hHookWnd = NULL;

		static BOOL		s_bAltKey = FALSE;
		static BOOL		s_bTabKey = FALSE;
		static BOOL		s_bWinNT = FALSE;

		static LRESULT CALLBACK msghook( UINT nCode, WPARAM wParam, LPARAM lParam)
		{
			HWND hWnd = GetFocus();

			if(::GetParent(hWnd) != s_hHookWnd && hWnd != s_hHookWnd)
				return CallNextHookEx(s_hHook, nCode, wParam, lParam);

			if(nCode == HC_ACTION)
			{
				if ( s_bWinNT ) // WH_KEYBOARD_LL = WINNT
				{
					// Disable Windows Key
					PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
					if ( p->vkCode == VK_LWIN || p->vkCode == VK_RWIN || p->vkCode == VK_APPS)
						return 1;

					if(p->vkCode == VK_SNAPSHOT)
					{
						SendMessage(s_hHookWnd, (UINT)wParam, VK_SNAPSHOT, 0);
						return 1;
					}

					switch(wParam)
					{
					case WM_KEYUP:
					case WM_KEYDOWN:
						switch(p->vkCode)
						{
						case VK_LMENU:
							return 1;
						}
						break;

						// Alt키 밎 시스템 키 막기
					case WM_SYSKEYDOWN:
						if(p->vkCode == VK_LMENU)
						{
							s_bAltKey = TRUE;
							return 0;
						}
						else if(p->vkCode == VK_TAB)
						{
							if(s_bAltKey)
							{
								s_bAltKey = FALSE;
								ShowWindow(s_hHookWnd, SW_MINIMIZE);
								return 1;
							}
						}
						break;
					}
				}
				else // WH_KEYBOARD = WIN98
				{
					if ( (DWORD)wParam == VK_LWIN || (DWORD)wParam == VK_RWIN || (DWORD)wParam == VK_APPS )
						return 1;

					if ( (DWORD)lParam & 0x40000000 ) // Key Pressed
					{
						if(wParam == VK_SNAPSHOT)	// 윈9X계열은 한번 발생한다.
						{
							SendMessage(s_hHookWnd, WM_KEYDOWN, VK_SNAPSHOT, 0);
							return 1;
						}

						if(wParam == VK_LMENU)
						{
							s_bAltKey = TRUE;
							return 0;
						}
						else if(wParam == VK_TAB)
						{
							if(s_bAltKey)
							{
								s_bAltKey = FALSE;
								ShowWindow(s_hHookWnd, SW_MINIMIZE);
								return 1;
							}
						}
					}
				}
			}

			return CallNextHookEx(s_hHook, nCode, wParam, lParam);
		}

		// Hook을 생성한다.
		// 13은 WH_KEYBOARD_LL 윈2k용의 키보드 제어.. xp에서도 사용 
		static VOID LOCKHOOK(HINSTANCE hInst, HWND hWnd) 
		{ 
			s_hHookWnd=hWnd; 

			// OS버젼 알아내기
			OSVERSIONINFO osvi;
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx( &osvi );

			if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT ) // WinNT or Windows 2000
			{
				s_bWinNT = TRUE;
				s_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)msghook, hInst, 0); 
			}
			else
			{
				s_bWinNT = FALSE;
				s_hHook = ::SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)msghook, hInst, 0);
			}
		}
			
		// 생성한 Hook를 삭제한다.
		static void UNLOCKHOOK() 
		{
			UnhookWindowsHookEx(s_hHook);
		}
	};
}