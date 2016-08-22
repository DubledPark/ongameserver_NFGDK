/** 
 *  @file		NFWindow.h
 *  @brief		������ ���� ��ü
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#define WIN32_LEAN_AND_MEAN		

#include <Nave/Nave.h>
#include <list>

namespace NaveFramework {

	/// ���̺귯������ ������ ��Ͻ� �⺻������ ����ϴ� �����췹�� �̸�
	#define PROJECTNAME		L"NaveGDK_DX9"

	/// ���콺 �� ������
	#define GET_WHEEL_DELTA(p)  (int)((short)HIWORD(p))

	/** 
	 *  @class        NFWindow
	 *  @brief        �˾������� �� ���ϵ� ������� ������ ���� Ŭ����
	 *  @remarks      ���ø����̼ǿ��� �����츦 �����ϴ� ��� Ŭ������ ��� �޴´�.
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFWindow
	{
	public:
		// NFWindow ������
		NFWindow();
		/// NFWindow �Ҹ���
		virtual ~NFWindow();

	public:
		/// ���� ������ ����Ʈ
		static NFWindow*			s_pMainWnd;

	public:
		/// ������ �ڵ� ����
		VOID	OnSetWnd(HWND hWnd) { m_hWnd = hWnd;	}
		/// �θ� �ڵ� ����
		VOID	OnSetParent(HWND hWnd) { m_hParent = hWnd;	}
		/// Ȱ��ȭ ����
		BOOL	IsShow() { return m_bShow; }

		/// �����츦 Show �մϴ�.
		virtual VOID	OnShowWindow() 
		{ 
			m_bShow = TRUE;
			if(m_hWnd)
				ShowWindow(m_hWnd, SW_SHOWNA);
		}
		/// �����츦 Hide �մϴ�.
		virtual VOID	OnHideWindow() 
		{ 
			m_bShow = FALSE;
			if(m_hWnd)
				ShowWindow(m_hWnd, SW_HIDE);
		}

		/// ������ ���� ũ��
		inline int GetWidth() { return m_iWidth; }
		/// ������ ���� ũ��
		inline int GetHeight() { return m_iHeight; }

		inline HWND GetWND() { return m_hWnd; }

		/// TrackMouse Event ó��
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

		/// Windows MsgProc�� �޽����� �߻��ϸ� �̰����� �޽����� �Ѿ�´�.
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// ������ ����
		virtual BOOL OnDestroy()				{ return TRUE; }
		/// ������ ����
		virtual BOOL OnCreate(HWND hParent)		{ return TRUE; }

	protected:
		/// ������ Ȱ��ȭ ����
		BOOL	m_bShow;
		/// �ڱ��ڽ��� ������ �ڵ�
		HWND	m_hWnd;		
		/// �θ� ������ �ڵ�
		HWND	m_hParent;		
		/// ���콺 ���� ����
		BOOL	m_bMouseIn;

		/// ������ ����ũ��
		INT		m_iWidth;
		/// ������ ����ũ��
		INT		m_iHeight;
	};

	/// ����Ʈ�� �����츦 �߰�
	BOOL InsertWindows(NFWindow * ObjPtr);				
	/// ����Ʈ�� �ִ� �����츦 ����
	BOOL DeleteWindows(NFWindow * ObjPtr);				

	/// NFWindow����Ʈ�� ��ϵ� hWnd�� �����찪�� ���´�.
	NFWindow* GetWINDOW(HWND hWnd);

	/// Windows ������Ʈ�� ����Ѵ�.
	VOID InitWindowsObject(HINSTANCE hInst, LPCWSTR Title, LPCWSTR Icon = IDI_APPLICATION);

	/// �⺻ ������� �����츦 �����Ѵ�.
	HWND CreateWindowsEx(LPCWSTR Title, DWORD dwExStyle, DWORD dwStyle, int Sx, int Sy, int Width, int Height, HWND Parent = NULL);
}