/** 
 *  @file		NFForceFeedback.h
 *  @brief		force feedback device
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFKeyMap.h"

namespace NaveFramework { 

	/** 
	 *  @class        NFForceFeedback
	 *  @brief        force feedback device
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFForceFeedback
	{
	public:
		NFForceFeedback(void);
		~NFForceFeedback(void);

		/**
		 * @brief	상태를 활성화 합니다.
		 * @param nState	황성화 상태 WA_ACTIVE 등등.
		 * @return  성공여부
		 */
		HRESULT	SetActive(DWORD nState = WA_ACTIVE);
		/**
		 * @brief	현재 활성화 상태 리턴
		 * @return  활성화 상태값
		 */
		BOOL	IsActive() { return m_bActive; }

		/**
		 * @brief	force feedback device를 생성합니다.
		 * @param hWnd 윈도우 핸들
		 * @param pDI  DirectInput 핸들 
		 * @return	성공여부
		 */
		BOOL	Create(HWND hWnd, LPDIRECTINPUT8 pDI);
		/// force feedback device를 삭제합니다.
		VOID	Release();

		/**
		 * @brief	흔들어 줍니다. :)
		 * @param lX X값
		 * @param lY Y값
		 * @param fLifeTime 유지시간
		 * @return  결과
		 */
		HRESULT SetFF(LONG lX = 0, LONG lY = 0, float fLifeTime = 0);

		/// force feedback상태를 업데이트 합니다. 
		VOID	Update();

	private:
		HWND				 m_hWnd;
		LPDIRECTINPUTDEVICE8 m_pDevice;     
		LPDIRECTINPUTEFFECT  m_pEffect;

		DWORD                m_dwNumForceFeedbackAxis;
		LONG				 m_lLifeTime;
		LONG				 m_lLastTime;

		BOOL				 m_bActive;
	};

}