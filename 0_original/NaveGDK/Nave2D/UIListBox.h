/** 
 *  @file		UIListBox.h
 *  @brief		리스트 박스 컨트롤
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIControl.h"
#include "NFText.h"
#include <vector>

#include "UIScrollBar.h"

namespace Nave2D { 

	#define COMBO_DROPLIST	0xFFFF
	#define MAX_LISTITEM 256

	/** 
	 *  @class        UIListBox
	 *  @brief        리스트박스을 생성하고 관리하는 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIListBox : public UIControl
	{
	public:
		/// UIListBox 생성자
		UIListBox(void);
		/// UIListBox 소멸자
		~UIListBox(void);

	public:
		/**
		 * @brief	리스트박스를 생성합니다.
		 * @param pParent	부모포인트
		 * @param fX		로컬X
		 * @param fY		로컬Y
		 * @param fWidth	가로크기
		 * @param fHeight	세로크기
		 * @param nID		객체 아이디
		 * @param ItemHeight 리스트아이템의 높이
		 * @warning	fX,fY의 값은 부모의 시작점에서의 로컬좌표이다.
		 * @return	성공여부
		 */
		virtual BOOL OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID, int ItemHeight = 16);
		/// 리스트박스를 삭제한다.
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
	    virtual const char* ToString() { return "UIListBox"; }

		/// 리스트박스를 렌더링합니다.
		virtual VOID OnRender();

		/// Move Objec 처리
		virtual VOID OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/**
		 * @brief		콤보박스 사이즈 설정
		 * @param fWidth	콤보박스 가로 크기
		 * @param fHeight   콤보박스 세로 크기(콤보리스트제외)
		 */
		virtual VOID SetSize(float fWidth, float fHeight);

		/// 리스트박스의 셀렉트커서 스킨
		virtual VOID SetCursorSkin(LPSPRITEINFO CursorSkin);
		/// 리스트박스의 스크롤 스킨
		virtual VOID SetScrollSkin(LPSPRITEINFO ScrollSkin);
		/// 리스트 박스의 스크롤 Thumb 스킨
		virtual VOID SetScrollThumbSkin(LPSPRITEINFO ThumbSkin);
		/// 리스트 박스의 스크롤 Button 스킨
		virtual VOID SetScrollButtonSkin(LPSPRITEINFO ButtonSkin);

		/// 아이템을 그린다.
		virtual VOID OnDrawItem(int ItemIndex, RECT& DrawRect);

		/// 모든 아이템을 삭제합니다.
		virtual VOID ResetContent();
		/// 항목을 추가한다.
		virtual int	 AddItem(const WCHAR* pString, LPARAM pData = NULL);	
		/// 항목을 삭제한다.
		int  RemoveItem(int index);	

		/// 리스트박스의 아이템을 선택합니다.
		VOID SetCurSel(int index);
		/// 리스트박스의 선택된 아이템을 리턴합니다.
		int GetCurSel();
		
		/// 총 항목의 개수를 구한다.
		int	 GetCount();				
		/// 항목의 높이를 조절한다.
		int	 GetItemHeight();			
		/// 항목의 높이를 조절한다.
		VOID SetItemHeight(int Height);	

		/// 아이템 데이타를 조사한다.
		LPARAM  GetItemData(int index);	
		/// 아이템 데이타를 설정한다.
		VOID SetItemData(int index, LPARAM Data);	

		/// 화면에 보이는 최상위 인덱스를 조사한다.
		VOID SetTopIndex(int index);	
		/// 화면에 보이는 최상위 인덱스를 조사한다.
		int  GetTopIndex();				

		/// 텍스트의 컬러를 지정합니다.
		VOID SetTextColor(COLORREF TextColor);

		/// 주어진 항목의 문자열 길이를 조사한다.
		int	 GetTextLen(int index);		
		/// 주어진 항목의 문자열을 조사한다.
		VOID GetText(int index, WCHAR* Buff);		
		/// 주어진 항목의 작업영역 좌표를 조사한다.
		RECT GetItemRect(int index);	

	private:
		/// 리스트박스의 아이템 구조체
		typedef struct LISTITEM
		{
			Nave::String	Title;
			LPARAM			Data;
			NFText			TextOut;
			~LISTITEM()
			{
				TextOut.OnDestroy();
			}
		}*LPLISTITEM;

		/// 아이템 리스트
		std::vector<LPLISTITEM>	m_Items;

		/// 스크롤바의 가로크기
		float				m_fScrollWidth;

		/// 아이템 객체의 높이
		int					m_iItemHeight;
		/// 최상위 아이템의 인덱스
		int					m_iTopIndex;
		/// 한화면에 나오는 아이템의 개수
		int					m_iViewItemCount;	

		/// 선택된 아이템
		int					m_iSelectedItem;
		/// 마우스가 존재하고 있는 아이템
		int					m_iCursorItem;
		/// 커서의 출력 유무
		BOOL				m_bShowCursor;

		/// 텍스트 컬러
		COLORREF			m_TextColor;

		/// 리스트박스에 보이는 스크롤바
		UIScrollBar			m_ScrollBar;

		/// 커서의 스프라이트
		UISprite			m_CursorCur;			
		/// 선택된 커서의 스프라이트
		UISprite			m_CursorSel;			
		/// 커서 스프라이트 정보 (Cur,Sel 커서포함)
		SPRITEINFO			m_CursorSkin;
	};

}