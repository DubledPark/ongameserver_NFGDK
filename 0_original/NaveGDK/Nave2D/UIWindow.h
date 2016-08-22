/** 
 *  @file		UIWindow.h
 *  @brief		�ϳ��� ������ ��ü�� �����Ѵ�.
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "Nave2D.h"
#include <NaveFramework/NFWindow.h>

namespace Nave2D {

	/** 
	 *  @class        UIWindow
	 *  @brief        �ϳ��� ������ ��ü�� �����Ѵ�.
	 *  @remarks      Windows���� �����츦 �����ϴ� Ŭ����
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIWindow : public NaveFramework::NFWindow  
	{
	public:
		/// UIWindow ������
		UIWindow();
		/// UIWindow �Ҹ���
		virtual ~UIWindow();

	public:
		/// ���ν����� �����մϴ�.
		virtual HRESULT EndProcess();
		/// Exit ����
		inline BOOL	IsExit()	{ return m_Exit; }

		/// ���ν����� �����մϴ�.
		virtual HRESULT StartProcess(BOOL bWindow = TRUE);

		/// Windows Create
		virtual BOOL CreateWindows(HINSTANCE hInstance, int nCmdShow, LPCWSTR Title, int Width, int Height, BOOL PopupMode = TRUE, BOOL WindowMode = TRUE, LPCWSTR Icon = MIS(IDI_APPLICATION));

	protected:
		/// WM_PAINT
		virtual VOID OnPaint(HWND hWnd, HDC hDC) { return; }
		/// �������� ��ü�� ������Ʈ �Ѵ�.
		virtual VOID OnUpdate(float dt) { };
		/// ȭ�鿡 ��ü�� �������մϴ�.
		virtual VOID OnRender() { return; };

		/// UIObject���� �߻��ϴ� Event�� ó���մϴ�.
		virtual VOID OnObjectEvent(INT nID, INT iEvent, LPARAM lParam) { return; };

		/// Windows MsgProc
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		/// ������ Ÿ��Ʋ
		WCHAR				m_WindowsTitle[64];

		/// WS_POPUP ���� ��ü����϶� ���
		BOOL				m_bPopupMode;

		/// ������ Focus ����
		BOOL				m_bFocus;
		/// ������ Active ����
		BOOL				m_bActive;
		/// ������ Ready ����
		BOOL				m_bReady;

		/// ������ Exit ����
		BOOL				m_Exit;

		/// �������� ����
		CRect				m_WindowRect;

		/// ������ ��� ����.
		BOOL				m_bWindowMode;

	protected:
		/// ������ ��������
		VOID	ReSizeWindow(BOOL bWindowMode, int Width, int Height);
		/// Minimize Windows ȣ��
		virtual HRESULT MinimizedWindows();
		/// Restore Windows ȣ��
		virtual HRESULT RestoredWindows();
		/// �����츦 Show �մϴ�.
		VOID	OnShowWindow() 
		{ 
			m_bShow = TRUE;
			if(m_hWnd)
				ShowWindow(m_hWnd, SW_SHOWNA);

			SetForegroundWindow(Nave::hMainWnd);					// Slightly Higher Priority
			SetFocus(Nave::hMainWnd);								// Sets Keyboard Focus To The Window
		}

		/// ����� ������Ʈ�� �ʱ�ȭ �մϴ�. (�����켳���� ����� �Ŀ� ȣ��)
		virtual HRESULT InitObjects()			{ return S_OK; }
		/// ����� ������Ʈ�� �����մϴ�. (���η����� ������(EndProcessȣ����) ȣ��)
		virtual HRESULT ReleaseObjects()		{ return S_OK; }
	};

}