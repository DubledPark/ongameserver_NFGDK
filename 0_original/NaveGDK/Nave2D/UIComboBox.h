/** 
 *  @file		UIComboBox.h
 *  @brief		콤보박스 컨트롤
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIButton.h"
#include "UIEditBox.h"
#include "UIListBox.h"

namespace Nave2D { 

	/** 
	 *  @class        UIComboBox
	 *  @brief        콤보박스 컨트롤
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UIComboBox : public UIEditBox
	{
	public:
		/// UIComboBox 생성자
		UIComboBox(void);
		/// UIComboBox 소멸자
		virtual ~UIComboBox(void);

	public:

		/**
		 * @brief	콤보박스를 생성합니다.
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
		/// 콤보박스를 삭제한다.
		virtual BOOL OnDestroy();

		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY);
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY);
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY);
		/// WM_MOUSEWHEEL
		virtual BOOL OnMouseWheel(int iMouseX, int iMouseY, int zDelta);

		/// Message Proc
		virtual LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// 오브젝트 고유 스트링
	    virtual const char* ToString() { return "UIComboBox"; }

		/// 렌더링을 건다.
		virtual VOID OnRender();

		// 콤보 리스트를 렌더링 한다.
		virtual VOID OnRenderTop();	

		/// Move Objec 처리
		virtual VOID OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/**
		 * @brief		콤보박스 사이즈 설정
		 * @param fWidth	콤보박스 가로 크기
		 * @param fHeight   콤보박스 세로 크기(콤보리스트제외)
		 */
		virtual VOID SetSize(float fWidth, float fHeight);

		/// 콤보박스 기본스킨 설정
		virtual VOID SetSkin(LPSPRITEINFO Skin);
		/// 콤보박스 DropButton스킨
		virtual VOID SetDropButtonSkin(LPSPRITEINFO Skin);
		/// 콤보박스 DropList스킨
		virtual VOID SetDropListSkin(LPSPRITEINFO Skin);
		/// 콤보박스 SelectCursor스킨
		virtual VOID SetCursorSkin(LPSPRITEINFO CursorSkin);
		/// 콤보박스 Scroll스킨
		virtual VOID SetScrollSkin(LPSPRITEINFO ScrollSkin);
		/// 콤보박스 Scroll Thumb 스킨
		virtual VOID SetScrollThumbSkin(LPSPRITEINFO ThumbSkin);
		/// 콤보박스 Scroll Button 스킨
		virtual VOID SetScrollButtonSkin(LPSPRITEINFO ButtonSkin);

		/// 컨트롤 Rect를 업데이트 합니다.
		virtual VOID UpdateControlRect();

		/// 콤보컨트롤 아이템을 초기화 합니다.
		virtual VOID ResetContent();

		/// 새로운 아이템을 추가합니다.
		virtual VOID AddItem(const WCHAR* pString, LPARAM pData = NULL);
		/// 아이템을 삭제합니다.
		virtual VOID RemoveItem(int iIndex);

		/// 아이템 개수를 리턴합니다.
		int GetItemCount();

		/// 커서를 설정합니다.
		VOID SetCurSel(int iIndex);
		/// DropList를 hide 합니다.
		VOID HideDropList();

	private:
		/// 컨트롤 State를 업데이트 합니다.
		VOID UpdateControlState(int State);


	private:
		/// DropList 현재 사이즈
		float			m_fDropListSize;
		/// DropList 최대 사이즈
		float			m_fMaxDropListSize;

		/// Drop Button 사이즈
		float			m_fDropBtnSize;

		/// DropList Enable 플레그
		BOOL			m_bListShow;
		/// DropList 렌더링 위치 
		BOOL			m_bUpperDropList;

		/// 컨트롤 State 상태
		int				m_iState;

		/// Drop Button 스프라이트
		UISprite		m_DropButton;

		/// Drop Button 영역
		CRect			m_DropButtonRect;

		/// Drop Button 스킨
		SPRITEINFO		m_DropButtonSkin;

		/// DropList 객체
		UIListBox		m_DropList;
	};

}