/** 
 *  @file		NFForceFeedback.h
 *  @brief		force feedback device
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
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
		 * @brief	���¸� Ȱ��ȭ �մϴ�.
		 * @param nState	Ȳ��ȭ ���� WA_ACTIVE ���.
		 * @return  ��������
		 */
		HRESULT	SetActive(DWORD nState = WA_ACTIVE);
		/**
		 * @brief	���� Ȱ��ȭ ���� ����
		 * @return  Ȱ��ȭ ���°�
		 */
		BOOL	IsActive() { return m_bActive; }

		/**
		 * @brief	force feedback device�� �����մϴ�.
		 * @param hWnd ������ �ڵ�
		 * @param pDI  DirectInput �ڵ� 
		 * @return	��������
		 */
		BOOL	Create(HWND hWnd, LPDIRECTINPUT8 pDI);
		/// force feedback device�� �����մϴ�.
		VOID	Release();

		/**
		 * @brief	���� �ݴϴ�. :)
		 * @param lX X��
		 * @param lY Y��
		 * @param fLifeTime �����ð�
		 * @return  ���
		 */
		HRESULT SetFF(LONG lX = 0, LONG lY = 0, float fLifeTime = 0);

		/// force feedback���¸� ������Ʈ �մϴ�. 
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