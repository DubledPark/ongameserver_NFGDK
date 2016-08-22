/** 
 *  @file		UIDialog.h
 *  @brief		���̾�α� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIObject.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UIStatic.h"
#include "UIScrollBar.h"
#include "UIListBox.h"
#include "UIEditBox.h"
#include "UIComboBox.h"
#include <vector>

namespace Nave2D {

	/** 
	 *  @class        UIDialog
	 *  @brief        ���̾�α� Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIDialog : public UIObject
	{
	public:
		/// UIDialog ������
		UIDialog(void);
		/// UIDialog ������
		UIDialog(const Nave::String& name);
		/// UIDialog �Ҹ���
		virtual ~UIDialog(void);

	public:
		/// ���̾�α׸� �����մϴ�.
		virtual BOOL OnCreate(UIObject* pParent)		{ OnSetParent(pParent); OnMove(m_fX, m_fY); return TRUE; }
		/// ���̾�α׸� �����մϴ�.
		virtual BOOL OnDestroy()						{ return TRUE; }

		/// WM_SCALE
		virtual VOID OnScale() { return; }
		/// WM_COMMAND
		virtual VOID OnCommand(int iID, int iEvent, LPARAM lParam) { return; }
		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONDBLCLK
		virtual BOOL OnLButtonDClick(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
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

		/// ������Ʈ ���� ��Ʈ��
	    virtual const char* ToString() { return "UIDialog"; }

		/// ������Ʈ�� Processó���Ѵ�.
		virtual VOID	OnUpdate(float dt = 0.0f);
		/// ������Ʈ�� ȭ�鿡 �׸���.
		virtual VOID	OnRender();

		/// ���̾�α׿� ��Ų�� �����մϴ�.
		virtual VOID	SetSkin(LPSPRITEINFO Skin);
		/// ���̾�α��� �ؽ��ĸ� �ε��մϴ�.
		virtual VOID	SetTexture(const Nave::String& textureName);
		/// �ؽ��Ŀ��� ȭ�鿡 �������� ������ �����մϴ�.
		virtual VOID	SetShowArea(float fX, float fY, float fWidth, float fHeight);

		/// ���̾�α׾ȿ� ���콺�� �������� ��Ŀ���� �����մϴ�.
		virtual BOOL	OnSetFocus(int iMouseX, int iMouseY);

		/// ���̾�α׸� �̵��մϴ�.
		virtual VOID	OnMove(float fX, float fY);
		/// ���̾�α׸� Show �մϴ�.
		virtual VOID	OnShow();
		/// ���̾�α׸� Hide �մϴ�.
		virtual VOID	OnHide();

		/// Modal �������� ���̾�α׸� �����մϴ�.
		VOID			SetModalFlag(BOOL Mode);
		/// ���� ���̾�αװ� Modal �������� ���ɴϴ�.
		inline BOOL		IsModal() const { return m_bModalMode; }

		/// ���̾�α��� ���� �̸��� ���ɴϴ�.
		inline const Nave::String& GetName() const;
		/// ���̾�α��� ���� �̸��� �����մϴ�.
		inline VOID		SetName(Nave::String& name);

		/// ���̾�α��� ������ ���ɴϴ�.
		inline int		GetOpacity()					{ return (int)m_Sprite.GetOpacity(); }
		/// ���̾�α��� ������ �����մϴ�.
		inline VOID		SetOpacity( int iOpacity )		{ m_Sprite.SetOpacity((BYTE)iOpacity); }
		/// ���̾�α��� Blend ��带 �����մϴ�.
		virtual VOID	SetBlendMode(UISprite::BLEND BlendMode);

		/// Index�� �ش��ϴ� ��Ʈ���� �����մϴ�.
	    UIControl*		GetControl( int iIndex );
		/// pt�� ��ġ�� ��Ʈ���� �����մϴ�.
	    UIControl*		GetControlAtPoint( CPoint pt );

		/// Index�� �ش��ϴ� ��Ʈ���� Ȱ��ȭ ���¸� �����ϴ�.
		BOOL			GetControlVisible( int iIndex );
		/// Index�� �ش��ϴ� ��Ʈ���� Ȱ��ȭ ���¸� ó���մϴ�.
	    VOID			SetControlVisible( int iIndex, BOOL bVisible );

		/// ��ư�� �߰��մϴ�.
		UIButton*		AddButton( DWORD nID, float fX, float fY, float fWidth, float fHeight); 
		/// üũ�ڽ��� �߰��մϴ�.
		UICheckBox*		AddCheckBox( DWORD nID, float fX, float fY, float fWidth, float fHeight);
		/// Static�� �߰��մϴ�.
		UIStatic*		AddStatic( DWORD nID, float fX, float fY, float fWidth, float fHeight);
		/// ��ũ�ѹٸ� �߰��մϴ�.
		UIScrollBar*	AddScrollBar( DWORD nID, float fX, float fY, float fWidth, float fHeight);
		/// ����Ʈ�ڽ��� �߰��մϴ�.
		UIListBox*		AddListBox( DWORD nID, float fX, float fY, float fWidth, float fHeight);
		/// ����Ʈ�ڽ��� �߰��մϴ�.
		UIEditBox*		AddEditBox( DWORD nID, float fX, float fY, float fWidth, float fHeight, DWORD Style = 0);
		/// �޺��ڽ��� �߰��մϴ�.
		UIComboBox*		AddComboBox( DWORD nID, float fX, float fY, float fWidth, float fHeight);

		/// ��� ��Ʈ���� �ʱ�ȭ�մϴ�.
		VOID			ClearControl();

	private:
		/// Modal ����
		BOOL					m_bModalMode;

		/// ���콺 ������ġ Drag�Ҷ� ���
		CPoint					m_PrevPoint;
		/// LButton Click ����
		BOOL					m_bLButtonClick;

		/// ��Ʈ�� ���庯��
		std::vector<UIControl*>	m_vecControl;

		/// ���̾�α� �����̸�
		Nave::String			m_strName;

		/// ���̾�α� ��������Ʈ ����ü
		SPRITEINFO				m_Skin;
		/// ���̾�α� ��������Ʈ ��ü
		UISprite				m_Sprite;
	};

}