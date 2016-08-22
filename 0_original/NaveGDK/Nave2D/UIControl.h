/** 
 *  @file		UIControl.h
 *  @brief		UI컨트롤 객체의 Super 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFImage.h"
#include "UISprite.h"
#include "UIObject.h"

namespace Nave2D {

	/** 
	 *  @class        UIControl
	 *  @brief        UI컨트롤의 Super 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UIControl : public UIObject
	{
	public:
		// UIControl 생성자
		UIControl();
		/// UIControl 소멸자
		virtual ~UIControl();

	public:
		/**
		 * @brief	컨트롤을 생성합니다.
		 * @param pParent	부모포인트
		 * @param fX		로컬X
		 * @param fY		로컬Y
		 * @param fWidth	가로크기
		 * @param fHeight	세로크기
		 * @param nID		객체 아이디
		 * @warning	fX,fY의 값은 부모의 시작점에서의 로컬좌표이다.
		 * @return	성공여부
		 */
		virtual BOOL	OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID);
		/// 컨트롤을 삭제합니다.
		virtual BOOL	OnDestroy()					{ return TRUE; }

		/// 컨트롤에 Data를 설정합니다.
		VOID			SetData(DWORD dwData) { m_dwData = dwData; }
		/// 컨트롤의 Data를 얻습니다.
		DWORD			GetData() { return m_dwData; }

		/// WM_COMMAND
		virtual VOID OnCommand(int iID, int iEvent, LPARAM lParam) { return; }
		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY) { return FALSE; }	// 이것은 모든게 다받아야한다.
		/// WM_LBUTTONDBLCLK
		virtual BOOL OnLButtonDClick(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_RBUTTONDOWN
		virtual BOOL OnRButtonDown(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_RBUTTONUP
		virtual BOOL OnRButtonUp(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_MOUSEWHEEL
		virtual BOOL OnMouseWheel(int iMouseX, int iMouseY, int zDelta) { return InRect(iMouseX, iMouseY); }

		/// Message Proc
		virtual LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// 오브젝트 고유 스트링
	    virtual const char* ToString()							{ return "UIControl"; }

		/// 오브젝트를 Process처리한다.
		virtual VOID	OnUpdate(float dt = 0.0f)				{ return; }
		/// 오브젝트를 화면에 그린다.
		virtual VOID	OnRender();
		/// 전 컨트롤을 Render 위에 렌더링을 겁니다.
		virtual VOID	OnRenderTop()							{ return; }

		/// 컨트롤의 상태를 얻어옵니다.
		inline BOOL		IsEnable() { return m_bEnable; }

		/// 컨트롤의 상태 활성, 비활성 상태가 FALSE의경우 렌더링은 되지만 이벤트는 발생안한다.
		virtual VOID	SetEnable(BOOL bEnable) { m_bEnable = bEnable; }

		/// 컨트롤의 로컬 좌표를 설정합니다.
		VOID			SetLocal(float fLocalX, float fLocalY);
		/// 컨트롤의 사이즈를 설정합니다.
		virtual VOID	SetSize(float fWidth, float fHeight);

		/// Move Objec 처리
		virtual VOID	OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/// 투명도를 얻어옵니다.
		inline  int		GetOpacity()					{ return (int)m_Sprite.GetOpacity(); }
		/// 투명도를 설정한다.
		inline  VOID	SetOpacity( int iOpacity )		{ m_Sprite.SetOpacity((BYTE)iOpacity); }
		/// 블랜드 모드를 설정합니다.
		virtual VOID	SetBlendMode(UISprite::BLEND BlendMode);

		/// 기본 스킨을 설정합니다.
		virtual VOID	SetSkin(LPSPRITEINFO Skin);
		/// 텍스쳐를 설정합니다.
		virtual VOID	SetTexture(const Nave::String& textureName);
		/// 화면렌더링 영역을 설정합니다.
		virtual VOID	SetShowArea(float fX, float fY, float fWidth, float fHeight);

		/// 컨트롤 Rect를 업데이트 합니다.
		virtual VOID	UpdateControlRect();

	protected:
		/// 컨트롤의 로컬위치 X
		float			m_fLocalX;
		/// 컨트롤의 로컬위치 X
		float			m_fLocalY;

		/// 컨트롤의 활성화 상태
		BOOL			m_bEnable;
		/// 컨트롤의 고유 데이타
		DWORD			m_dwData;

		/// 컨트롤의 스킨 정보
		SPRITEINFO		m_Skin;
		/// 컨트롤의 스프라이트
		UISprite		m_Sprite;
	};

}