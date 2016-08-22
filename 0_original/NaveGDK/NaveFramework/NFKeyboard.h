/** 
 *  @file		NFKeyboard.h
 *  @brief		Ű�굵 Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFKeyMap.h"

namespace NaveFramework {

	/** 
	 *  @class        NFKeyboard
	 *  @brief        Ű���带 �����ϴ� Ŭ����
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