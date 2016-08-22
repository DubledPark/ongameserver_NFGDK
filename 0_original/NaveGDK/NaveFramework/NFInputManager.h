/** 
 *  @file		NFInputManager.h
 *  @brief		Input �޴��� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFKeyMap.h"
#include "NFKeyboard.h"
#include "NFMouse.h"
#include "NFJoystick.h"
#include "NFForceFeedback.h"

namespace NaveFramework {

	/** 
	 *  @class        NFInputManager
	 *  @brief        DirectInput�� �̿��Ͽ� Keyboard, Mouse, Joystick���� ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFInputManager : public Nave::NFSingleton<NFInputManager>
	{
	public:
		/// NFInputManager ������
		NFInputManager(void);
		/// NFInputManager �Ҹ���
		~NFInputManager(void);

	public:
		/// DirectInput��ü�� �����Ѵ�.
		BOOL	Create(HWND hWnd);
		/// ��� ��ü�� �����Ѵ�.
		VOID	ReleaseAll();

		/// Ű���带 �����մϴ�.
		BOOL	CreateKeyboard(BOOL bExclusive = TRUE, BOOL bForeground = TRUE, BOOL bImmediate = TRUE, BOOL bDisableWindowsKey = TRUE);
		/// Ű���� ���ɴϴ�.
		const NFKeyboard*	GetKeyboard() const { return m_pKeyboard; }

		/// ���콺�� �����մϴ�.
		BOOL	CreateMouse(BOOL bExclusive = TRUE, BOOL bForeground = TRUE);
		/// ���콺�� ���ɴϴ�.
		const NFMouse*	GetMouse() const { return m_pMouse; }
		
		/// ���̽�ƽ�� �����մϴ�.
		BOOL	CreateJoystick();
		/// ���̽�ƽ�� ���ɴϴ�.
		const NFJoystick*	GetJoystick() const { return m_pJoystick; }

		/// Force Feedback�� �����մϴ�.
		BOOL	CreateFFDevice();
		/// Force Feedback�� ���ɴϴ�.
		const NFForceFeedback*	GetFFDevice() const { return m_pFFEffect; }

		/// Input �� ���ɴϴ�.
		LPDIRECTINPUT8 GetDevice() { return m_pDI; }
		
		/// Input �� Ȱ��ȭ �մϴ�.
		HRESULT	SetActive(DWORD nState = WA_ACTIVE);
		/// Input�� Ȱ��ȭ ����
		BOOL	IsActive() { return m_bActive; }
		
		/// Input�� ������Ʈ �մϴ�.
		VOID	Update();

		/// KeyMap�� �����մϴ�.
		VOID	ResetKeyMap();
		/// Key�� �߰��մϴ�.
		VOID	AddKeyMap(Nave::String szCmd, int iKey, int iModKey, int iState, int iEvent, float fParam);
		/// KeyList�� �̿��� KeyMap�� �߰��մϴ�.
		VOID	AddKeyMapList(Input::INPUTMAP* pList, int iSize);

		/// KeyMap�� �����մϴ�.
		void	SaveInputMap(const WCHAR* szFileName = L"Key.xml");
		/// KeyMap�� �ε��մϴ�.
		void	LoadInputMap(const WCHAR* szFileName = L"Key.xml");

	private:
		/// ������ �ڵ�
		HWND				m_hWnd;
		/// DirectInput �ڵ�
		LPDIRECTINPUT8      m_pDI;

		/// Ȱ��ȭ �����ڵ�
		BOOL				m_bActive;

		/// Ű���� 
		NFKeyboard*			m_pKeyboard;
		/// ���콺
		NFMouse*			m_pMouse;
		/// ���̽�ƽ
		NFJoystick*			m_pJoystick;
		/// ForceFeedback
		NFForceFeedback*	m_pFFEffect;
	};
}
