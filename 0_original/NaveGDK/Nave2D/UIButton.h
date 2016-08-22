/** 
 *  @file		UIButton.h
 *  @brief		��ư ��ü
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIControl.h"

namespace Nave2D { 

	/** 
	 *  @class        UIButton
	 *  @brief        ��ư�� �����ϰ� �����ϴ� Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UIButton : public UIControl
	{
	public:
		/// UIButton ������
		UIButton(void);
		/// UIButton �Ҹ���
		virtual ~UIButton(void);

	public:

		/**
		 * @brief	��ư�� �����մϴ�.
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
		/// ��ư�� �����Ѵ�.
		virtual BOOL OnDestroy();

		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY);
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY);
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY);

		/// Message Proc
		virtual LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// ������Ʈ ���� ��Ʈ��
		virtual const char* ToString()  { return "UIButton"; }

		/// Enable ó��
		virtual VOID	SetEnable(BOOL bEnable);

		/// Move Objec ó��
		virtual VOID	OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);
		
		/// ������ Area�� �����Ѵ�.
		virtual VOID	SetShowArea(float fX, float fY, float fWidth, float fHeight);

		/// �������� ����� Skin�� �����մϴ�.
		virtual VOID	SetSkin(LPSPRITEINFO Skin);

		/// ��Ʈ�� Rect�� ������Ʈ �մϴ�.
		virtual VOID	UpdateControlRect();
		
		/// ��Ʈ�� State �� ó���Ѵ�.
		inline int		GetState()						{ return m_iState; }

	protected:
		/// ��Ʈ�� ���� State 0 = Normal, 1 = Over, 2 = Click
		int				m_iState;			
		/// ��ư Ŭ�� ����
		BOOL			m_bButtonClick;
	};

}