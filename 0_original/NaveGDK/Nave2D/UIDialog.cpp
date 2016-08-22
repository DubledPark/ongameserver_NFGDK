#include "Nave2D.h"
#include "UIDialog.h"
#include "UIControl.h"
#include "NFTextureManager.h"
#include <NaveFramework/NFVideoDX9.h>

using namespace NaveFramework;

namespace Nave2D {

	UIDialog::UIDialog(void)
	{
		m_bModalMode = FALSE;
		m_bLButtonClick = FALSE;
		m_PrevPoint.x = m_PrevPoint.y = 0;
	}

	UIDialog::UIDialog(const Nave::String& name) : m_strName(name)
	{
		m_bModalMode = FALSE;
		m_bLButtonClick = FALSE;
		m_PrevPoint.x = m_PrevPoint.y = 0;
	}

	UIDialog::~UIDialog(void)
	{
		ClearControl();
		SetVisible(FALSE);
	}

	VOID UIDialog::ClearControl()
	{
		int iSize = m_vecControl.size();

		for(int i = 0; i < iSize; ++i)
		{
			_DELETE(m_vecControl[i]);
		}

		m_vecControl.clear();
	}

	VOID UIDialog::SetName(Nave::String& name)
	{
		m_strName = name;
	}

	const Nave::String& UIDialog::GetName() const 
	{
		return m_strName;
	}

	BOOL UIDialog::OnSetFocus(int iMouseX, int iMouseY)
	{
		iMouseX -= (int)m_fX;
		iMouseY -= (int)m_fY;

		if(0 < iMouseX && iMouseX < m_fWidth )
		{
			if(0 < iMouseY && iMouseY < m_fHeight )
			{
				DialogAlwaysOnTop(this);
				return TRUE;
			}
		}
		return FALSE;
	}

	VOID UIDialog::OnShow()
	{
		if(m_bModalMode == TRUE)
		{
			DialogAlwaysOnTop(this);
		}
		SetVisible(TRUE);
	}

	VOID UIDialog::OnHide()
	{
		SetVisible(FALSE);
	}

	VOID UIDialog::SetModalFlag(BOOL Mode) 
	{ 
		m_bModalMode = Mode; 
		if(m_bModalMode == TRUE)
		{
			DialogAlwaysOnTop(this);
		}
	};

	LRESULT UIDialog::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		POINT Point = Nave::GetMousePos();
		int X = Point.x;
		int Y = Point.y;

		// 컨트롤 에게도 메시지를 날린다.
		int iControl = m_vecControl.size();
		for(int i = iControl-1; i >= 0; --i)
		{
			if(!m_vecControl[i]->IsVisible())
				continue;

			if(m_vecControl[i]->MsgProc(uMsg, wParam, lParam))
			{
				if(uMsg == WM_LBUTTONDOWN)
					DialogAlwaysOnTop(this);

				return TRUE;
			}
		}

		switch(uMsg)
		{
		case WM_SCALE:
			OnScale();
			break;

		case WM_COMMAND:
			OnCommand(LOWORD(wParam), HIWORD(wParam), lParam);
			break;

		case WM_LBUTTONUP:
			m_bLButtonClick = FALSE;
			OnLButtonUp(X, Y);
			break;

		case WM_LBUTTONDOWN:
			if(OnSetFocus(X, Y))
			{
				if(OnLButtonDown(X, Y))
				{
					m_PrevPoint = Point;
					m_bLButtonClick = TRUE;
				}
				return TRUE;
			}
			break;

		case WM_LBUTTONDBLCLK:
			if(OnSetFocus(X, Y))
			{
				OnLButtonDClick(X, Y);
			}
			break;

		case WM_MOUSEMOVE:
			if(m_bLButtonClick)
			{
				int x = X-m_PrevPoint.x;
				int y = Y-m_PrevPoint.y;
				m_fX += x;
				m_fY += y;
				OnMove(m_fX, m_fY);
				m_PrevPoint = Point;
			}
			OnMouseMove(X, Y);
			break;

		case WM_RBUTTONUP:
			OnRButtonUp(X, Y);
			break;

		case WM_RBUTTONDOWN:
			OnSetFocus(X, Y);
			OnRButtonDown(X, Y);
			break;

		case WM_MOUSEWHEEL:
			OnMouseWheel(X, Y, GET_WHEEL_DELTA(wParam));
			break;
		}

		if(m_bModalMode)
			return TRUE;

		return InRect(X, Y);
	}

	VOID UIDialog::SetBlendMode(UISprite::BLEND BlendMode)
	{
		m_Sprite.SetBlendMode(BlendMode);
	}

	VOID UIDialog::OnUpdate(float dt)
	{
		int iSize = (int)m_vecControl.size();

		for(int i = 0; i < iSize; ++i)
		{
			if(!m_vecControl[i]->IsVisible())
				continue;

			m_vecControl[i]->OnUpdate(dt);
		}
	}

	VOID UIDialog::OnRender()
	{
		if(!IsVisible()) return;
		NFVideoDX9::GetInstance().OnDraw(&m_Sprite, (int)m_fX, (int)m_fY);

		int iSize = (int)m_vecControl.size();

		for(int i = 0; i < iSize; ++i)
		{
			if(!m_vecControl[i]->IsVisible())
				continue;

			m_vecControl[i]->OnRender();
		}

		// 일반 컨트롤러위에 뜰 오브젝트 처리 (ComboBox의 ComboList등)
		for(int i = 0; i < iSize; ++i)
		{
			if(!m_vecControl[i]->IsVisible())
				continue;

			m_vecControl[i]->OnRenderTop();
		}
	}

	VOID UIDialog::OnMove(float fX, float fY)
	{
		int iSize = (int)m_vecControl.size();

		for(int i = 0; i < iSize; ++i)
		{
			m_vecControl[i]->OnMoveObject(fX, fY);
		}
	}

    UIControl* UIDialog::GetControl( int iIndex )
	{
		if(iIndex < 0)
			return NULL;
		if((int)m_vecControl.size() >= iIndex)
			return NULL;

		return m_vecControl[iIndex];
	}

    UIControl* UIDialog::GetControlAtPoint( CPoint pt )
	{
		int iSize = (int)m_vecControl.size();

		for(int i = iSize; i >= 0; --i)
		{
			if(m_vecControl[i]->InRect(pt.x, pt.y))
				return m_vecControl[i];
		}

		return NULL;
	}

	BOOL UIDialog::GetControlVisible( int iIndex )
	{
		UIControl* pCtrl = GetControl(iIndex);
		if(pCtrl == NULL)
			return FALSE;

		return pCtrl->IsVisible();
	}

    VOID UIDialog::SetControlVisible( int iIndex, BOOL bVisible )
	{
		UIControl* pCtrl = GetControl(iIndex);
		if(pCtrl == NULL)
			return;

		pCtrl->SetVisible(bVisible);
	}

	UIButton* UIDialog::AddButton( DWORD nID, float fX, float fY, float fWidth, float fHeight)
	{
		UIButton* pButton = new UIButton();
		pButton->OnCreate(this, fX, fY, fWidth, fHeight, nID);
		pButton->OnMoveObject(m_fX, m_fY);

		m_vecControl.push_back(pButton);

		return pButton;
	}

	UICheckBox* UIDialog::AddCheckBox( DWORD nID, float fX, float fY, float fWidth, float fHeight)
	{
		UICheckBox* pCheckBox = new UICheckBox();
		pCheckBox->OnCreate(this, fX, fY, fWidth, fHeight, nID);
		pCheckBox->OnMoveObject(m_fX, m_fY);

		m_vecControl.push_back(pCheckBox);

		return pCheckBox;
	}

	UIStatic* UIDialog::AddStatic( DWORD nID, float fX, float fY, float fWidth, float fHeight)
	{
		UIStatic* pStatic = new UIStatic();
		pStatic->OnCreate(this, fX, fY, fWidth, fHeight, nID);
		pStatic->OnMoveObject(m_fX, m_fY);

		m_vecControl.push_back(pStatic);

		return pStatic;
	}

	UIScrollBar* UIDialog::AddScrollBar( DWORD nID, float fX, float fY, float fWidth, float fHeight)
	{
		UIScrollBar* pScroll = new UIScrollBar();
		pScroll->OnCreate(this, fX, fY, fWidth, fHeight, nID);
		pScroll->OnMoveObject(m_fX, m_fY);

		m_vecControl.push_back(pScroll);

		return pScroll;
	}

	UIListBox* UIDialog::AddListBox( DWORD nID, float fX, float fY, float fWidth, float fHeight)
	{
		UIListBox* pList = new UIListBox();
		pList->OnCreate(this, fX, fY, fWidth, fHeight, nID);
		pList->OnMoveObject(m_fX, m_fY);

		m_vecControl.push_back(pList);

		return pList;
	}

	UIEditBox* UIDialog::AddEditBox( DWORD nID, float fX, float fY, float fWidth, float fHeight, DWORD Style)
	{
		UIEditBox* pEdit = new UIEditBox();
		pEdit->OnCreate(this, fX, fY, fWidth, fHeight, nID, Style);
		pEdit->OnMoveObject(m_fX, m_fY);

		m_vecControl.push_back(pEdit);

		return pEdit;
	}

	UIComboBox* UIDialog::AddComboBox( DWORD nID, float fX, float fY, float fWidth, float fHeight)
	{
		UIComboBox* pCombo = new UIComboBox();
		pCombo->OnCreate(this, fX, fY, fWidth, fHeight, nID);
		pCombo->OnMoveObject(m_fX, m_fY);

		m_vecControl.push_back(pCombo);

		return pCombo;
	}


	VOID UIDialog::SetSkin(LPSPRITEINFO Skin)
	{
		if(Skin)
			m_Skin = *Skin;

		m_fWidth = m_Skin.fWidth;
		m_fHeight = m_Skin.fHeight;

		m_Sprite.SetImage(m_Skin.pTexture, m_Skin.fX, m_Skin.fY, m_Skin.fWidth, m_Skin.fHeight);
		m_Sprite.SetSize(m_fWidth, m_fHeight);
	}

	VOID UIDialog::SetTexture(const Nave::String& textureName)
	{
		LPD3DDEVICE pd3dDevice = NFVideoDX9::GetInstance().Get3DDevice();
		LPD3DTEXTURE	pTexture;
		DWORD dwTexture = NFTextureManager::GetInstance().CreateTexture(pd3dDevice, textureName.c_str(), &pTexture);

		LPTEXTURENODE pNode = NFTextureManager::GetInstance().FindTextureNode(dwTexture);

		m_Skin.dwTexture = dwTexture;
		m_Skin.pTexture = pTexture;
		m_Skin.fX = 0;
		m_Skin.fY = 0;
		m_Skin.fWidth = (float)pNode->GetWidth();
		m_Skin.fHeight = (float)pNode->GetHeight();

		SetShowArea(0, 0, (float)pNode->GetWidth(), (float)pNode->GetHeight());
	}

	VOID UIDialog::SetShowArea(float fX, float fY, float fWidth, float fHeight)
	{
		m_Skin.fX = fX;
		m_Skin.fY = fY;
		m_Skin.fWidth = fWidth;
		m_Skin.fHeight = fHeight;

		SetSkin(NULL);
	}
}