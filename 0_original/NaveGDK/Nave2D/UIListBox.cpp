#include "Nave2D.h"
#include "UIListBox.h"
#include <NaveFramework/NFVideoDX9.h>

using namespace NaveFramework;

namespace Nave2D { 

	UIListBox::UIListBox(void)
	{
		m_iItemHeight = 16;
		m_iViewItemCount = 0;
		m_iTopIndex = 0;
		m_iSelectedItem = -1;
		m_iCursorItem = -1;
		m_bShowCursor = FALSE;

		m_fScrollWidth = 18;

		SetTextColor(RGB(0,0,0));
	}

	UIListBox::~UIListBox(void)
	{
		OnDestroy();
	}

	BOOL UIListBox::OnDestroy()
	{
		ResetContent();

		return TRUE;
	}

	BOOL UIListBox::OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID, int ItemHeight)
	{
		m_dID = nID;

		m_fLocalX = m_fX = fX;
		m_fLocalY = m_fY = fY;
		m_fWidth = fWidth;
		m_fHeight = fHeight;
		SetItemHeight(ItemHeight);

		OnSetParent(pParent);
		SetVisible(TRUE);

		m_Sprite.OnCreate(fWidth, fHeight);

		m_ScrollBar.OnCreate(this, m_fLocalX+m_fWidth-m_fScrollWidth, m_fLocalY, m_fScrollWidth, m_fHeight, m_dID);

		m_CursorCur.OnCreate(fWidth, (float)ItemHeight);		// 스크롤의 Cursor
		m_CursorSel.OnCreate(fWidth, (float)ItemHeight);		// 스크롤의 Select Cursor

		m_Items.reserve(MAX_LISTITEM);

		// 스크롤및 관련 파일을 이동하기 위해서
		OnMoveObject(0, 0);
		return TRUE;
	}

	VOID UIListBox::OnMoveObject(float fX, float fY, float fWidth, float fHeight)
	{
		m_fX = m_fLocalX+fX;
		m_fY = m_fLocalY+fY;

		if(fWidth != 0)
			m_fWidth = fWidth;

		if(fHeight != 0)
			m_fHeight = fHeight;

		m_Sprite.SetSize(m_fWidth, m_fHeight);

		// 기타정보 갱신
		m_iViewItemCount = (int)m_fHeight/GetItemHeight();

		m_ScrollBar.SetLocal(m_fLocalX+m_fWidth-m_fScrollWidth, m_fLocalY);
		m_ScrollBar.OnMoveObject(fX, fY, m_fScrollWidth, m_fHeight);
		m_ScrollBar.SetPageSize(m_iViewItemCount);

		float fH = (float)GetItemHeight();
		if(m_ScrollBar.IsShowScroll())
		{
			m_CursorCur.SetSize(m_fWidth-m_fScrollWidth, fH);
			m_CursorSel.SetSize(m_fWidth-m_fScrollWidth, fH);
		}
		else
		{
			m_CursorCur.SetSize(m_fWidth, fH);
			m_CursorSel.SetSize(m_fWidth, fH);
		}

		UpdateControlRect();
	}

	VOID UIListBox::SetSize(float fWidth, float fHeight)
	{
		OnMoveObject(m_fX-m_fLocalX, m_fY-m_fLocalY, fWidth, fHeight);
	}

	VOID UIListBox::SetScrollSkin(LPSPRITEINFO ScrollSkin)
	{
		if(ScrollSkin)
		{
			m_fScrollWidth = ScrollSkin->fWidth;
			
			m_ScrollBar.SetLocal(m_fLocalX+m_fWidth-m_fScrollWidth, m_fHeight);
			OnMoveObject(m_fX-m_fLocalX, m_fY-m_fLocalY);
		}

		m_ScrollBar.SetSkin(ScrollSkin);

		UpdateControlRect();
	}

	VOID UIListBox::SetScrollThumbSkin(LPSPRITEINFO ThumbSkin)
	{
		m_ScrollBar.SetThumbSkin(ThumbSkin);

		UpdateControlRect();
	}

	VOID UIListBox::SetScrollButtonSkin(LPSPRITEINFO ButtonSkin)
	{
		m_ScrollBar.SetButtonSkin(ButtonSkin);

		UpdateControlRect();
	}

	VOID UIListBox::SetCursorSkin(LPSPRITEINFO CursorSkin)
	{
		if(CursorSkin)
		{
			m_bShowCursor = TRUE;
			m_CursorSkin = *CursorSkin;
		}

		float fH = m_CursorSkin.fHeight/2;

		m_CursorCur.SetImage(m_CursorSkin.pTexture, m_CursorSkin.fX, m_CursorSkin.fY, m_CursorSkin.fWidth, fH);
		m_CursorSel.SetImage(m_CursorSkin.pTexture, m_CursorSkin.fX, m_CursorSkin.fY+fH, m_CursorSkin.fWidth, fH);

		fH = (float)GetItemHeight();
		m_CursorCur.SetSize(m_fWidth-m_fScrollWidth, fH);
		m_CursorSel.SetSize(m_fWidth-m_fScrollWidth, fH);

		UpdateControlRect();
	}

	VOID UIListBox::OnRender()
	{
		if(!IsVisible()) return;

		// 기본 배경 그리기
		if(m_Skin.dwTexture != 0)
		{
			UIControl::OnRender();
		}

		// 아이템 그리기
		// Top 값을 가져와 순서대로 찍는다.
		int Top = GetTopIndex();
		int Count = GetCount();

		RECT rect;

		if(Count != 0) 
		{
			// 화면에 보이는 개수가 최대 개수보다 크다는 것은 밑부분이 비게 된다는 것이다.
			int ItemCount = m_iViewItemCount;
			if(m_iViewItemCount+Top >= Count)
				ItemCount = Count-Top;

			rect = GetItemRect(Top);
			for(int i = 0; i < ItemCount; ++i)
			{
				if(m_bShowCursor)
				{
					if(Top+i == m_iSelectedItem)
						NFVideoDX9::GetInstance().OnDraw(&m_CursorSel, (int)m_fX+rect.left, (int)m_fY+rect.top);
					else if(Top+i == m_iCursorItem)
						NFVideoDX9::GetInstance().OnDraw(&m_CursorCur, (int)m_fX+rect.left, (int)m_fY+rect.top);
				}

				// 로컬좌표 계산
				OnDrawItem(Top+i, rect);

				rect.top += m_iItemHeight;
				rect.bottom += m_iItemHeight;
			}
		}


		m_ScrollBar.OnRender();

		return;
	}

	VOID UIListBox::OnDrawItem(int ItemIndex, RECT& DrawRect)
	{
		// 아이템을 그린다.
		LPLISTITEM pItem = m_Items[ItemIndex];

		// 리스트 박스의 글자는 기본적으로 반투명이다.&
		pItem->TextOut.OnRender((int)(m_fX+DrawRect.left), (int)(m_fY+DrawRect.top));
	}

	LRESULT UIListBox::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		POINT Point = Nave::GetMousePos();
		int X = Point.x;
		int Y = Point.y;

		// 더블클릭을 무시한다.
		switch(uMsg)
		{
		case WM_LBUTTONDBLCLK:
			OnLButtonDown(X, Y);
			return InRect(X, Y);
		}

		return UIControl::MsgProc(uMsg, wParam, lParam);
	}

	BOOL UIListBox::OnMouseMove(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		m_iCursorItem = -1;

		if(m_ScrollBar.OnMouseMove(iMouseX, iMouseY))
		{
			m_iTopIndex = m_ScrollBar.GetThumbPos();
			return TRUE;
		}
		if(InRect(iMouseX, iMouseY))
		{
			CPoint pt(iMouseX-(int)m_fX, iMouseY-(int)m_fY);
			CRect rect;

			int Top = GetTopIndex();
			int Count = GetCount();
			int ItemCount = m_iViewItemCount;
			if(m_iViewItemCount+Top >= Count)
				ItemCount = Count-Top;

			rect = GetItemRect(Top);

			for(int i = Top; i < Top+ItemCount; ++i)
			{
				if(rect.PtInRect(pt))
				{
					m_iCursorItem = i;
					return TRUE;
				}
				rect.top += m_iItemHeight;
				rect.bottom += m_iItemHeight;
			}

		}
		return FALSE;
	}

	BOOL UIListBox::OnLButtonDown(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		if(m_ScrollBar.OnLButtonDown(iMouseX, iMouseY))
		{
			m_iTopIndex = m_ScrollBar.GetThumbPos();
			return TRUE;
		}
		if(InRect(iMouseX, iMouseY))
		{
			CPoint pt(iMouseX-(int)m_fX, iMouseY-(int)m_fY);
			CRect rect;

			int Top = GetTopIndex();
			int Count = GetCount();
			int ItemCount = m_iViewItemCount;
			if(m_iViewItemCount+Top >= Count)
				ItemCount = Count-Top;

			rect = GetItemRect(Top);

			for(int i = Top; i < Top+ItemCount; ++i)
			{
				if(rect.PtInRect(pt))
				{
					SetCurSel(i);

					if(m_dID == COMBO_DROPLIST)
						PostMessageEx(m_hParent, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_LDOWN), m_iSelectedItem);
					else
						PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_LDOWN), m_iSelectedItem);

					return TRUE;
				}
				rect.top += m_iItemHeight;
				rect.bottom += m_iItemHeight;
			}
		}
		return FALSE;
	}

	BOOL UIListBox::OnLButtonUp(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		if(m_ScrollBar.OnLButtonUp(iMouseX, iMouseY))
		{
			m_iTopIndex = m_ScrollBar.GetThumbPos();
			return TRUE;
		}
		if(InRect(iMouseX, iMouseY))
		{
			if(m_dID == COMBO_DROPLIST)
				PostMessageEx(m_hParent, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_LUP), m_iSelectedItem);
			else
				PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, TYPE_LUP), m_iSelectedItem);
			return TRUE;
		}
		return FALSE;
	}

	BOOL UIListBox::OnMouseWheel(int iMouseX, int iMouseY, int zDelta)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		if(InRect(iMouseX, iMouseY))
		{
			if( zDelta > 0 )
				m_ScrollBar.Scroll( -1 );
			else
				m_ScrollBar.Scroll( 1 );

			m_iTopIndex = m_ScrollBar.GetThumbPos();
			return TRUE;
		}

		return FALSE;
	}

	VOID UIListBox::SetTextColor(COLORREF TextColor)
	{
		m_TextColor = TextColor;
	}

	int	UIListBox::AddItem(const WCHAR* pString, LPARAM pData)
	{
		int Cnt = GetCount();
		if(Cnt >= MAX_LISTITEM)
			RemoveItem(0);

		LPLISTITEM pItem = new LISTITEM;
		pItem->Title = pString;
		pItem->Data = pData;
		pItem->TextOut.OnCreate();
		CRect rt(0, 0, (int)m_fWidth, m_iItemHeight);
		pItem->TextOut.DrawText(255, rt, m_TextColor, DT_LEFT|DT_VCENTER|DT_SINGLELINE, pString);

		m_Items.push_back(pItem);

		m_ScrollBar.SetRange(0, m_Items.size());

		UpdateControlRect();
		return m_Items.size();
	}

	int UIListBox::RemoveItem(int index)
	{
		if(index >= (int)m_Items.size() || index < 0)
			return -1;

		LPLISTITEM pItem = m_Items[index];
		_DELETE(pItem);

		m_Items.erase(m_Items.begin()+index);

		m_ScrollBar.SetRange(0, m_Items.size());

		UpdateControlRect();
		return m_Items.size();
	}

	VOID UIListBox::ResetContent()
	{
		for( int i = 0; i < (int)m_Items.size(); ++i )
		{
			LPLISTITEM pItem = m_Items[i];
			_DELETE(pItem);
		}

		m_iTopIndex = 0;
		m_iSelectedItem = -1;
		m_iCursorItem = -1;

		m_Items.clear();
		m_ScrollBar.SetRange(0, 1);
	}

	int	 UIListBox::GetCount()				// 총 항목의 개수를 구한다.
	{
		return m_Items.size();
	}

	int	 UIListBox::GetItemHeight()	// 항목의 높이를 조절한다.
	{
		return m_iItemHeight;
	}

	VOID UIListBox::SetItemHeight(int Height)	// 항목의 높이를 조절한다.
	{
		m_iItemHeight = Height;

		m_iViewItemCount = (int)m_fHeight/GetItemHeight();

		if(m_bShowCursor)
		{
			float fH = (float)m_iItemHeight;
			m_CursorCur.SetSize(m_fWidth-m_fScrollWidth, fH);
			m_CursorSel.SetSize(m_fWidth-m_fScrollWidth, fH);
		}
	}

	LPARAM  UIListBox::GetItemData(int index)	// 아이템 데이타를 조사한다.
	{
		if(index >= (int)m_Items.size() || index < 0)
			return 0;

		return m_Items[index]->Data;
	}

	VOID UIListBox::SetItemData(int index, LPARAM Data)	// 아이템 데이타를 설정한다.
	{
		if(index >= (int)m_Items.size() || index < 0)
			return;

		m_Items[index]->Data = Data;

		m_ScrollBar.SetPageSize(m_iViewItemCount);
	}

	VOID UIListBox::SetTopIndex(int index)	// 화면에 보이는 최상위 인덱스를 조사한다.
	{
		if(index < 0)
			index = 0;

		int Count = GetCount();
		if(Count < index+m_iViewItemCount)
			index = Count-m_iViewItemCount;

		if(index < 0)
			index = 0;

		m_iTopIndex = index;
		m_ScrollBar.SetPosition(m_iTopIndex);
	}

	int  UIListBox::GetTopIndex()				// 화면에 보이는 최상위 인덱스를 조사한다.
	{
		return m_iTopIndex;
	}

	RECT UIListBox::GetItemRect(int index)	// 주어진 항목의 작업영역 좌표를 조사한다.
	{
		RECT rt;
		SetRect(&rt, 0, 0, 0, 0);

		int CurSel = index-GetTopIndex();
		if(CurSel < 0 || CurSel >= m_iViewItemCount)
			return rt;

		if(m_ScrollBar.IsShowScroll())
			rt.right = (int)(m_fWidth-m_fScrollWidth);
		else
			rt.right = (int)m_fWidth;

		rt.top = CurSel*m_iItemHeight+2;
		rt.bottom = rt.top+m_iItemHeight+2;

		return rt;
	}

	int	 UIListBox::GetTextLen(int index)		// 주어진 항목의 문자열 길이를 조사한다.
	{
		if(index >= (int)m_Items.size() || index < 0)
			return 0;

		return (int)m_Items[index]->Title.size();
	}

	VOID UIListBox::GetText(int index, WCHAR* Buff)		// 주어진 항목의 문자열을 조사한다.
	{
		if(index >= (int)m_Items.size() || index < 0)
			return;

		wcscpy(Buff, m_Items[index]->Title.c_str());
	}

	VOID UIListBox::SetCurSel(int index)
	{
		if(index >= (int)m_Items.size() || index < 0)
			return;

		m_iSelectedItem = index;
	}

	int UIListBox::GetCurSel()
	{
		return m_iSelectedItem;
	}
}