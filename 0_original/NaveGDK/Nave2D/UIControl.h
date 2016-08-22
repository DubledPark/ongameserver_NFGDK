/** 
 *  @file		UIControl.h
 *  @brief		UI��Ʈ�� ��ü�� Super Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFImage.h"
#include "UISprite.h"
#include "UIObject.h"

namespace Nave2D {

	/** 
	 *  @class        UIControl
	 *  @brief        UI��Ʈ���� Super Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UIControl : public UIObject
	{
	public:
		// UIControl ������
		UIControl();
		/// UIControl �Ҹ���
		virtual ~UIControl();

	public:
		/**
		 * @brief	��Ʈ���� �����մϴ�.
		 * @param pParent	�θ�����Ʈ
		 * @param fX		����X
		 * @param fY		����Y
		 * @param fWidth	����ũ��
		 * @param fHeight	����ũ��
		 * @param nID		��ü ���̵�
		 * @warning	fX,fY�� ���� �θ��� ������������ ������ǥ�̴�.
		 * @return	��������
		 */
		virtual BOOL	OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID);
		/// ��Ʈ���� �����մϴ�.
		virtual BOOL	OnDestroy()					{ return TRUE; }

		/// ��Ʈ�ѿ� Data�� �����մϴ�.
		VOID			SetData(DWORD dwData) { m_dwData = dwData; }
		/// ��Ʈ���� Data�� ����ϴ�.
		DWORD			GetData() { return m_dwData; }

		/// WM_COMMAND
		virtual VOID OnCommand(int iID, int iEvent, LPARAM lParam) { return; }
		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY) { return FALSE; }	// �̰��� ���� �ٹ޾ƾ��Ѵ�.
		/// WM_LBUTTONDBLCLK
		virtual BOOL OnLButtonDClick(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_RBUTTONDOWN
		virtual BOOL OnRButtonDown(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_RBUTTONUP
		virtual BOOL OnRButtonUp(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_MOUSEWHEEL
		virtual BOOL OnMouseWheel(int iMouseX, int iMouseY, int zDelta) { return InRect(iMouseX, iMouseY); }

		/// Message Proc
		virtual LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// ������Ʈ ���� ��Ʈ��
	    virtual const char* ToString()							{ return "UIControl"; }

		/// ������Ʈ�� Processó���Ѵ�.
		virtual VOID	OnUpdate(float dt = 0.0f)				{ return; }
		/// ������Ʈ�� ȭ�鿡 �׸���.
		virtual VOID	OnRender();
		/// �� ��Ʈ���� Render ���� �������� �̴ϴ�.
		virtual VOID	OnRenderTop()							{ return; }

		/// ��Ʈ���� ���¸� ���ɴϴ�.
		inline BOOL		IsEnable() { return m_bEnable; }

		/// ��Ʈ���� ���� Ȱ��, ��Ȱ�� ���°� FALSE�ǰ�� �������� ������ �̺�Ʈ�� �߻����Ѵ�.
		virtual VOID	SetEnable(BOOL bEnable) { m_bEnable = bEnable; }

		/// ��Ʈ���� ���� ��ǥ�� �����մϴ�.
		VOID			SetLocal(float fLocalX, float fLocalY);
		/// ��Ʈ���� ����� �����մϴ�.
		virtual VOID	SetSize(float fWidth, float fHeight);

		/// Move Objec ó��
		virtual VOID	OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/// ������ ���ɴϴ�.
		inline  int		GetOpacity()					{ return (int)m_Sprite.GetOpacity(); }
		/// ������ �����Ѵ�.
		inline  VOID	SetOpacity( int iOpacity )		{ m_Sprite.SetOpacity((BYTE)iOpacity); }
		/// ���� ��带 �����մϴ�.
		virtual VOID	SetBlendMode(UISprite::BLEND BlendMode);

		/// �⺻ ��Ų�� �����մϴ�.
		virtual VOID	SetSkin(LPSPRITEINFO Skin);
		/// �ؽ��ĸ� �����մϴ�.
		virtual VOID	SetTexture(const Nave::String& textureName);
		/// ȭ�鷻���� ������ �����մϴ�.
		virtual VOID	SetShowArea(float fX, float fY, float fWidth, float fHeight);

		/// ��Ʈ�� Rect�� ������Ʈ �մϴ�.
		virtual VOID	UpdateControlRect();

	protected:
		/// ��Ʈ���� ������ġ X
		float			m_fLocalX;
		/// ��Ʈ���� ������ġ X
		float			m_fLocalY;

		/// ��Ʈ���� Ȱ��ȭ ����
		BOOL			m_bEnable;
		/// ��Ʈ���� ���� ����Ÿ
		DWORD			m_dwData;

		/// ��Ʈ���� ��Ų ����
		SPRITEINFO		m_Skin;
		/// ��Ʈ���� ��������Ʈ
		UISprite		m_Sprite;
	};

}