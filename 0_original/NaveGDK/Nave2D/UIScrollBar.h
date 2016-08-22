/** 
 *  @file		UIScrollBar.h
 *  @brief		스크롤 바 컨트롤
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIControl.h"

namespace Nave2D { 

	#define SCROLLBTN_HEIGHT 18
	#define SCROLLTHUMB_HEIGHT 22
	#define SCROLL_MINTHUMBSIZE 10

	/** 
	 *  @class        UIScrollBar
	 *  @brief        스크롤바를 생성하고 관리하는 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIScrollBar : public UIControl
	{
	public:
		/// UIScrollBar 생성자
		UIScrollBar(void);
		/// UIScrollBar 소멸자
		~UIScrollBar(void);

	public:
		/**
		 * @brief	스크롤바를 생성합니다.
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
		/// 스크롤바를 삭제한다.
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
		LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// 오브젝트 고유 스트링
	    virtual const char* ToString() { return "UIScrollBar"; }

		/// 렌더링을 건다.
		virtual VOID OnRender();

		/// Move Objec 처리
		virtual VOID OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/// 스크롤바의 기본 스킨 (배경)
		virtual VOID SetSkin(LPSPRITEINFO Skin);
		/// 스크롤바의 Thumb 스킨
		virtual VOID SetThumbSkin(LPSPRITEINFO ThumbSkin);
		/// 스크롤바의 Button 스킨
		virtual VOID SetButtonSkin(LPSPRITEINFO ButtonSkin);
		/// 스크롤바 기본스킨의 화면렌더링 영역을 설정합니다
		virtual VOID SetShowArea(float fX, float fY, float fWidth, float fHeight);

		/// 컨트롤 Rect를 업데이트 합니다.
		virtual VOID UpdateControlRect();

		/// 스크롤의 Range를 설정합니다.
		VOID SetRange( int iStart, int iEnd );

		/// 스크롤바의 스크롤막대(Thumb)의 크기를 고정합니다.
		VOID SetScrollThumbHold(BOOL bHold, float fScrollThumbHeight = SCROLLTHUMB_HEIGHT);

		/// 스크롤의 위치로 이동합니다.
		VOID SetPosition( int iPosition );

		/// 스크롤의 ThumbPos를 구합니다.
		inline int GetThumbPos() const { return m_iPosition; }
		/// 스크롤 바의 Thumb위치를 조절합니다.
		VOID SetThumbPos( int iPosition ) { m_iPosition = iPosition; UpdateThumbRect(); }

		/// 스크롤에서 화면에 보여지는 사이즈의 크기
		inline int GetPageSize() const { return m_iPageSize; }
		/// 스크롤의 페이지값을 설정합니다.
		VOID SetPageSize( int iPageSize ) { m_iPageSize = iPageSize; UpdateThumbRect(); }

		/// 스크롤을 이동합니다.
		VOID Scroll(int iDelta);

		/// 스크롤의 활성화 상태를 구합니다.
		inline BOOL IsShowScroll() { return m_bShowScroll; }

	private:
		/// 스크롤의 영역을 업데이트 합니다.
		VOID UpdateCap();

		/// 스크롤의 Thumb 영역을 업데이트 합니다.
		VOID UpdateThumbRect();

		/// 스크롤 컨트롤의 상태를 갱신합니다. (Button)
		VOID UpdateControl(int iType, int State);

	private:
		enum{
			ST_NONE,		/// 스크롤 상태없음
			ST_UP,			/// 스크롤 UpButton상태
			ST_DOWN,		/// 스크롤 DownButto상태
			ST_THUMB		/// 스크롤 Thumb상태
		};

		/// 스크롤의 표시유무
		BOOL		m_bShowScroll;		

		/// Thumb의 Drag 상태
		BOOL		m_bThumbDrag;
		/// Thumb의 Drag OffSet 정보
		int			m_iThumbOffsetY;

		/// 스크롤의 Thumb 스프라이트
		UISprite	m_Thumb;		
		/// 스크롤의 Up 버튼 스프라이트
		UISprite	m_UpButton;		
		/// 스크롤의 Down 버튼 스프라이트
		UISprite	m_DownButton;	

		/// MouseMove사용. 어떤 부분이 활성화 됐냐. 0:None, 1:Up, 1:Down, 2:Thumb
		int			m_iMoveState;	
		/// MouseDown사용. 어떤 부분이 활성화 됐냐. 0:None, 1:Up, 1:Down, 2:Thumb
		int			m_iDownState;   
		/// 스크롤의 현재 상태
		int			m_iState;

		/// Track의 영역
		CRect		m_TrackRect;
		/// Thumb의 영역
		CRect		m_ThumbRect;
		/// UpButton의 영역
		CRect		m_UpButtonRect;
		/// DownButton의 영역
		CRect		m_DownButtonRect;

		/// 스크롤의 현재 포지션
		int			m_iPosition;	
		/// 화면에 보여지는 사이즈의 크기
		int			m_iPageSize;	
		/// 시작위치
		int			m_iStart;		
		/// 종료위치
		int			m_iEnd;			

		/// 스크롤버튼의 높이값
		float		m_fScrollBtn_Height;
		/// Thumb의 높이값
		float		m_fScrollThumb_Height;
		/// Thumb의 Hold 상태
		BOOL		m_bScrollThumbHold;

		/// 커서위치
		CPoint		m_ptLastMouse;	

		/// Thumb의 스킨상태
		SPRITEINFO	m_ThumbSkin;
		/// Button의 스킨상태
		SPRITEINFO	m_ButtonSkin;
		
	};

}