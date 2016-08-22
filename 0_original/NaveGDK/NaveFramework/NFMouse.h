/** 
 *  @file		NFMouse.h
 *  @brief		마우스 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFKeyMap.h"

namespace NaveFramework { 

	/** 
	 *  @class        NFMouse
	 *  @brief        마우스를 관리한다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFMouse
	{
	public:
		NFMouse(void);
		~NFMouse(void);

	public:
		HRESULT	SetActive(DWORD nState = WA_ACTIVE);
		BOOL	IsActive() { return m_bActive; }

		BYTE*	GetKeyState() { return m_bKeyState; }

		inline	LONG GetStateX() { return m_MouseState.lX; }
		inline	LONG GetStateY() { return m_MouseState.lY; }
		inline	LONG GetStateZ() { return m_MouseState.lZ; }

		inline	LONG GetMouseX() { return m_lMouseX; }
		inline	LONG GetMouseY() { return m_lMouseY; }
		inline	LONG GetMouseZ() { return m_lMouseZ; }

		BOOL	Create(HWND hWnd, LPDIRECTINPUT8 pDI, BOOL bExclusive = TRUE, BOOL bForeground = TRUE);
		VOID	Release();

		VOID	SetMousePos(LONG lX, LONG lY, LONG lZ);

		VOID	Update();

	private:
		HRESULT ReadImmediateData();
		HRESULT ReadBufferedData();

		VOID	UpdateKeyMap();

	private:
		HWND				 m_hWnd;
		LPDIRECTINPUTDEVICE8 m_pMouse;     

		BYTE	m_bKeyState[8];

		BOOL	m_bActive;

		BOOL    m_bExclusive;
		BOOL    m_bForeground;
		BOOL    m_bImmediate;

		DIMOUSESTATE2 m_MouseState;      // DirectInput mouse state structure

		LONG	m_lMouseX;
		LONG	m_lMouseY;
		LONG	m_lMouseZ;
	};

}