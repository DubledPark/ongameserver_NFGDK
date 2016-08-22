/** 
 *  @file		UIEditBox.h
 *  @brief		����Ʈ �ڽ� ��Ʈ��
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIControl.h"
#include "NFText.h"
#include "UICaret.h"
#include <vector>

namespace Nave2D { 

	/// EditBox���� ����ϴ� Notify �޽��� Define
	#define WM_EDIT_NOTIFY		WM_USER + 0x00ed

	/// MultiLine EditBox���� ����� EditItem ����ü
	typedef struct EDITITEM
	{
		Nave::String	strText;
		NFText			dcText;

		~EDITITEM()
		{
			dcText.OnDestroy();
		}
	}*LPEDITITEM;

	/** 
	 *  @class        UIEditBox
	 *  @brief        ����Ʈ�ڽ��� �����ϰ� �����ϴ� ��Ʈ��
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIEditBox : public UIControl
	{
	public:
		/// UIEditBox ������
		UIEditBox(void);
		/// UIEditBox �Ҹ���
		virtual ~UIEditBox(void);

	public:
		/**
		 * @brief	����Ʈ�ڽ��� �����մϴ�.
		 * @param pParent	�θ�����Ʈ
		 * @param fX		����X
		 * @param fY		����Y
		 * @param fWidth	����ũ��
		 * @param fHeight	����ũ��
		 * @param nID		��ü ���̵�
		 * @warning	fX,fY�� ���� �θ��� ������������ ������ǥ�̴�.
		 * @return	��������
		 */		
		virtual BOOL OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID, DWORD Style = 0);
		/// ����Ʈ�ڽ��� �����Ѵ�.
		virtual BOOL OnDestroy();

		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY);
		/// WM_LBUTTONDCLICK
		virtual BOOL OnLButtonDClick(int iMouseX, int iMouseY);

		/// Edit Widnows Message Proc
		virtual LRESULT MsgProcEx( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// ��Ŀ���� �����.
		virtual VOID SetFocus();
		/// ��Ŀ���� �Ҿ���.
		virtual VOID KillFocus();

		/// ������Ʈ ���� ��Ʈ��
	    virtual const char* ToString() { return "UIEditBox"; }

		/// Editbox�� ��������Ʈ�� ������ �մϴ�.
		virtual VOID OnPaint();

		/// ȭ�鿡 �������� �մϴ�.
		virtual VOID OnRender();

		/// Move Objec ó��
		virtual VOID OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/**
		 * @brief		����Ʈ�ڽ� ������ ����
		 * @param fWidth	����Ʈ�ڽ� ���� ũ��
		 * @param fHeight   ����Ʈ�ڽ� ���� ũ��(�޺�����Ʈ����)
		 */
		virtual VOID SetSize(float fWidth, float fHeight);

		/// ����Ʈ�ڽ� �⺻��Ų ����
		virtual VOID SetSkin(LPSPRITEINFO Skin);

		/// EditBox�� ��� ��ü�� �����մϴ�.
		VOID ResetContent();

		/// �ؽ�Ʈ �÷��� �����մϴ�.
		VOID SetTextColor(COLORREF TextColor);
		/// ĳ���� �÷��� �����մϴ�.
		VOID SetCaretColor(COLORREF CaretColor);

		/// ��Ʈ�� �����մϴ�.
		VOID SetFont(HFONT hFont);
		/// �н����� ���� �����մϴ�.
		VOID SetPasswordMode(BOOL bPassMode = FALSE);

		/// ��Ʈ���� �����մϴ�. (��Ƽ������ �����Ѵ�.)
		VOID SetText(const WCHAR* buff);
		/// ��� ��Ʈ���� ��´�. (��Ƽ������ �����Ѵ�.)
		const Nave::String GetText();
		/// ��Ʈ�� ���̸� ���ɴϴ�.
		int GetTextLen();

		/// Editbox�� ReadOnly ���¾��
		BOOL GetReadOnly() { return m_bReadOnly; };
		/// EditBox�� ReadOnly�� �����մϴ�.
		VOID SetReadOnly(BOOL Mode) { m_bReadOnly = Mode; }

		/// ����Ʈ �ڽ��� �Է� �ִ���ڼ��� �����մϴ�.
		VOID SetLimitText(int Max);

		/// ����Ʈ �ڽ� IME �ڵ� ���
		inline HWND GetIME() { return m_hIME; }

	public:	
		/// ����Ʈ�ڽ� ����Ʈ (����Ŭ�������� Editbox������ �޽��� ó���� �ʿ�)
		static std::map<DWORD, UIEditBox*>	m_EditObject;
		/// ����Ʈ �ڽ��� �߰��Ѵ�.
		static VOID RegisterEdit(UIEditBox* pThis);
		/// ����Ʈ �ڽ��� �����Ѵ�.
		static VOID UnRegisterEdit(UIEditBox* pThis);
		/// ����Ʈ �ڽ� ����Ŭ���� ���ν���
		static LRESULT CALLBACK WndEditProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		/// ��Ʈ���� üũ�մϴ�.
		BOOL CheckString();
		/// ���ο� ���ξ������� �߰��մϴ�. VK_ENTER ó��
		VOID AddLine();
		/// ������ ���ξ������� �����մϴ�. VK_DEL ó��
		VOID DelLine();

		/// ���� �Է�â�� ���ڿ��� �����Ѵ�.
		VOID SetString(const WCHAR* buff);
		/// ���� �Է�â�� ���ڿ��� �����´�.
		int GetString(WCHAR* buff);
		/// ���� �Է�â�� ���ڿ� ũ�⸦ ���´�.
		int GetStringLen();

	private:
		/// ����Ʈ�ڽ� IME Windows
		HWND		m_hIME;
		/// ����Ʈ�ڽ� ��Ʈ
		HFONT		m_hFont;

		/// ����Ʈ�ڽ� ��Ÿ��
		DWORD		m_dwStyle;
		/// ����Ʈ�ڽ� ��Ƽ���ο��� �Ѷ����� ����
		float		m_fItemHeight;

		/// ����Ʈ�ڽ����� �Էµǰ� �ִ� ���ڿ�
		NFText		m_Text;

		/// ��Ƽ���� ����Ʈ�ڽ����� �Է��� �Ϸ�� �Ѷ���
		std::vector<LPEDITITEM>	m_vecItem;

		/// ����Ʈ�ڽ��� ĳ��
		UICaret		m_Caret;
		/// ĳ�� ũ��
		CSize		m_CaretSize;

		/// IME COMPOSITION ����
		BOOL		m_bIMEComp;
		
		/// ����Ʈ �ڽ� ��Ŀ������
		BOOL		m_bFocus;

		/// �б����� ����ǥ��
		BOOL		m_bReadOnly;
		/// �н����� ��� ����ǥ��
		BOOL		m_bPasswordMode;

		/// �ֻ����� ����� �������� (��ũ�ѹٰ� ����ɰ�� ���)
		int			m_iTopLine;

		/// �ؽ�Ʈ �÷�
		COLORREF	m_TextColor;

		/// ����Ʈ�ڽ� ����Ŭ���̿� ���ν�����ü
		WNDPROC		m_OldWndProc;		
	};

}