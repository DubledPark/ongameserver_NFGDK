/** 
 *  @file		NFKeyboard.h
 *  @brief		키브도 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFKeyMap.h"

namespace NaveFramework {

	/** 
	 *  @class        NFKeyboard
	 *  @brief        키보드를 관리하는 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFKeyboard
	{
	public:
		NFKeyboard(void);
		~NFKeyboard(void);

	public:
		HRESULT	SetActive(DWORD nState = WA_ACTIVE);
		BOOL	IsActive() { return m_bActive; }

		BYTE*	GetKeyState() { return m_bKeyState; }

		BOOL	Create(HWND hWnd, LPDIRECTINPUT8 pDI, BOOL bExclusive = TRUE, BOOL bForeground = TRUE, BOOL bImmediate = TRUE, BOOL bDisableWindowsKey = TRUE);
		VOID	Release();

		VOID	Update();

	private:
		HRESULT ReadImmediateData();
		HRESULT ReadBufferedData();

		VOID	UpdateKeyMap();

	private:
		HWND				 m_hWnd;
		LPDIRECTINPUTDEVICE8 m_pKeyboard;

		BYTE	m_bKeyState[MAX_KEY];

		BOOL	m_bActive;

		BOOL    m_bExclusive;
		BOOL    m_bForeground;
		BOOL    m_bImmediate;
		BOOL    m_bDisableWindowsKey;
	};

}