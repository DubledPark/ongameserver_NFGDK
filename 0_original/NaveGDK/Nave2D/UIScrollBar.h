/** 
 *  @file		UIScrollBar.h
 *  @brief		��ũ�� �� ��Ʈ��
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
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
	 *  @brief        ��ũ�ѹٸ� �����ϰ� �����ϴ� Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIScrollBar : public UIControl
	{
	public:
		/// UIScrollBar ������
		UIScrollBar(void);
		/// UIScrollBar �Ҹ���
		~UIScrollBar(void);

	public:
		/**
		 * @brief	��ũ�ѹٸ� �����մϴ�.
		 * @param pParent	�θ�����Ʈ
		 * @param fX		����X
		 * @param fY		����Y
		 * @param fWidth	����ũ��
		 * @param fHeight	����ũ��
		 * @param nID		��ü ���̵�
		 * @warning	fX,fY�� ���� �θ��� ������������ ������ǥ�̴�.
		 * @return	��������
		 */
		virtual BOOL OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID);
		/// ��ũ�ѹٸ� �����Ѵ�.
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

		/// ������Ʈ ���� ��Ʈ��
	    virtual const char* ToString() { return "UIScrollBar"; }

		/// �������� �Ǵ�.
		virtual VOID OnRender();

		/// Move Objec ó��
		virtual VOID OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/// ��ũ�ѹ��� �⺻ ��Ų (���)
		virtual VOID SetSkin(LPSPRITEINFO Skin);
		/// ��ũ�ѹ��� Thumb ��Ų
		virtual VOID SetThumbSkin(LPSPRITEINFO ThumbSkin);
		/// ��ũ�ѹ��� Button ��Ų
		virtual VOID SetButtonSkin(LPSPRITEINFO ButtonSkin);
		/// ��ũ�ѹ� �⺻��Ų�� ȭ�鷻���� ������ �����մϴ�
		virtual VOID SetShowArea(float fX, float fY, float fWidth, float fHeight);

		/// ��Ʈ�� Rect�� ������Ʈ �մϴ�.
		virtual VOID UpdateControlRect();

		/// ��ũ���� Range�� �����մϴ�.
		VOID SetRange( int iStart, int iEnd );

		/// ��ũ�ѹ��� ��ũ�Ѹ���(Thumb)�� ũ�⸦ �����մϴ�.
		VOID SetScrollThumbHold(BOOL bHold, float fScrollThumbHeight = SCROLLTHUMB_HEIGHT);

		/// ��ũ���� ��ġ�� �̵��մϴ�.
		VOID SetPosition( int iPosition );

		/// ��ũ���� ThumbPos�� ���մϴ�.
		inline int GetThumbPos() const { return m_iPosition; }
		/// ��ũ�� ���� Thumb��ġ�� �����մϴ�.
		VOID SetThumbPos( int iPosition ) { m_iPosition = iPosition; UpdateThumbRect(); }

		/// ��ũ�ѿ��� ȭ�鿡 �������� �������� ũ��
		inline int GetPageSize() const { return m_iPageSize; }
		/// ��ũ���� ���������� �����մϴ�.
		VOID SetPageSize( int iPageSize ) { m_iPageSize = iPageSize; UpdateThumbRect(); }

		/// ��ũ���� �̵��մϴ�.
		VOID Scroll(int iDelta);

		/// ��ũ���� Ȱ��ȭ ���¸� ���մϴ�.
		inline BOOL IsShowScroll() { return m_bShowScroll; }

	private:
		/// ��ũ���� ������ ������Ʈ �մϴ�.
		VOID UpdateCap();

		/// ��ũ���� Thumb ������ ������Ʈ �մϴ�.
		VOID UpdateThumbRect();

		/// ��ũ�� ��Ʈ���� ���¸� �����մϴ�. (Button)
		VOID UpdateControl(int iType, int State);

	private:
		enum{
			ST_NONE,		/// ��ũ�� ���¾���
			ST_UP,			/// ��ũ�� UpButton����
			ST_DOWN,		/// ��ũ�� DownButto����
			ST_THUMB		/// ��ũ�� Thumb����
		};

		/// ��ũ���� ǥ������
		BOOL		m_bShowScroll;		

		/// Thumb�� Drag ����
		BOOL		m_bThumbDrag;
		/// Thumb�� Drag OffSet ����
		int			m_iThumbOffsetY;

		/// ��ũ���� Thumb ��������Ʈ
		UISprite	m_Thumb;		
		/// ��ũ���� Up ��ư ��������Ʈ
		UISprite	m_UpButton;		
		/// ��ũ���� Down ��ư ��������Ʈ
		UISprite	m_DownButton;	

		/// MouseMove���. � �κ��� Ȱ��ȭ �Ƴ�. 0:None, 1:Up, 1:Down, 2:Thumb
		int			m_iMoveState;	
		/// MouseDown���. � �κ��� Ȱ��ȭ �Ƴ�. 0:None, 1:Up, 1:Down, 2:Thumb
		int			m_iDownState;   
		/// ��ũ���� ���� ����
		int			m_iState;

		/// Track�� ����
		CRect		m_TrackRect;
		/// Thumb�� ����
		CRect		m_ThumbRect;
		/// UpButton�� ����
		CRect		m_UpButtonRect;
		/// DownButton�� ����
		CRect		m_DownButtonRect;

		/// ��ũ���� ���� ������
		int			m_iPosition;	
		/// ȭ�鿡 �������� �������� ũ��
		int			m_iPageSize;	
		/// ������ġ
		int			m_iStart;		
		/// ������ġ
		int			m_iEnd;			

		/// ��ũ�ѹ�ư�� ���̰�
		float		m_fScrollBtn_Height;
		/// Thumb�� ���̰�
		float		m_fScrollThumb_Height;
		/// Thumb�� Hold ����
		BOOL		m_bScrollThumbHold;

		/// Ŀ����ġ
		CPoint		m_ptLastMouse;	

		/// Thumb�� ��Ų����
		SPRITEINFO	m_ThumbSkin;
		/// Button�� ��Ų����
		SPRITEINFO	m_ButtonSkin;
		
	};

}