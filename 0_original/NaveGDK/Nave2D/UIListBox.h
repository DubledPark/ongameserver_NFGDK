/** 
 *  @file		UIListBox.h
 *  @brief		����Ʈ �ڽ� ��Ʈ��
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
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
	 *  @brief        ����Ʈ�ڽ��� �����ϰ� �����ϴ� Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIListBox : public UIControl
	{
	public:
		/// UIListBox ������
		UIListBox(void);
		/// UIListBox �Ҹ���
		~UIListBox(void);

	public:
		/**
		 * @brief	����Ʈ�ڽ��� �����մϴ�.
		 * @param pParent	�θ�����Ʈ
		 * @param fX		����X
		 * @param fY		����Y
		 * @param fWidth	����ũ��
		 * @param fHeight	����ũ��
		 * @param nID		��ü ���̵�
		 * @param ItemHeight ����Ʈ�������� ����
		 * @warning	fX,fY�� ���� �θ��� ������������ ������ǥ�̴�.
		 * @return	��������
		 */
		virtual BOOL OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID, int ItemHeight = 16);
		/// ����Ʈ�ڽ��� �����Ѵ�.
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
	    virtual const char* ToString() { return "UIListBox"; }

		/// ����Ʈ�ڽ��� �������մϴ�.
		virtual VOID OnRender();

		/// Move Objec ó��
		virtual VOID OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/**
		 * @brief		�޺��ڽ� ������ ����
		 * @param fWidth	�޺��ڽ� ���� ũ��
		 * @param fHeight   �޺��ڽ� ���� ũ��(�޺�����Ʈ����)
		 */
		virtual VOID SetSize(float fWidth, float fHeight);

		/// ����Ʈ�ڽ��� ����ƮĿ�� ��Ų
		virtual VOID SetCursorSkin(LPSPRITEINFO CursorSkin);
		/// ����Ʈ�ڽ��� ��ũ�� ��Ų
		virtual VOID SetScrollSkin(LPSPRITEINFO ScrollSkin);
		/// ����Ʈ �ڽ��� ��ũ�� Thumb ��Ų
		virtual VOID SetScrollThumbSkin(LPSPRITEINFO ThumbSkin);
		/// ����Ʈ �ڽ��� ��ũ�� Button ��Ų
		virtual VOID SetScrollButtonSkin(LPSPRITEINFO ButtonSkin);

		/// �������� �׸���.
		virtual VOID OnDrawItem(int ItemIndex, RECT& DrawRect);

		/// ��� �������� �����մϴ�.
		virtual VOID ResetContent();
		/// �׸��� �߰��Ѵ�.
		virtual int	 AddItem(const WCHAR* pString, LPARAM pData = NULL);	
		/// �׸��� �����Ѵ�.
		int  RemoveItem(int index);	

		/// ����Ʈ�ڽ��� �������� �����մϴ�.
		VOID SetCurSel(int index);
		/// ����Ʈ�ڽ��� ���õ� �������� �����մϴ�.
		int GetCurSel();
		
		/// �� �׸��� ������ ���Ѵ�.
		int	 GetCount();				
		/// �׸��� ���̸� �����Ѵ�.
		int	 GetItemHeight();			
		/// �׸��� ���̸� �����Ѵ�.
		VOID SetItemHeight(int Height);	

		/// ������ ����Ÿ�� �����Ѵ�.
		LPARAM  GetItemData(int index);	
		/// ������ ����Ÿ�� �����Ѵ�.
		VOID SetItemData(int index, LPARAM Data);	

		/// ȭ�鿡 ���̴� �ֻ��� �ε����� �����Ѵ�.
		VOID SetTopIndex(int index);	
		/// ȭ�鿡 ���̴� �ֻ��� �ε����� �����Ѵ�.
		int  GetTopIndex();				

		/// �ؽ�Ʈ�� �÷��� �����մϴ�.
		VOID SetTextColor(COLORREF TextColor);

		/// �־��� �׸��� ���ڿ� ���̸� �����Ѵ�.
		int	 GetTextLen(int index);		
		/// �־��� �׸��� ���ڿ��� �����Ѵ�.
		VOID GetText(int index, WCHAR* Buff);		
		/// �־��� �׸��� �۾����� ��ǥ�� �����Ѵ�.
		RECT GetItemRect(int index);	

	private:
		/// ����Ʈ�ڽ��� ������ ����ü
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

		/// ������ ����Ʈ
		std::vector<LPLISTITEM>	m_Items;

		/// ��ũ�ѹ��� ����ũ��
		float				m_fScrollWidth;

		/// ������ ��ü�� ����
		int					m_iItemHeight;
		/// �ֻ��� �������� �ε���
		int					m_iTopIndex;
		/// ��ȭ�鿡 ������ �������� ����
		int					m_iViewItemCount;	

		/// ���õ� ������
		int					m_iSelectedItem;
		/// ���콺�� �����ϰ� �ִ� ������
		int					m_iCursorItem;
		/// Ŀ���� ��� ����
		BOOL				m_bShowCursor;

		/// �ؽ�Ʈ �÷�
		COLORREF			m_TextColor;

		/// ����Ʈ�ڽ��� ���̴� ��ũ�ѹ�
		UIScrollBar			m_ScrollBar;

		/// Ŀ���� ��������Ʈ
		UISprite			m_CursorCur;			
		/// ���õ� Ŀ���� ��������Ʈ
		UISprite			m_CursorSel;			
		/// Ŀ�� ��������Ʈ ���� (Cur,Sel Ŀ������)
		SPRITEINFO			m_CursorSkin;
	};

}