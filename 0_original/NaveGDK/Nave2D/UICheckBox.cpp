#include "Nave2D.h"
#include "UICheckBox.h"
#include <NaveFramework/NFVideoDX9.h>

namespace Nave2D { 

	UICheckBox::UICheckBox(void)
	{
		m_iState = TYPE_NONE;
		m_bPushFlag = FALSE;
		m_bButtonClick = FALSE;
	}

	UICheckBox::~UICheckBox(void)
	{
		m_bPushFlag = FALSE;
		m_iState = TYPE_NONE;
		OnDestroy();
	}

	VOID UICheckBox::UpdateControlRect()
	{
		float height = m_Skin.fHeight/3;
		float SY = m_Skin.fY;

		if(IsEnable())
			SY += m_bPushFlag ? height*2 : height; 
		else
			SY = m_Skin.fY;

		m_Sprite.SetImage(m_Skin.pTexture, m_Skin.fX, SY, m_Skin.fWidth, height);
		m_Sprite.SetSize(m_Skin.fWidth, height);
	}

	// WM_MOUSEMOVE
	BOOL UICheckBox::OnMouseMove(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		return FALSE; 
	}

	// WM_LBUTTONDOWN
	BOOL UICheckBox::OnLButtonDown(int iMouseX, int iMouseY) 
	{ 
		if(!IsEnable() || !IsVisible())
			return FALSE;

		INT Temp = m_iState;
		m_iState = TYPE_NONE;

		if(InRect(iMouseX, iMouseY))
		{
			m_bButtonClick = TRUE;
			m_iState = TYPE_LDOWN;
			m_bPushFlag = !m_bPushFlag;
			UpdateControlRect();

			PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_LDOWN), 0);
			return TRUE;
		}
		return FALSE; 
	}

	// WM_LBUTTONUP
	BOOL UICheckBox::OnLButtonUp(int iMouseX, int iMouseY) 
	{ 
		if(!IsEnable() || !IsVisible())
			return FALSE;

		if(m_bButtonClick==FALSE)
			return FALSE;

		m_bButtonClick = FALSE;

		m_iState = TYPE_NONE;

		if(InRect(iMouseX, iMouseY))
		{
			PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_LUP), 0);
			return TRUE;
		}
		return FALSE; 
	}

}
