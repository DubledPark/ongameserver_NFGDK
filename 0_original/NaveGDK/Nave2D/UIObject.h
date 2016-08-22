/** 
 *  @file		UIObject.h
 *  @brief		UI��Ʈ�� ��ü�� Super Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

namespace Nave2D {

	/** 
	 *  @class        UIObject
	 *  @brief        UI��Ʈ�� ��ü�� Super Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIObject : public Nave::NFObject
	{
	public:
		/// UIObject ������
		UIObject(void);
		/// UIObject �Ҹ���
		virtual ~UIObject(void);

	public:
		/// �������̵�
		inline DWORD GetID() const { return m_dID; };
		/// �θ� ������Ʈ
		inline UIObject* GetParent() const { return m_hParent; }

		/// �ڱ� �ڽſ� �ڵ��� �����Ѵ�.
		inline VOID	OnSetParent(UIObject* pThis) 
		{
			m_hParent = pThis;	
		}
		
		/// ��ġ���� �����մϴ�.
		virtual VOID SetPos(float fX, float fY)
		{
			m_fX = fX;
			m_fY = fY;
		}

		/// ������Ʈ ����� �����մϴ�.
		virtual VOID SetSize(float fWidth, float fHeight)
		{
			m_fWidth = fWidth;
			m_fHeight = fHeight;
		}

		/// X��ǥ 
		inline float GetX() const	{ return m_fX; }
		/// Y��ǥ 
		inline float GetY()	const	{ return m_fY; }
		/// ������Ʈ ����ũ��
		inline float GetWidth() const	{ return m_fWidth; }
		/// ������Ʈ ����ũ��
		inline float GetHeight() const	{ return m_fHeight; }

		/// ��ǥ���� ������Ʈ �ȿ� ����ִ��� Ȯ��.
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
	    
		/// Ȱ��ȭ ���¸� ���ɴϴ�.
		inline BOOL		IsVisible() { return m_bVisible; }

		/// ��Ʈ���� Ȱ��ȭ ���¸� ó���մϴ�. FALSE �ϰ�� �̺�Ʈó���� �������� ���� �ʽ��ϴ�.
		virtual VOID	SetVisible(BOOL bVisible) 
		{ 
			m_bVisible = bVisible;
		}

		/// ������Ʈ�� �����մϴ�.
		virtual BOOL OnCreate(UIObject* pParent)		{ OnSetParent(pParent); return TRUE; }
		/// ������Ʈ�� �����մϴ�.
		virtual BOOL OnDestroy()						{ return TRUE; }
		
		/// ������Ʈ�� Processó���Ѵ�.
		virtual VOID OnUpdate(float dt = 0.0f)			{ return; }
		/// ������Ʈ�� ȭ�鿡 �׸���.
		virtual VOID OnRender()							{ return; }

		/// ������Ʈ ���� ��Ʈ��
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
		/// ������ ������Ʈ�� �������̵� (�޽��� ���޽� ���)
		DWORD		m_dID;			

		/// Ȱ��ȭ ��Ȱ��ȭ
		BOOL		m_bVisible;		

		/// �θ� ������ �ڵ�
		UIObject*	m_hParent;		

		/// ���� ��ġX
		float		m_fX;
		/// ���� ��ġY
		float		m_fY;				
		/// ���� ����ũ��
		float		m_fWidth;
		/// ���� ����ũ��
		float		m_fHeight;	
	};

}