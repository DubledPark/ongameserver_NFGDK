/** 
 *  @file		NFWindow.h
 *  @brief		윈도우 생성 객체
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#define WIN32_LEAN_AND_MEAN		

#include <Nave/Nave.h>
#include <list>

namespace NaveFramework {

	/// 라이브러리에서 윈도우 등록시 기본적으로 사용하는 윈도우레지 이름
	#define PROJECTNAME		L"NaveGDK_DX9"

	/// 마우스 휠 디파인
	#define GET_WHEEL_DELTA(p)  (int)((short)HIWORD(p))

	/** 
	 *  @class        NFWindow
	 *  @brief        팝업윈도우 및 차일드 윈도우등 윈도우 관리 클래스
	 *  @remarks      어플리케이션에서 윈도우를 생성하는 모든 클래스가 상속 받는다.
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFWindow
	{
	public:
		// NFWindow 생성자
		NFWindow();
		/// NFWindow 소멸자
		virtual ~NFWindow();

	public:
		/// 메인 윈도우 포인트
		static NFWindow*			s_pMainWnd;

	public:
		/// 윈도우 핸들 셋팅
		VOID	OnSetWnd(HWND hWnd) { m_hWnd = hWnd;	}
		/// 부모 핸들 셋팅
		VOID	OnSetParent(HWND hWnd) { m_hParent = hWnd;	}
		/// 활성화 상태
		BOOL	IsShow() { return m_bShow; }

		/// 윈도우를 Show 합니다.
		virtual VOID	OnShowWindow() 
		{ 
			m_bShow = TRUE;
			if(m_hWnd)
				ShowWindow(m_hWnd, SW_SHOWNA);
		}
		/// 윈도우를 Hide 합니다.
		virtual VOID	OnHideWindow() 
		{ 
			m_bShow = FALSE;
			if(m_hWnd)
				ShowWindow(m_hWnd, SW_HIDE);
		}

		/// 윈도우 가로 크기
		inline int GetWidth() { return m_iWidth; }
		/// 윈도우 세로 크기
		inline int GetHeight() { return m_iHeight; }

		inline HWND GetWND() { return m_hWnd; }

		/// TrackMouse Event 처리
		VOID OnTrackMouseEvent();

		/// WM_PAINT
		virtual VOID OnPaint(HWND hWnd, HDC hDC) { return; }
		/// WM_COMMAND
		virtual VOID OnCommand(HWND hWnd, int iID, int iEvent, LPARAM lParam) { return; }
		/// WM_MOUSEWHEEL
		virtual BOOL OnMouseWheel(int iMouseX, int iMouseY, int zDelta) { return FALSE; }
		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_MOUSEHOVER
		virtual BOOL OnMouseOver(int iMouseX, int iMouseY) { return FALSE; }
		///WM_LBUTTONDBLCLK
		virtual BOOL OnLButtonDClick(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_RBUTTONDOWN
		virtual BOOL OnRButtonDown(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_RBUTTONUP
		virtual BOOL OnRButtonUp(int iMouseX, int iMouseY) { return FALSE; }

		/// Windows MsgProc에 메시지가 발생하면 이곳으로 메시지가 넘어온다.
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// 윈도우 삭제
		virtual BOOL OnDestroy()				{ return TRUE; }
		/// 윈도우 생성
		virtual BOOL OnCreate(HWND hParent)		{ return TRUE; }

	protected:
		/// 윈도우 활성화 상태
		BOOL	m_bShow;
		/// 자기자신의 윈도우 핸들
		HWND	m_hWnd;		
		/// 부모 윈도우 핸들
		HWND	m_hParent;		
		/// 마우스 상태 변수
		BOOL	m_bMouseIn;

		/// 윈도우 가로크기
		INT		m_iWidth;
		/// 윈도우 세로크기
		INT		m_iHeight;
	};

	/// 리스트에 윈도우를 추가
	BOOL InsertWindows(NFWindow * ObjPtr);				
	/// 리스트에 있는 윈도우를 삭제
	BOOL DeleteWindows(NFWindow * ObjPtr);				

	/// NFWindow리스트에 등록된 hWnd의 윈도우값을 얻어온다.
	NFWindow* GetWINDOW(HWND hWnd);

	/// Windows 레지스트를 등록한다.
	VOID InitWindowsObject(HINSTANCE hInst, LPCWSTR Title, LPCWSTR Icon = IDI_APPLICATION);

	/// 기본 윈도우로 윈도우를 생성한다.
	HWND CreateWindowsEx(LPCWSTR Title, DWORD dwExStyle, DWORD dwStyle, int Sx, int Sy, int Width, int Height, HWND Parent = NULL);
}