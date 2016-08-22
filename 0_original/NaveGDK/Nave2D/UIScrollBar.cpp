#include "Nave2D.h"
#include "UIScrollBar.h"
#include <NaveFramework/NFVideoDX9.h>

using namespace NaveFramework;

namespace Nave2D { 

	UIScrollBar::UIScrollBar(void)
	{
		m_bShowScroll = FALSE;

		m_iPosition = 0;			// 스크롤의 현재 포지션
		m_iPageSize = 1;			// PageDown,Up을 이용하거나 Thumb의 상단 Track, 하단 Track를 눌렀을대 이동되는 라인수
		m_iStart = 0;				// 시작위치
		m_iEnd = 1;					// 종료위치

		m_bThumbDrag = FALSE;
		m_iThumbOffsetY = 0;

		m_iMoveState = ST_NONE;
		m_iDownState = ST_NONE;
		m_iState = TYPE_NONE;

		m_fScrollBtn_Height = SCROLLBTN_HEIGHT;
		m_fScrollThumb_Height = SCROLLTHUMB_HEIGHT;
		m_bScrollThumbHold = TRUE;

		m_ThumbRect.SetRect(0,0,0,0);
		m_UpButtonRect.SetRect(0,0,0,0);
		m_DownButtonRect.SetRect(0,0,0,0);

		m_ptLastMouse = CPoint(0,0);
	}

	UIScrollBar::~UIScrollBar(void)
	{
	}

	BOOL UIScrollBar::OnDestroy()
	{
		return TRUE;
	}

	LRESULT UIScrollBar::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
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

	BOOL UIScrollBar::OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID)
	{
		m_dID = nID;

		m_fLocalX = m_fX = fX;
		m_fLocalY = m_fY = fY;
		m_fWidth = fWidth;
		m_fHeight = fHeight;

		m_Sprite.OnCreate(fWidth, fHeight);					// 스크롤의 배경
		m_UpButton.OnCreate(fWidth, m_fScrollBtn_Height);		// 스크롤의 Up 버튼
		m_DownButton.OnCreate(fWidth, m_fScrollBtn_Height);	// 스크롤의 Down 버튼
		m_Thumb.OnCreate(fWidth, m_fScrollThumb_Height);		// 스크롤의 Thumb

		OnSetParent(pParent);
		SetVisible(TRUE);
		return TRUE;
	}

	VOID UIScrollBar::SetScrollThumbHold(BOOL bHold, float fScrollThumbHeight)
	{
		m_bScrollThumbHold = bHold;
		m_fScrollThumb_Height = fScrollThumbHeight;
	}

	VOID UIScrollBar::UpdateControl(int iType, int State)
	{
		float fW = m_ButtonSkin.fWidth/2;
		float fH = m_fScrollBtn_Height;

		float SX = m_ButtonSkin.fX;
		float SY = m_ButtonSkin.fY+(fH*State);

		if(iType == ST_UP)
		{
			m_UpButton.SetImage(m_ButtonSkin.pTexture, SX, SY, fW, fH);
			m_UpButton.SetSize(m_fWidth, m_fScrollBtn_Height);		// 스크롤의 Up 버튼
		}
		else if(iType == ST_DOWN)
		{
			SX += fW;
			m_DownButton.SetImage(m_ButtonSkin.pTexture, SX, SY, fW, fH);
			m_DownButton.SetSize(m_fWidth, m_fScrollBtn_Height);	// 스크롤의 Down 버튼
		}
	}
	
	VOID UIScrollBar::OnRender()
	{
		if(!IsVisible()) return;

		if(!m_bShowScroll)
			return;

		NFVideoDX9::GetInstance().OnDraw(&m_Sprite, (int)m_fX, (int)m_fY);

		NFVideoDX9::GetInstance().OnDraw(&m_UpButton, (int)m_UpButtonRect.left, (int)m_UpButtonRect.top);

		NFVideoDX9::GetInstance().OnDraw(&m_DownButton, (int)m_DownButtonRect.left, (int)m_DownButtonRect.top);

		NFVideoDX9::GetInstance().OnDraw(&m_Thumb, (int)m_ThumbRect.left, (int)m_ThumbRect.top);
	}

	VOID UIScrollBar::OnMoveObject(float fX, float fY, float fWidth, float fHeight)
	{
		m_fX = m_fLocalX+fX;
		m_fY = m_fLocalY+fY;

		if(fWidth != 0)
			m_fWidth = fWidth;

		if(fHeight != 0)
			m_fHeight = fHeight;

		UpdateControlRect();
	}

	VOID UIScrollBar::SetPosition( int iPosition )
	{
		if( iPosition < 0 )
			iPosition = 0;

		if( iPosition >= m_iEnd )
			iPosition = m_iEnd - 1;

		if( m_iPosition > iPosition )
			m_iPosition = iPosition;
		else if( m_iPosition + m_iPageSize <= iPosition )
			m_iPosition = iPosition - m_iPageSize + 1;

		UpdateThumbRect();
	}

	VOID UIScrollBar::UpdateControlRect()
	{
		m_Sprite.SetSize(m_fWidth, m_fHeight);					// 스크롤의 배경
		m_UpButton.SetSize(m_fWidth, m_fScrollBtn_Height);		// 스크롤의 Up 버튼
		m_DownButton.SetSize(m_fWidth, m_fScrollBtn_Height);	// 스크롤의 Down 버튼

		// Up, Down 버튼 위치 및 크기 조절
		m_UpButtonRect.SetRect((int)m_fX, (int)m_fY, (int)(m_fX+m_fWidth), (int)(m_fY+m_fScrollBtn_Height));
		m_DownButtonRect.SetRect((int)m_fX, (int)(m_fY+m_fHeight-m_fScrollBtn_Height), (int)(m_fX+m_fWidth), (int)(m_fY+m_fHeight));
		m_TrackRect.SetRect((int)m_fX, (int)m_UpButtonRect.bottom, (int)(m_fX+m_fWidth), (int)m_DownButtonRect.top);

		UpdateThumbRect();
	}

	VOID UIScrollBar::UpdateThumbRect()
	{
		float fY = 0;
		float fThumbH = m_fScrollThumb_Height;
		float MaxThumbSize = (float)m_TrackRect.Height();
        float MaxPosition = (float)(m_iEnd - m_iStart - m_iPageSize);

		if(m_bScrollThumbHold)
		{
			// 높이를 구한다.
			if( m_iEnd - m_iStart > m_iPageSize )
			{
				fY = (m_iPosition-m_iStart) * (MaxThumbSize - fThumbH ) / MaxPosition;

				m_bShowScroll = TRUE;
			}
			else
				m_bShowScroll = FALSE;
		}
		else
		{
			fThumbH = MaxThumbSize;

			// 높이를 구한다.
			if( m_iEnd - m_iStart > m_iPageSize )
			{
				fThumbH = max( MaxThumbSize * m_iPageSize / (m_iEnd-m_iStart) , SCROLL_MINTHUMBSIZE);

				fY = (m_iPosition-m_iStart) * (MaxThumbSize - fThumbH ) / MaxPosition;

				m_bShowScroll = TRUE;
			}
			else
				m_bShowScroll = TRUE;

		}
		m_Thumb.SetSize(m_fWidth, fThumbH);		// 스크롤의 Thumb
		m_ThumbRect.SetRect((int)m_fX, m_UpButtonRect.bottom+(int)fY, (int)(m_fX+m_fWidth), m_UpButtonRect.bottom+(int)(fY+fThumbH));
	}

	VOID UIScrollBar::Scroll( int iDelta )
	{
	    m_iPosition += iDelta;

		UpdateCap();
		   
		UpdateThumbRect();
	}
	
	VOID UIScrollBar::UpdateCap()
	{
		if( m_iPosition < m_iStart || m_iEnd - m_iStart <= m_iPageSize )
		{
			m_iPosition = m_iStart;
		}
		else if( m_iPosition + m_iPageSize > m_iEnd )
		{
			m_iPosition = m_iEnd - m_iPageSize;
		}
	}

	VOID UIScrollBar::SetRange( int iStart, int iEnd )
	{
		m_iStart = iStart;		// 시작위치
		m_iEnd = iEnd;			// 종료위치

		UpdateCap();

		UpdateThumbRect();
	}

	VOID UIScrollBar::SetSkin(LPSPRITEINFO Skin)
	{
		if(Skin)
			m_Skin = *Skin;

		m_Sprite.SetImageInfo(&m_Skin);

		UpdateControlRect();
	}

	VOID UIScrollBar::SetThumbSkin(LPSPRITEINFO ThumbSkin)
	{
		if(ThumbSkin)
			m_ThumbSkin = *ThumbSkin;

		m_Thumb.SetImageInfo(&m_ThumbSkin);

		UpdateControlRect();
	}

	VOID UIScrollBar::SetButtonSkin(LPSPRITEINFO ButtonSkin)
	{
		if(ButtonSkin)
			m_ButtonSkin = *ButtonSkin;

		float fW = m_ButtonSkin.fWidth/2;
		m_fScrollBtn_Height = m_ButtonSkin.fHeight/3;

		m_UpButton.SetImage(m_ButtonSkin.pTexture, m_ButtonSkin.fX, m_ButtonSkin.fY, fW, m_fScrollBtn_Height);

		m_DownButton.SetImage(m_ButtonSkin.pTexture, m_ButtonSkin.fX+fW, m_ButtonSkin.fY, fW, m_fScrollBtn_Height);

		UpdateControlRect();
	}

	VOID UIScrollBar::SetShowArea(float fX, float fY, float fWidth, float fHeight)
	{
		// 이미지의 영역설정
		m_Skin.fX = fX;
		m_Skin.fY = fY;
		m_Skin.fWidth = fWidth;
		m_Skin.fHeight = fHeight;

		SetSkin(NULL);
	}

	// WM_MOUSEMOVE
	BOOL UIScrollBar::OnMouseMove(int iMouseX, int iMouseY)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;
		
		if(!m_bShowScroll)
			return FALSE;

		CPoint pt(iMouseX, iMouseY);

		if(m_bThumbDrag)
		{
            m_ThumbRect.bottom += pt.y - m_iThumbOffsetY - m_ThumbRect.top;
            m_ThumbRect.top = pt.y - m_iThumbOffsetY;

            if( m_ThumbRect.top < m_TrackRect.top )
                OffsetRect( &m_ThumbRect, 0, m_TrackRect.top - m_ThumbRect.top );
            else if( m_ThumbRect.bottom > m_TrackRect.bottom )
                OffsetRect( &m_ThumbRect, 0, m_TrackRect.bottom - m_ThumbRect.bottom );

            int iMaxFirstItem = m_iEnd - m_iStart - m_iPageSize;
			int iMaxThumb = m_TrackRect.Height() - m_ThumbRect.Height();

            m_iPosition = m_iStart +
                          ( m_ThumbRect.top - m_TrackRect.top +
                            iMaxThumb / ( iMaxFirstItem * 2 ) ) * 
                          iMaxFirstItem  / iMaxThumb;

			return TRUE;
		}

		if(m_iMoveState == ST_UP)
		{
			if(!m_UpButtonRect.PtInRect(pt))
			{
				m_iMoveState = ST_NONE;
				m_iState = TYPE_NONE;
				UpdateControl(ST_UP, m_iState);
				return TRUE;
			}
		}
		else if(m_iMoveState == ST_DOWN)
		{
			if(!m_DownButtonRect.PtInRect(pt))
			{
				m_iMoveState = ST_NONE;
				m_iState = TYPE_NONE;
				UpdateControl(ST_DOWN, m_iState);
				return TRUE;
			}
		}

		if(m_iMoveState == ST_NONE)
		{
			if(m_UpButtonRect.PtInRect(pt))
			{
				m_iMoveState = ST_UP;

				if(m_iDownState == ST_UP)
					m_iState = TYPE_LDOWN;
				else
					m_iState = TYPE_OVER;

				UpdateControl(ST_UP, m_iState);
				return TRUE;
			}
			else if(m_DownButtonRect.PtInRect(pt))
			{
				m_iMoveState = ST_DOWN;

				if(m_iDownState == ST_DOWN)
					m_iState = TYPE_LDOWN;
				else
					m_iState = TYPE_OVER;

				UpdateControl(ST_DOWN, m_iState);
				return TRUE;
			}
		}
		return FALSE; 
	}

	// WM_LBUTTONDOWN
	BOOL UIScrollBar::OnLButtonDown(int iMouseX, int iMouseY) 
	{ 
		if(!IsEnable() || !IsVisible())
			return FALSE;

		m_bThumbDrag = FALSE;
		m_iDownState = ST_NONE;

		if(!m_bShowScroll)
			return FALSE;

		if(InRect(iMouseX, iMouseY))
		{
			m_ptLastMouse.x = iMouseX;
			m_ptLastMouse.y = iMouseY;

			if(!m_bShowScroll)
				return TRUE;

			if(m_UpButtonRect.PtInRect(m_ptLastMouse))
			{
				m_iDownState = ST_UP;
				m_iState = TYPE_LDOWN;
				UpdateControl(ST_UP, m_iState);

                if( m_iPosition > m_iStart )
                    --m_iPosition;

				UpdateThumbRect();
				return TRUE;
			}
			else if(m_DownButtonRect.PtInRect(m_ptLastMouse))
			{
				m_iDownState = ST_DOWN;
				m_iState = TYPE_LDOWN;
				UpdateControl(ST_DOWN, m_iState);

                if( m_iPosition + m_iPageSize < m_iEnd )
                    ++m_iPosition;

				UpdateThumbRect();
				return TRUE;
			}
			else if(m_ThumbRect.PtInRect(m_ptLastMouse))
			{
				// Thumb선택
                SetCapture( GetActiveWindow() );
				m_iDownState = ST_THUMB;
				m_bThumbDrag = TRUE;
				m_iThumbOffsetY = m_ptLastMouse.y - m_ThumbRect.top;;
				return TRUE;				
			}
			else if(m_TrackRect.PtInRect(m_ptLastMouse))
			{
                SetCapture( GetActiveWindow() );

				if(m_ThumbRect.top > m_ptLastMouse.y)
				{
					// PageUp
					m_iDownState = ST_THUMB;
                    Scroll( -( m_iPageSize - 1 ) );
				}
				else if(m_ThumbRect.bottom < m_ptLastMouse.y)
				{
					// PageDown
					m_iDownState = ST_THUMB;
                    Scroll( m_iPageSize - 1 );
				}
				return TRUE;
			}
		}

		return FALSE; 
	}

	// WM_LBUTTONUP
	BOOL UIScrollBar::OnLButtonUp(int iMouseX, int iMouseY) 
	{ 
		ReleaseCapture();
		m_bThumbDrag = FALSE;

		if(!IsEnable() || !IsVisible())
			return FALSE;

		if(!m_bShowScroll)
			return FALSE;

		if(m_iDownState == ST_NONE)
			return FALSE;

		// 초기화
		m_iDownState = ST_NONE;

		if(m_iMoveState == ST_NONE)
		{
			m_iState = TYPE_NONE;
			UpdateControl(ST_UP, m_iState);
			UpdateControl(ST_DOWN, m_iState);
			return TRUE;
		}
		else
		{
			m_iState = TYPE_NONE;
			CPoint pt(iMouseX, iMouseY);
			if(m_UpButtonRect.PtInRect(pt))
			{
				m_iState = TYPE_OVER;
			}
			else if(m_DownButtonRect.PtInRect(pt))
			{
				m_iState = TYPE_OVER;
			}
			UpdateControl(m_iMoveState, m_iState);
			return TRUE;
		}

		return FALSE; 
	}

	BOOL UIScrollBar::OnMouseWheel(int iMouseX, int iMouseY, int zDelta)
	{
		if(!IsEnable() || !IsVisible())
			return FALSE;

		CPoint pt(iMouseX, iMouseY);
		if(m_TrackRect.PtInRect(pt))
		{
			if( zDelta > 0 )
				Scroll( -1 );
			else
				Scroll( 1 );

			return TRUE;
		}

		return FALSE;
	}
}
