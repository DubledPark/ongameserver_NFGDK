/** 
 *  @file		NFInputManager.h
 *  @brief		Input 메니져 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
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
	 *  @brief        DirectInput을 이용하여 Keyboard, Mouse, Joystick등을 관리
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFInputManager : public Nave::NFSingleton<NFInputManager>
	{
	public:
		/// NFInputManager 생성자
		NFInputManager(void);
		/// NFInputManager 소멸자
		~NFInputManager(void);

	public:
		/// DirectInput객체를 생성한다.
		BOOL	Create(HWND hWnd);
		/// 모든 객체를 삭제한다.
		VOID	ReleaseAll();

		/// 키보드를 생성합니다.
		BOOL	CreateKeyboard(BOOL bExclusive = TRUE, BOOL bForeground = TRUE, BOOL bImmediate = TRUE, BOOL bDisableWindowsKey = TRUE);
		/// 키보드 얻어옵니다.
		const NFKeyboard*	GetKeyboard() const { return m_pKeyboard; }

		/// 마우스를 생성합니다.
		BOOL	CreateMouse(BOOL bExclusive = TRUE, BOOL bForeground = TRUE);
		/// 마우스를 얻어옵니다.
		const NFMouse*	GetMouse() const { return m_pMouse; }
		
		/// 조이스틱을 생성합니다.
		BOOL	CreateJoystick();
		/// 조이스틱을 얻어옵니다.
		const NFJoystick*	GetJoystick() const { return m_pJoystick; }

		/// Force Feedback을 생성합니다.
		BOOL	CreateFFDevice();
		/// Force Feedback을 얻어옵니다.
		const NFForceFeedback*	GetFFDevice() const { return m_pFFEffect; }

		/// Input 를 얻어옵니다.
		LPDIRECTINPUT8 GetDevice() { return m_pDI; }
		
		/// Input 를 활성화 합니다.
		HRESULT	SetActive(DWORD nState = WA_ACTIVE);
		/// Input의 활성화 상태
		BOOL	IsActive() { return m_bActive; }
		
		/// Input을 업데이트 합니다.
		VOID	Update();

		/// KeyMap을 리셋합니다.
		VOID	ResetKeyMap();
		/// Key를 추가합니다.
		VOID	AddKeyMap(Nave::String szCmd, int iKey, int iModKey, int iState, int iEvent, float fParam);
		/// KeyList를 이용해 KeyMap을 추가합니다.
		VOID	AddKeyMapList(Input::INPUTMAP* pList, int iSize);

		/// KeyMap을 저장합니다.
		void	SaveInputMap(const WCHAR* szFileName = L"Key.xml");
		/// KeyMap을 로드합니다.
		void	LoadInputMap(const WCHAR* szFileName = L"Key.xml");

	private:
		/// 윈도우 핸들
		HWND				m_hWnd;
		/// DirectInput 핸들
		LPDIRECTINPUT8      m_pDI;

		/// 활성화 상태핸들
		BOOL				m_bActive;

		/// 키보드 
		NFKeyboard*			m_pKeyboard;
		/// 마우스
		NFMouse*			m_pMouse;
		/// 조이스틱
		NFJoystick*			m_pJoystick;
		/// ForceFeedback
		NFForceFeedback*	m_pFFEffect;
	};
}
