#include "Nave2D.h"
#include "UIComboBox.h"
#include <NaveFramework/NFVideoDX9.h>

#include "NFTextureManager.h"

#include <Nave/NFTokenizer.h>

using namespace NaveFramework;

namespace Nave2D { 

	UIComboBox::UIComboBox(void)
	{
		m_bListShow = FALSE;

		m_fDropBtnSize = 18;
		m_fDropListSize = 20;
		m_fMaxDropListSize = 100;

		m_iState = TYPE_NONE;

		m_DropButtonRect.SetRect(0,0,0,0);
	}

	UIComboBox::~UIComboBox(void)
	{
		OnDestroy();
	}

	// 생성과 소멸시 발생 생성자, 소멸자와는 다르다.
	BOOL UIComboBox::OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID)
	{
		UIEditBox::OnCreate(pParent, fX, fY, fWidth-m_fDropBtnSize, fHeight, nID);

		m_DropList.OnCreate(this, m_fLocalX, m_fLocalY+m_fHeight+1, m_fWidth+m_fDropBtnSize, m_fDropListSize, COMBO_DROPLIST);
		m_DropList.SetEnable(FALSE);

		m_DropButton.OnCreate(m_fDropBtnSize, m_fDropBtnSize);		// 스크롤의 Up 버튼

		UpdateControlRect();
		return TRUE;
	}

	VOID UIComboBox::HideDropList()
	{
		m_bListShow = FALSE;
		m_DropList.SetEnable(FALSE);
	}

	BOOL UIComboBox::OnDestroy()
	{
		m_DropList.OnDestroy();

		m_DropButton.OnDestroy();

		return TRUE;
	}

	VOID UIComboBox::UpdateControlState(int State)
	{
		float fW = m_DropButtonSkin.fWidth;
		float fH = m_DropButtonSkin.fHeight/3;

		float SX = m_DropButtonSkin.fX;
		float SY = m_DropButtonSkin.fY+(fH*State);

		m_DropButton.SetImage(m_DropButtonSkin.pTexture, SX, SY, fW, fH);
		m_DropButton.SetSize(m_fDropBtnSize, m_fDropBtnSize);		// 스크롤의 Up 버튼
	}

	VOID UIComboBox::SetSize(float fWidth, float fHeight)
	{
		m_fWidth = fWidth-m_fDropBtnSize;
		m_fHeight = fHeight;

		m_Sprite.SetSize(fWidth, m_fHeight);

		UpdateControlRect();
	}

	VOID UIComboBox::OnMoveObject(float fX, float fY, float fWidth, float fHeight)
	{
		m_fX = m_fLocalX+fX;
		m_fY = m_fLocalY+fY;

		if(fWidth != 0)
			m_fWidth = fWidth;

		if(fHeight != 0)
			m_fHeight = fHeight;

		SetSize(m_fWidth+m_fDropBtnSize, m_fHeight);

		m_DropList.SetLocal(m_fLocalX, m_fLocalY+m_fHeight+1);
		m_DropList.OnMoveObject(fX, fY, m_fWidth+m_fDropBtnSize, m_fDropListSize);

		UpdateControlRect();
	}

	VOID UIComboBox::UpdateControlRect()
	{
		UIEditBox::UpdateControlRect();

		// 컨트롤이미지의 크기설정
		m_DropButton.SetSize(m_fDropBtnSize, m_fDropBtnSize);		// 스크롤의 Up 버튼
		m_DropList.SetSize(m_fWidth+m_fDropBtnSize, m_fDropListSize);

		// DropButton 위치설정
		int iY = 0;
		if(m_fHeight > m_fDropBtnSize)
			iY = (int)(m_fHeight-m_fDropBtnSize)/2;

		m_DropButtonRect.SetRect((int)(m_fX+m_fWidth-1), (int)(m_fY+iY), (int)(m_fX+m_fWidth+m_fDropBtnSize-1), (int)(m_fY+iY+m_fDropBtnSize));
	}

	VOID UIComboBox::SetSkin(LPSPRITEINFO Skin)
	{
		if(Skin)
			m_Skin = *Skin;

		m_Sprite.SetImageInfo(&m_Skin);

		SetSize(m_Skin.fWidth, m_Skin.fHeight);
		UpdateControlRect();
	}

	VOID UIComboBox::SetDropButtonSkin(LPSPRITEINFO ButtonSkin)
	{
		if(ButtonSkin)
			m_DropButtonSkin = *ButtonSkin;

		float fW = m_DropButtonSkin.fWidth;
		float fH = m_DropButtonSkin.fHeight/3;

		m_DropButton.SetImage(m_DropButtonSkin.pTexture, m_DropButtonSkin.fX, m_DropButtonSkin.fY, fW, fH);

		UpdateControlRect();
	}

	VOID UIComboBox::SetDropListSkin(LPSPRITEINFO Skin)
	{
		m_DropList.SetSkin(Skin);

		m_DropList.SetSize(m_fWidth+m_fDropBtnSize, m_fDropListSize);

		UpdateControlRect();
	}

	VOID UIComboBox::SetCursorSkin(LPSPRITEINFO CursorSkin)
	{
		m_DropList.SetCursorSkin(CursorSkin);

		UpdateControlRect();
	}

	VOID UIComboBox::SetScrollSkin(LPSPRITEINFO ScrollSkin)
	{
		m_DropList.SetScrollSkin(ScrollSkin);

		UpdateControlRect();
	}

	VOID UIComboBox::SetScrollThumbSkin(LPSPRITEINFO ThumbSkin)
	{
		m_DropList.SetScrollThumbSkin(ThumbSkin);

		UpdateControlRect();
	}

	VOID UIComboBox::SetScrollButtonSkin(LPSPRITEINFO ButtonSkin)
	{
		m_DropList.SetScrollButtonSkin(ButtonSkin);

		UpdateControlRect();
	}

	VOID UIComboBox::ResetContent()
	{
		m_DropList.ResetContent();
	}

	int UIComboBox::GetItemCount()
	{
		return m_DropList.GetCount();
	}

	VOID UIComboBox::SetCurSel(int iIndex)
	{
		if(iIndex < 0)
			return;

		m_DropList.SetEnable(FALSE);
		m_DropList.SetCurSel(iIndex);

		if(GetItemCount() == 0)
		{
			SetText(L" ");
			m_DropList.SetCurSel(0);
		}
		else
		{
			// 콤보박스에 있는 데이타를 읽어들여 EditBox에 셋팅한다.
			WCHAR Buff[256];
			m_DropList.GetText(iIndex, Buff);
			SetText(Buff);
		}
	}

	VOID UIComboBox::AddItem(const WCHAR* pString, LPARAM pData)
	{
		int Count = m_DropList.GetCount();
		int ItemH = m_DropList.GetItemHeight();

		if(((Count+1)*ItemH)+4 < m_fMaxDropListSize)
			m_fDropListSize = (float)((Count+1)*ItemH)+4;
		else
			m_fDropListSize = m_fMaxDropListSize;

		UpdateControlRect();

		m_DropList.AddItem(pString, pData);
		m_DropList.SetTopIndex(0);
	}

	VOID UIComboBox::RemoveItem(int iIndex)
	{
		m_DropList.RemoveItem(iIndex);

		// 사이즈 재설정.
		int Count = m_DropList.GetCount();
		int ItemH = m_DropList.GetItemHeight();

		if(((Count+1)*ItemH)+4 < m_fMaxDropListSize)
			m_fDropListSize = (float)((Count+1)*ItemH)+4;
		else
			m_fDropListSize = m_fMaxDropListSize;

		UpdateControlRect();
	}


/*
	VOID UIComboBox::SetUpperDropList(BOOL Flag) 
	{ 
		m_bUpperDropList = Flag; 
		if(m_bUpperDropList == FALSE)
			m_DropList.OnMoveObject(m_fX, m_fY+m_fDropBtnSize, m_fWidth, m_fHeight);
		else
			m_DropList.OnMoveObject(m_fX, m_fY-m_fHeight, m_fWidth, m_fHeight);
	}
*/
	VOID UIComboBox::OnRender()
	{
		if(!IsVisible()) return;

		UIEditBox::OnRender();

		NFVideoDX9::GetInstance().OnDraw(&m_DropButton, (int)m_DropButtonRect.left, (int)m_DropButtonRect.top);
	}

	VOID UIComboBox::OnRenderTop()
	{
		if(m_bListShow)
		{
			m_DropList.OnRender();	
		}
	}

	LRESULT UIComboBox::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		POINT Point = Nave::GetMousePos();
		int X = Point.x;
		int Y = Point.y;

		switch(uMsg)
		{
		case WM_LBUTTONDBLCLK:
			OnLButtonDown(X, Y);
			return InRect(X, Y);

		case WM_OBJECTEVENT:
			if(LOWORD(wParam) == COMBO_DROPLIST)
			{
				if(HIWORD(wParam) == TYPE_LUP)
				{
					if(lParam == -1) return TRUE;

					WCHAR Buff[256];
					m_DropList.GetText(lParam, Buff);
					SetString(Buff);

					HideDropList();

					PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, COMBO_CHANG), lParam);
				}
			}
			return TRUE;
		}

		return UIEditBox::MsgProc(uMsg, wParam, lParam );
	}

	// WM_MOUSEMOVE
	BOOL UIComboBox::OnMouseMove(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		CPoint pt(iMouseX, iMouseY);

		if(m_DropButtonRect.PtInRect(pt))
		{
			m_iState = TYPE_OVER;
			UpdateControlState(m_iState);
			return TRUE;
		}
		else if(m_iState != TYPE_NONE)
		{
			m_iState = TYPE_NONE;
			UpdateControlState(m_iState);
		}

		if(m_DropList.OnMouseMove(iMouseX, iMouseY))
			return TRUE;

		return FALSE;
	}

	// WM_LBUTTONDOWN
	BOOL UIComboBox::OnLButtonDown(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		if(InRect(iMouseX, iMouseY))
		{
			SetFocus();
			return TRUE;
		}

		if(GetFocus() == GetIME())
		{
			KillFocus();
		}

		CPoint pt(iMouseX, iMouseY);

		if(m_DropButtonRect.PtInRect(pt))
		{
			m_iState = TYPE_LDOWN;
			UpdateControlState(m_iState);

			// 여기서 리스트를 Show 한다.
			if(m_bListShow == FALSE)
			{
				m_bListShow = TRUE;
				m_DropList.SetEnable(TRUE);
			}
			else
				HideDropList();

			return TRUE;
		}
		// 만약 드랍리스트에서 이벤트가 발생하지 않았으면 드랍리스트를 Hide시킨다
		if(m_DropList.OnLButtonDown(iMouseX, iMouseY))
			return TRUE;

		HideDropList();

		return FALSE;
	}

	// WM_LBUTTONUP
	BOOL UIComboBox::OnLButtonUp(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		CPoint pt(iMouseX, iMouseY);

		m_iState = TYPE_NONE;
		if(m_DropButtonRect.PtInRect(pt))
			m_iState = TYPE_OVER;

		UpdateControlState(m_iState);

		if(m_DropList.OnLButtonUp(iMouseX, iMouseY))
			return TRUE;

		return FALSE;
	}

	// WM_LBUTTONUP
	BOOL UIComboBox::OnMouseWheel(int iMouseX, int iMouseY, int zDelta)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		if(m_DropList.OnMouseWheel(iMouseX, iMouseY, zDelta))
			return TRUE;

		return FALSE;
	}

}