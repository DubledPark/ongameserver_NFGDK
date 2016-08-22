/** 
 *  @file		UIWindowDX9.h
 *  @brief		DirectX를 사용하는 윈도우를 생성합니다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
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
	 *  @brief        DirectX가 붙은 윈도우를 생성합니다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIWindowDX9 : public UIWindow
	{
	public:
		/// UIWindowDX9 생성자
		UIWindowDX9(void);
		/// UIWindowDX9 소멸자
		virtual ~UIWindowDX9(void);

	public:
		/// 윈도우의 모드가 변경됩니다. Windows <-> Full
		virtual VOID ChangeWindowMode(BOOL bWindowsMode);

		/// 프로시져를 종료합니다.
		virtual HRESULT EndProcess();

		/// 프로시져를 실행합니다.
		virtual HRESULT StartProcess(BOOL bWindow = TRUE);

		/// Windows Create
		virtual BOOL CreateWindows(HINSTANCE hInstance, int nCmdShow, LPCWSTR Title, int Width, int Height, BOOL PopupMode = TRUE, BOOL WindowMode = TRUE, LPCTSTR Icon = MIS(IDI_APPLICATION));

	protected:
		/// 사용자 오브젝트를 초기화 합니다. (윈도우설정이 종료됀 후에 호출)
		virtual HRESULT InitObjects()			{ return S_OK; }
		/// 사용자 오브젝트를 삭제합니다. (메인루프가 끊난후(EndProcess호출후) 호출)
		virtual HRESULT ReleaseObjects()		{ return S_OK; }

		/// Minimize Windows 호출
		virtual HRESULT MinimizedWindows();
		/// Restore Windows 호출
		virtual HRESULT RestoredWindows();

		/// 화면을 업데이트 합니다.
		virtual VOID OnUpdate(float dt);
		/// 오브젝트를 렌더링합니다.
		virtual VOID RenderObject() { }
		/// 최상위에 객체를 렌더링합니다.
		virtual VOID RenderTopmost()  { }

		/// 오브젝트를 Restored 합니다. (D3D 객체)
		virtual VOID	RestoredObject()			{ }
		/// 화면을 렌더링 합니다.
		virtual VOID	OnRender();

		/// Windows MsgProc
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	private:
		/// Video 객체
		NaveFramework::NFVideoDX9*			m_pVideo;
		/// Input Manager
		NaveFramework::NFInputManager*		m_pInputManager;
		/// Font Manager (추후 FreeType 라이브러리로 변경)
		NaveFramework::NFFontManager*		m_pFontManager;
		/// Texture Manager
		NFTextureManager*	m_pTextureManager;
		/// Image Manager
		NFImageManager*		m_pImageManager;
		/// Dialog Manager
		UIDialogManager*	m_pDialogManager;
	};

}