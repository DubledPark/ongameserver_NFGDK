/** 
 *  @file		NFJoystick.h
 *  @brief		조이스틱 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFKeyMap.h"

namespace NaveFramework {

	/** 
	 *  @class        NFJoystick
	 *  @brief        조이스틱 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFJoystick
	{
	public:
		NFJoystick(void);
		~NFJoystick(void);

	public:
		HRESULT	SetActive(DWORD nState = WA_ACTIVE);
		BOOL	IsActive() { return m_bActive; }

		BYTE*	GetKeyState() { return m_bKeyState; }

		inline	LONG GetAxisX() { return m_JoyState.lX; }
		inline	LONG GetAxisY() { return m_JoyState.lY; }
		inline	LONG GetAxisZ() { return m_JoyState.lZ; }

		inline	LONG GetRotX() { return m_JoyState.lRx; }
		inline	LONG GetRotY() { return m_JoyState.lRy; }
		inline	LONG GetRotZ() { return m_JoyState.lRz; }

		inline	LONG GetSlider(int iPos) 
		{ 
			if(iPos < 0 || iPos >= 2)
				return 0;

			return m_JoyState.rglSlider[iPos]; 
		}

		inline	LONG GetPov(int iPos) 
		{ 
			if(iPos < 0 || iPos >= 4)
				return 0;

			return m_JoyState.rgdwPOV[iPos]; 
		}

		BOOL	Create(HWND hWnd, LPDIRECTINPUT8 pDI);
		VOID	Release();

		VOID	Update();

	private:
		HWND				 m_hWnd;
		LPDIRECTINPUTDEVICE8 m_pJoy;     

		BYTE	m_bKeyState[128];
		DIJOYSTATE2 m_JoyState;           // DInput joystick state 

		BOOL	m_bActive;

		VOID	UpdateKeyMap();
	};

}