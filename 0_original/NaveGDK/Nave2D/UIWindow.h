/** 
 *  @file		UIWindow.h
 *  @brief		하나의 윈도우 객체를 관리한다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "Nave2D.h"
#include <NaveFramework/NFWindow.h>

namespace Nave2D {

	/** 
	 *  @class        UIWindow
	 *  @brief        하나의 윈도우 객체를 관리한다.
	 *  @remarks      Windows에서 윈도우를 생성하는 클래스
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIWindow : public NaveFramework::NFWindow  
	{
	public:
		/// UIWindow 생성자
		UIWindow();
		/// UIWindow 소멸자
		virtual ~UIWindow();

	public:
		/// 프로시져를 종료합니다.
		virtual HRESULT EndProcess();
		/// Exit 상태
		inline BOOL	IsExit()	{ return m_Exit; }

		/// 프로시저를 시작합니다.
		virtual HRESULT StartProcess(BOOL bWindow = TRUE);

		/// Windows Create
		virtual BOOL CreateWindows(HINSTANCE hInstance, int nCmdShow, LPCWSTR Title, int Width, int Height, BOOL PopupMode = TRUE, BOOL WindowMode = TRUE, LPCWSTR Icon = MIS(IDI_APPLICATION));

	protected:
		/// WM_PAINT
		virtual VOID OnPaint(HWND hWnd, HDC hDC) { return; }
		/// 윈도우의 객체를 업데이트 한다.
		virtual VOID OnUpdate(float dt) { };
		/// 화면에 객체를 렌더링합니다.
		virtual VOID OnRender() { return; };

		/// UIObject에서 발생하는 Event를 처리합니다.
		virtual VOID OnObjectEvent(INT nID, INT iEvent, LPARAM lParam) { return; };

		/// Windows MsgProc
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		/// 윈도우 타이틀
		WCHAR				m_WindowsTitle[64];

		/// WS_POPUP 상태 전체모드일때 사용
		BOOL				m_bPopupMode;

		/// 윈도우 Focus 상태
		BOOL				m_bFocus;
		/// 윈도우 Active 상태
		BOOL				m_bActive;
		/// 윈도우 Ready 상태
		BOOL				m_bReady;

		/// 윈도우 Exit 상태
		BOOL				m_Exit;

		/// 윈도우의 영역
		CRect				m_WindowRect;

		/// 윈도우 모드 여부.
		BOOL				m_bWindowMode;

	protected:
		/// 윈도우 리사이즈
		VOID	ReSizeWindow(BOOL bWindowMode, int Width, int Height);
		/// Minimize Windows 호출
		virtual HRESULT MinimizedWindows();
		/// Restore Windows 호출
		virtual HRESULT RestoredWindows();
		/// 윈도우를 Show 합니다.
		VOID	OnShowWindow() 
		{ 
			m_bShow = TRUE;
			if(m_hWnd)
				ShowWindow(m_hWnd, SW_SHOWNA);

			SetForegroundWindow(Nave::hMainWnd);					// Slightly Higher Priority
			SetFocus(Nave::hMainWnd);								// Sets Keyboard Focus To The Window
		}

		/// 사용자 오브젝트를 초기화 합니다. (윈도우설정이 종료됀 후에 호출)
		virtual HRESULT InitObjects()			{ return S_OK; }
		/// 사용자 오브젝트를 삭제합니다. (메인루프가 끊난후(EndProcess호출후) 호출)
		virtual HRESULT ReleaseObjects()		{ return S_OK; }
	};

}