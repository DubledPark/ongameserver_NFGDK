/** 
 *  @file		UIComboBox.h
 *  @brief		�޺��ڽ� ��Ʈ��
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIButton.h"
#include "UIEditBox.h"
#include "UIListBox.h"

namespace Nave2D { 

	/** 
	 *  @class        UIComboBox
	 *  @brief        �޺��ڽ� ��Ʈ��
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UIComboBox : public UIEditBox
	{
	public:
		/// UIComboBox ������
		UIComboBox(void);
		/// UIComboBox �Ҹ���
		virtual ~UIComboBox(void);

	public:

		/**
		 * @brief	�޺��ڽ��� �����մϴ�.
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
		/// �޺��ڽ��� �����Ѵ�.
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

		/// ������Ʈ ���� ��Ʈ��
	    virtual const char* ToString() { return "UIComboBox"; }

		/// �������� �Ǵ�.
		virtual VOID OnRender();

		// �޺� ����Ʈ�� ������ �Ѵ�.
		virtual VOID OnRenderTop();	

		/// Move Objec ó��
		virtual VOID OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/**
		 * @brief		�޺��ڽ� ������ ����
		 * @param fWidth	�޺��ڽ� ���� ũ��
		 * @param fHeight   �޺��ڽ� ���� ũ��(�޺�����Ʈ����)
		 */
		virtual VOID SetSize(float fWidth, float fHeight);

		/// �޺��ڽ� �⺻��Ų ����
		virtual VOID SetSkin(LPSPRITEINFO Skin);
		/// �޺��ڽ� DropButton��Ų
		virtual VOID SetDropButtonSkin(LPSPRITEINFO Skin);
		/// �޺��ڽ� DropList��Ų
		virtual VOID SetDropListSkin(LPSPRITEINFO Skin);
		/// �޺��ڽ� SelectCursor��Ų
		virtual VOID SetCursorSkin(LPSPRITEINFO CursorSkin);
		/// �޺��ڽ� Scroll��Ų
		virtual VOID SetScrollSkin(LPSPRITEINFO ScrollSkin);
		/// �޺��ڽ� Scroll Thumb ��Ų
		virtual VOID SetScrollThumbSkin(LPSPRITEINFO ThumbSkin);
		/// �޺��ڽ� Scroll Button ��Ų
		virtual VOID SetScrollButtonSkin(LPSPRITEINFO ButtonSkin);

		/// ��Ʈ�� Rect�� ������Ʈ �մϴ�.
		virtual VOID UpdateControlRect();

		/// �޺���Ʈ�� �������� �ʱ�ȭ �մϴ�.
		virtual VOID ResetContent();

		/// ���ο� �������� �߰��մϴ�.
		virtual VOID AddItem(const WCHAR* pString, LPARAM pData = NULL);
		/// �������� �����մϴ�.
		virtual VOID RemoveItem(int iIndex);

		/// ������ ������ �����մϴ�.
		int GetItemCount();

		/// Ŀ���� �����մϴ�.
		VOID SetCurSel(int iIndex);
		/// DropList�� hide �մϴ�.
		VOID HideDropList();

	private:
		/// ��Ʈ�� State�� ������Ʈ �մϴ�.
		VOID UpdateControlState(int State);


	private:
		/// DropList ���� ������
		float			m_fDropListSize;
		/// DropList �ִ� ������
		float			m_fMaxDropListSize;

		/// Drop Button ������
		float			m_fDropBtnSize;

		/// DropList Enable �÷���
		BOOL			m_bListShow;
		/// DropList ������ ��ġ 
		BOOL			m_bUpperDropList;

		/// ��Ʈ�� State ����
		int				m_iState;

		/// Drop Button ��������Ʈ
		UISprite		m_DropButton;

		/// Drop Button ����
		CRect			m_DropButtonRect;

		/// Drop Button ��Ų
		SPRITEINFO		m_DropButtonSkin;

		/// DropList ��ü
		UIListBox		m_DropList;
	};

}