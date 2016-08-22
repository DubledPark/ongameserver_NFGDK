/** 
 *  @file		UIObject.h
 *  @brief		UI컨트롤 객체의 Super 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

namespace Nave2D {

	/** 
	 *  @class        UIObject
	 *  @brief        UI컨트롤 객체의 Super 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIObject : public Nave::NFObject
	{
	public:
		/// UIObject 생성자
		UIObject(void);
		/// UIObject 소멸자
		virtual ~UIObject(void);

	public:
		/// 고유아이디
		inline DWORD GetID() const { return m_dID; };
		/// 부모 오브젝트
		inline UIObject* GetParent() const { return m_hParent; }

		/// 자기 자신에 핸들을 셋팅한다.
		inline VOID	OnSetParent(UIObject* pThis) 
		{
			m_hParent = pThis;	
		}
		
		/// 위치값을 설정합니다.
		virtual VOID SetPos(float fX, float fY)
		{
			m_fX = fX;
			m_fY = fY;
		}

		/// 오브젝트 사이즈를 설정합니다.
		virtual VOID SetSize(float fWidth, float fHeight)
		{
			m_fWidth = fWidth;
			m_fHeight = fHeight;
		}

		/// X좌표 
		inline float GetX() const	{ return m_fX; }
		/// Y좌표 
		inline float GetY()	const	{ return m_fY; }
		/// 오브젝트 가로크기
		inline float GetWidth() const	{ return m_fWidth; }
		/// 오브젝트 세로크기
		inline float GetHeight() const	{ return m_fHeight; }

		/// 좌표값이 오브젝트 안에 들어있는지 확인.
		BOOL InRect(int iMouseX, int iMouseY)
		{
			iMouseX -= (int)m_fX;
			iMouseY -= (int)m_fY;

			if(0 < iMouseX && iMouseX < m_fWidth )
			{
				if(0 < iMouseY && iMouseY < m_fHeight )
				{
					return TRUE;
				}
			}
			return FALSE;
		}
	    
		/// 활성화 상태를 얻어옵니다.
		inline BOOL		IsVisible() { return m_bVisible; }

		/// 컨트롤의 활성화 상태를 처리합니다. FALSE 일경우 이벤트처리와 렌더링을 하지 않습니다.
		virtual VOID	SetVisible(BOOL bVisible) 
		{ 
			m_bVisible = bVisible;
		}

		/// 오브젝트를 생성합니다.
		virtual BOOL OnCreate(UIObject* pParent)		{ OnSetParent(pParent); return TRUE; }
		/// 오브젝트를 삭제합니다.
		virtual BOOL OnDestroy()						{ return TRUE; }
		
		/// 오브젝트를 Process처리한다.
		virtual VOID OnUpdate(float dt = 0.0f)			{ return; }
		/// 오브젝트를 화면에 그린다.
		virtual VOID OnRender()							{ return; }

		/// 오브젝트 고유 스트링
	    virtual const char* ToString()					{ return "UIObject"; }

		/// WM_COMMAND
		virtual VOID OnCommand(int iID, int iEvent, LPARAM lParam) { return; }
		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONDBLCLK
		virtual BOOL OnLButtonDClick(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_RBUTTONDOWN
		virtual BOOL OnRButtonDown(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_RBUTTONUP
		virtual BOOL OnRButtonUp(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_MOUSEWHEEL
		virtual BOOL OnMouseWheel(int iMouseX, int iMouseY, int zDelta) { return FALSE; }

		/// Message Proc
		virtual LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		/// 생성한 오브젝트의 고유아이디 (메시지 전달시 사용)
		DWORD		m_dID;			

		/// 활성화 비활성화
		BOOL		m_bVisible;		

		/// 부모 윈도우 핸들
		UIObject*	m_hParent;		

		/// 현재 위치X
		float		m_fX;
		/// 현재 위치Y
		float		m_fY;				
		/// 현재 가로크기
		float		m_fWidth;
		/// 현재 세로크기
		float		m_fHeight;	
	};

}