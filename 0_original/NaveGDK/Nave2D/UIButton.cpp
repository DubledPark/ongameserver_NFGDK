#include "Nave2D.h"
#include "UIButton.h"

namespace Nave2D { 

	UIButton::UIButton(void)
	{
		m_iState = TYPE_NONE;
		m_bButtonClick = FALSE;
	}

	UIButton::~UIButton(void)
	{
		m_iState = TYPE_NONE;
		OnDestroy();
	}

	BOOL UIButton::OnDestroy()
	{
		return TRUE;
	}

	LRESULT UIButton::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		POINT Point = Nave::GetMousePos();
		int X = Point.x;
		int Y = Point.y;

		// 더블클릭을 무시한다.
		switch(uMsg)
		{
		case WM_LBUTTONDBLCLK:
			return OnLButtonDown(X, Y);
		}

		return UIControl::MsgProc(uMsg, wParam, lParam);
	}

	BOOL UIButton::OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID)
	{
		m_dID = nID;

		m_fLocalX = m_fX = fX;
		m_fLocalY = m_fY = fY;
		m_fWidth = fWidth;
		m_fHeight = fHeight;

		m_Sprite.OnCreate(fWidth, fHeight);
		OnSetParent(pParent);
		SetVisible(TRUE);
		return TRUE;
	}

	VOID UIButton::OnMoveObject(float fX, float fY, float fWidth, float fHeight)
	{
		UIControl::OnMoveObject(fX, fY, fWidth, fHeight);
	}

	VOID UIButton::SetSkin(LPSPRITEINFO Skin)
	{
		if(Skin)
			m_Skin = *Skin;

		m_Sprite.SetImageInfo(&m_Skin);

		SetSize(m_Skin.fWidth, m_Skin.fHeight/4);
		UpdateControlRect();
	}

	VOID UIButton::UpdateControlRect()
	{
		float height = m_Skin.fHeight/4;
		float SY = 0;

		if(IsEnable())
			SY = m_Skin.fY+(height*m_iState)+height;
		else
			SY = m_Skin.fY;

		m_Sprite.SetImage(m_Skin.pTexture, m_Skin.fX, SY, m_Skin.fWidth, height);
		m_Sprite.SetSize(m_Skin.fWidth, height);
	}

	VOID UIButton::SetEnable(BOOL bEnable)
	{
		UIControl::SetEnable(bEnable);

		m_iState = TYPE_NONE;
		UpdateControlRect();
	}

	VOID UIButton::SetShowArea(float fX, float fY, float fWidth, float fHeight)
	{
		m_Skin.fX = fX;
		m_Skin.fY = fY;
		m_Skin.fWidth = fWidth;
		m_Skin.fHeight = fHeight;

		SetSkin(NULL);
	}

	// WM_MOUSEMOVE
	BOOL UIButton::OnMouseMove(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		INT Temp = m_iState;
		m_iState = TYPE_NONE;

		if(InRect(iMouseX, iMouseY))
		{
			if(m_bButtonClick == TRUE)
				m_iState = TYPE_LDOWN;
			else
				m_iState = TYPE_OVER;
		}

		if(Temp != m_iState)
		{
			UpdateControlRect();
			if(m_iState == TYPE_OVER)
				PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_OVER), 0);
		}

		return FALSE; 
	}

	// WM_LBUTTONDOWN
	BOOL UIButton::OnLButtonDown(int iMouseX, int iMouseY) 
	{ 
		if(!IsEnable() || !IsVisible())
			return FALSE;

		INT Temp = m_iState;
		m_iState = TYPE_NONE;

		if(InRect(iMouseX, iMouseY))
		{
			m_bButtonClick = TRUE;
			m_iState = TYPE_LDOWN;

			if(Temp != m_iState)
				UpdateControlRect();

			PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_LDOWN), 0);
			return TRUE;
		}
		return FALSE; 
	}

	// WM_LBUTTONUP
	BOOL UIButton::OnLButtonUp(int iMouseX, int iMouseY) 
	{ 
		if(!IsEnable() || !IsVisible())
			return FALSE;

		if(m_bButtonClick==FALSE)
			return FALSE;

		m_bButtonClick = FALSE;

		INT Temp = m_iState;
		m_iState = TYPE_NONE;

		if(InRect(iMouseX, iMouseY))
		{
			m_iState = TYPE_OVER;
			if(Temp != m_iState)
				UpdateControlRect();

			PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_LUP), 0);
			return TRUE;
		}
		return FALSE; 
	}

}
