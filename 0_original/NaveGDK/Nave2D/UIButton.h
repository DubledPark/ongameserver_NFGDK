/** 
 *  @file		UIButton.h
 *  @brief		버튼 객체
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIControl.h"

namespace Nave2D { 

	/** 
	 *  @class        UIButton
	 *  @brief        버튼을 생성하고 관리하는 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UIButton : public UIControl
	{
	public:
		/// UIButton 생성자
		UIButton(void);
		/// UIButton 소멸자
		virtual ~UIButton(void);

	public:

		/**
		 * @brief	버튼을 생성합니다.
		 * @param pParent	부모포인트
		 * @param fX		로컬X
		 * @param fY		로컬Y
		 * @param fWidth	가로크기
		 * @param fHeight	세로크기
		 * @param nID		객체 아이디
		 * @warning	fX,fY의 값은 부모의 시작점에서의 로컬좌표이다.
		 * @return	성공여부
		 */
		virtual BOOL OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID);
		/// 버튼을 삭제한다.
		virtual BOOL OnDestroy();

		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY);
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY);
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY);

		/// Message Proc
		virtual LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// 오브젝트 고유 스트링
		virtual const char* ToString()  { return "UIButton"; }

		/// Enable 처리
		virtual VOID	SetEnable(BOOL bEnable);

		/// Move Objec 처리
		virtual VOID	OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);
		
		/// 렌더링 Area를 설정한다.
		virtual VOID	SetShowArea(float fX, float fY, float fWidth, float fHeight);

		/// 렌더링시 사용할 Skin을 설정합니다.
		virtual VOID	SetSkin(LPSPRITEINFO Skin);

		/// 컨트롤 Rect를 업데이트 합니다.
		virtual VOID	UpdateControlRect();
		
		/// 컨트롤 State 를 처리한다.
		inline int		GetState()						{ return m_iState; }

	protected:
		/// 컨트롤 상태 State 0 = Normal, 1 = Over, 2 = Click
		int				m_iState;			
		/// 버튼 클릭 상태
		BOOL			m_bButtonClick;
	};

}