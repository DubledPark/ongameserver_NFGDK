/** 
 *  @file		UIWindowDX9.h
 *  @brief		DirectX�� ����ϴ� �����츦 �����մϴ�.
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NFAdapter.h>
#include <NaveFramework/NFVideoDX9.h>
#include <NaveFramework/NFInputManager.h>
#include <NaveFramework/NFFontManager.h>


#include "UIWindow.h"

#include "NFTextureManager.h"
#include "NFImageManager.h"
#include "UIDialogManager.h"

namespace Nave2D { 

	/** 
	 *  @class        UIWindowDX9
	 *  @brief        DirectX�� ���� �����츦 �����մϴ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIWindowDX9 : public UIWindow
	{
	public:
		/// UIWindowDX9 ������
		UIWindowDX9(void);
		/// UIWindowDX9 �Ҹ���
		virtual ~UIWindowDX9(void);

	public:
		/// �������� ��尡 ����˴ϴ�. Windows <-> Full
		virtual VOID ChangeWindowMode(BOOL bWindowsMode);

		/// ���ν����� �����մϴ�.
		virtual HRESULT EndProcess();

		/// ���ν����� �����մϴ�.
		virtual HRESULT StartProcess(BOOL bWindow = TRUE);

		/// Windows Create
		virtual BOOL CreateWindows(HINSTANCE hInstance, int nCmdShow, LPCWSTR Title, int Width, int Height, BOOL PopupMode = TRUE, BOOL WindowMode = TRUE, LPCTSTR Icon = MIS(IDI_APPLICATION));

	protected:
		/// ����� ������Ʈ�� �ʱ�ȭ �մϴ�. (�����켳���� ����� �Ŀ� ȣ��)
		virtual HRESULT InitObjects()			{ return S_OK; }
		/// ����� ������Ʈ�� �����մϴ�. (���η����� ������(EndProcessȣ����) ȣ��)
		virtual HRESULT ReleaseObjects()		{ return S_OK; }

		/// Minimize Windows ȣ��
		virtual HRESULT MinimizedWindows();
		/// Restore Windows ȣ��
		virtual HRESULT RestoredWindows();

		/// ȭ���� ������Ʈ �մϴ�.
		virtual VOID OnUpdate(float dt);
		/// ������Ʈ�� �������մϴ�.
		virtual VOID RenderObject() { }
		/// �ֻ����� ��ü�� �������մϴ�.
		virtual VOID RenderTopmost()  { }

		/// ������Ʈ�� Restored �մϴ�. (D3D ��ü)
		virtual VOID	RestoredObject()			{ }
		/// ȭ���� ������ �մϴ�.
		virtual VOID	OnRender();

		/// Windows MsgProc
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	private:
		/// Video ��ü
		NaveFramework::NFVideoDX9*			m_pVideo;
		/// Input Manager
		NaveFramework::NFInputManager*		m_pInputManager;
		/// Font Manager (���� FreeType ���̺귯���� ����)
		NaveFramework::NFFontManager*		m_pFontManager;
		/// Texture Manager
		NFTextureManager*	m_pTextureManager;
		/// Image Manager
		NFImageManager*		m_pImageManager;
		/// Dialog Manager
		UIDialogManager*	m_pDialogManager;
	};

}