#include "Nave2D.h"
#include "UIEditBox.h"
#include <NaveFramework/NFVideoDX9.h>
#include <NaveFramework/NFFontManager.h>

#include "NFTextureManager.h"

#include <Nave/NFTokenizer.h>

using namespace NaveFramework;

namespace Nave2D { 

	std::map<DWORD, UIEditBox*>	UIEditBox::m_EditObject;

	VOID UIEditBox::RegisterEdit(UIEditBox* pThis)
	{
		DWORD dwThis = pThis->GetHashCode();
		m_EditObject[dwThis] = pThis;
	}

	VOID UIEditBox::UnRegisterEdit(UIEditBox* pThis)
	{
		DWORD dwThis = pThis->GetHashCode();

		if(m_EditObject.find(dwThis) != m_EditObject.end())
			m_EditObject.erase(dwThis);
	}

	LRESULT CALLBACK UIEditBox::WndEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{	
		if(UIEditBox::m_EditObject.empty())
			return DefWindowProcW(hWnd, uMsg, wParam, lParam);

		UIEditBox* ThisWnd;
		std::map<DWORD, UIEditBox*>::iterator obj = UIEditBox::m_EditObject.begin();
		std::map<DWORD, UIEditBox*>::iterator end = UIEditBox::m_EditObject.end();

		do{
			ThisWnd = obj->second;
			if(ThisWnd->GetIME() == hWnd)
				return ThisWnd->MsgProcEx(hWnd, uMsg, wParam, lParam);

			++obj;
		}while(obj != end);

		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	UIEditBox::UIEditBox()
	{
		m_hFont = NULL;
		m_dID = 0;
		m_hIME = NULL;
		m_hParent = NULL;

		m_OldWndProc = NULL;

		m_bFocus = FALSE;
		m_bIMEComp = FALSE;
		m_TextColor = RGB(0,0,0);

		m_bReadOnly = FALSE;

		m_fX = 0;
		m_fY = 0;
		m_fWidth = 0;
		m_fHeight = 0;
		m_fItemHeight = 0;
		m_iTopLine = 0;
	}

	UIEditBox::~UIEditBox()
	{
		OnDestroy();
	}

	BOOL UIEditBox::OnDestroy()
	{
		UnRegisterEdit(this);

		m_dID = 0;

		m_TextColor = RGB(0,0,0);

		m_fX = 0;
		m_fY = 0;
		m_fWidth = 0;
		m_fHeight = 0;
		m_fItemHeight = 0;
		m_iTopLine = 0;

		ResetContent();

		m_Text.OnDestroy();
		m_Caret.OnDestroy();
		return TRUE;
	}

	BOOL UIEditBox::CheckString()
	{
		if(m_dwStyle != ES_MULTILINE)
			return TRUE;
					
		if(m_bPasswordMode)
			return TRUE;

		WCHAR Buff[256];
		GetString(Buff);
		CSize Size = Nave::GetStringSize(Buff, m_hFont);

		// ������ ����.
		if(Size.cx >= m_fWidth-20)
		{
			CallWindowProcW(m_OldWndProc, GetIME(), WM_IME_ENDCOMPOSITION, 0, 0);

			float fMaxHeight = (m_vecItem.size()+1) * (m_fItemHeight+2);
			if(m_fHeight <= fMaxHeight)
			{
				return FALSE;
			}
			AddLine();
		}
		return TRUE;
	}

	VOID UIEditBox::DelLine()
	{
		if(m_vecItem.empty())
			return;

		int iIdx = m_vecItem.size()-1;

		LPEDITITEM pItem = m_vecItem[iIdx];
		if(pItem)
		{	
			SetString(pItem->strText.c_str());

			delete pItem;

			m_vecItem.pop_back();
		}
	}

	VOID UIEditBox::AddLine()
	{
		if(m_dwStyle != ES_MULTILINE)
			return;
					
		if(m_bPasswordMode)
			return;

		float fMaxHeight = (float)(m_vecItem.size()+1) * (m_fItemHeight+2);
		if(m_fHeight <= fMaxHeight)
			return;

		LPEDITITEM pItem = new EDITITEM;

		WCHAR Buff[256];
		GetString(Buff);

		pItem->strText = Buff;

		CSize Size = Nave::GetStringSize(Buff, m_hFont);
		RECT Rect = { 0, 0, (int)m_fWidth-4, (int)m_fItemHeight+2 };

		pItem->dcText.OnCreate();
		pItem->dcText.SetFont(m_hFont);
   		pItem->dcText.DrawText(255, Rect, m_TextColor, DT_LEFT|DT_SINGLELINE, Buff);

		m_vecItem.push_back(pItem);

		// �ʱ�ȭ �Ѵ�.
		SetString(L"");
	}

	VOID UIEditBox::ResetContent()
	{
		std::vector<LPEDITITEM>::iterator obj = m_vecItem.begin();
		std::vector<LPEDITITEM>::iterator end = m_vecItem.end();

		while(obj != end)
		{
			delete (*obj);

			++obj;
		}
		m_vecItem.clear();
	}

	LRESULT UIEditBox::MsgProcEx( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		if(hWnd != GetIME())
			return TRUE;

		switch (uMsg)
		{
		// Watch For Window Activate Message
		case WM_ACTIVATE:				
			::PostMessage(Nave::hMainWnd, uMsg, wParam, lParam);
			break;

		case WM_IME_STARTCOMPOSITION:     // ���� ������ ����. WM_IME_COMPOSITION �޽����� ���� �غ� �Ѵ�. 
			if(m_bPasswordMode)
				m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
			else
			{
				if(!CheckString())
				{
					m_bIMEComp = FALSE;
					// ���������� �����ų� ��Ŀ�� �缳���� �Ͼ��� ĳ������� �����Ѵ�.
					m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
					return FALSE;
				}

				// ���������� ���۵Ǿ����� ĳ��ũ�⸸ŭ Ŀ���� �ø���.
				m_bIMEComp = TRUE;
				m_Caret.SetCaretSize((float)m_CaretSize.cx, (float)m_CaretSize.cy);
			}
			break;

		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			m_bFocus = (BOOL)(WM_KILLFOCUS-uMsg);
			m_bIMEComp = FALSE;
			// ���������� �����ų� ��Ŀ�� �缳���� �Ͼ��� ĳ������� �����Ѵ�.
			m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
			break;

		case WM_IME_ENDCOMPOSITION:         // ���� ������ ��. ���յ� ���ڿ� ó���� ������ 
			{
				if(!CheckString())
				{
					m_bIMEComp = FALSE;
					// ���������� �����ų� ��Ŀ�� �缳���� �Ͼ��� ĳ������� �����Ѵ�.
					m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
					return FALSE;
				}

				m_bIMEComp = FALSE;
				// ���������� �����ų� ��Ŀ�� �缳���� �Ͼ��� ĳ������� �����Ѵ�.
				m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
			}
			break;

		case WM_IME_COMPOSITION:          // ���� �������� ���ڿ��̳� �ϼ��� ���ڿ��� �� ȭ�� ���
			// ���������� �ϼ��Ǿ���. 
			// ȭ�鿡 ������Ʈ�Ѵ�.
			if(!CheckString())
			{
				m_bIMEComp = FALSE;
				// ���������� �����ų� ��Ŀ�� �缳���� �Ͼ��� ĳ������� �����Ѵ�.
				m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
				return FALSE;
			}

			CallWindowProcW(m_OldWndProc, hWnd, uMsg, wParam, lParam);

			PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, EDIT_CHAR), (LPARAM)this);

			if(lParam != GCS_RESULTSTR)
			{
				// ���ڰ� �����ϸ�. �ڵ� �ٹٲ��� �Ѵ�.
				OnPaint();
			}
			return TRUE;

		case WM_CHAR:		
			//���ڰ� �������� Ŀ�� ǥ��..
			// �̰� ���������� ���͸� ġ�ų� Tab�� �������� ��� �Ÿ��� ������ �������ؼ�.
			// 13 : VK_RETURN, 9 : VK_TAB
			// ��Ÿ���� ��Ƽ������ �ƴϸ� TAB�̳� RETURN�� ġ�� ���Ҹ��� �����ʰ� �Ѵ�.
			if(wParam == VK_RETURN || wParam == VK_TAB )	
				return FALSE;

			if(wParam == VK_BACK)
			{
				if(GetStringLen() == 0)	// �Ѷ����� ������Ѵ�.
				{
					DelLine();
					return TRUE;
				}
			}
			else if(!CheckString())
			{
				m_bIMEComp = FALSE;
				// ���������� �����ų� ��Ŀ�� �缳���� �Ͼ��� ĳ������� �����Ѵ�.
				m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
				return FALSE;
			}

			CallWindowProcW(m_OldWndProc, hWnd, uMsg, wParam, lParam);

			PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, EDIT_CHAR), (LPARAM)this);

			// �Է��� �Ͼ�� ĳ���� ������Ÿ���� �����Ѵ�.
			m_Caret.InitCaretTime();

			OnPaint();
			return TRUE;

		case WM_KEYUP:
			switch(wParam)
			{
			case VK_SHIFT:
				break;
			}
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_TAB:
				PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, EDIT_TAB), (LPARAM)this);
				break;

			case VK_RETURN:
				if(m_dwStyle == ES_MULTILINE)
					AddLine();

				// ���ڿ��� �Ľ��Ѵ�.
				PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, EDIT_RETURN), (LPARAM)this);
				break;
				
			case VK_ESCAPE:
				PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, EDIT_ESC), (LPARAM)this);
				::SetFocus(Nave::hMainWnd);
				break;

			case VK_SPACE:
			case VK_DELETE:
				CallWindowProcW(m_OldWndProc, hWnd, uMsg, wParam, lParam);

				// �Է��� �Ͼ�� ĳ��Ÿ���� �����Ѵ�.
				m_Caret.InitCaretTime();
				OnPaint();
				break;

			case VK_SHIFT:
				return TRUE;

			case VK_LEFT:
			case VK_RIGHT:
			case VK_END:
			case VK_HOME:
				// ��Ƽ�Է±⿡�� ���úκ��̴�.
				return TRUE;
			}
			break;
		}

		if(m_OldWndProc != NULL)
			return CallWindowProcW(m_OldWndProc, hWnd, uMsg, wParam, lParam);

		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	BOOL UIEditBox::OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID, DWORD Style)
	{
		RegisterEdit(this);

		m_dID = nID;

		m_fLocalX = m_fX = fX;
		m_fLocalY = m_fY = fY;
		m_fWidth = fWidth;
		m_fHeight = fHeight;

		m_dwStyle = Style;
		DWORD dwStyle;

		if(Style == ES_MULTILINE)
		{
			dwStyle = WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | Style;
			m_fItemHeight = 16;
		}
		else
			dwStyle = WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | Style;

		// ��ġ ����Ʈ�� ��� ���ҽ��� ���� �Դ´�.
		m_hIME = CreateWindow( L"edit", L"",
				dwStyle,
				(int)fX, (int)fY, (int)fWidth, (int)fHeight,
				Nave::hMainWnd, (HMENU)nID,
				Nave::hInstance, NULL );

		OnSetParent(pParent);
		SetVisible(TRUE);

		// ������ ����Ŭ����(?)
		// ������ ����Ŭ�����̶� ������ HWND�� �޽����� �޴� ������ ���ν�����
		// ������ �ִ°�����.. �̷��� �ϸ�.. ����Ʈ �����쿡�� �޴� �޽�����
		// WndEditProc �̶� ������ ���ν������� ó���ϰ� ���ش�. 
		m_OldWndProc = (WNDPROC)SetWindowLong(GetIME(), GWL_WNDPROC, (long)WndEditProc);

		// edith ��Ʈ �Ŵ����� ���� FreeType ��Ʈ�� �����Ѵ�.
		SetFont(NFFontManager::GetInstance().GetHFont());

		m_CaretSize = Nave::GetStringSize(L"��", m_hFont);

		m_Text.OnCreate();
		m_Caret.OnCreate();
		m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
		m_Caret.SetBlendMode(UISprite::BLEND_INVERSE);

		m_Sprite.OnCreate(fWidth, fHeight);

		// ����...
		RECT rcRect;
		::SendMessageW(GetIME(), EM_GETRECT, 0, (LPARAM)&rcRect);

		SetRect(&rcRect, rcRect.left, rcRect.top+2, rcRect.right, rcRect.bottom);
		::SendMessageW(GetIME(), EM_SETRECT, 0, (LPARAM)&rcRect);

		SetLimitText(250);

		SetTextColor(RGB(255,255,255));

		SetPasswordMode();
		return TRUE;
	}

	VOID UIEditBox::OnMoveObject(float fX, float fY, float fWidth, float fHeight)
	{
		UIControl::OnMoveObject(fX, fY, fWidth, fHeight);
	}

	VOID UIEditBox::SetSize(float fWidth, float fHeight)
	{
		m_fWidth = fWidth;
		m_fHeight = fHeight;
		m_Sprite.SetSize(m_fWidth, m_fHeight);
	}

	VOID UIEditBox::SetSkin(LPSPRITEINFO Skin)
	{
		UIControl::SetSkin(Skin);
	}

	VOID UIEditBox::SetFont(HFONT hFont)
	{
		m_hFont = hFont;
		::SendMessageW(GetIME(), WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));
		m_Text.SetFont(hFont);

		m_CaretSize = Nave::GetStringSize(L"��", hFont);
		m_fItemHeight = (float)m_CaretSize.cy;
	}

	VOID UIEditBox::SetTextColor(COLORREF TextColor)
	{
		m_TextColor = TextColor;

		m_Caret.SetCaretColor(m_TextColor);
	}

	VOID UIEditBox::SetCaretColor(COLORREF CaretColor)
	{
		m_Caret.SetCaretColor(CaretColor);
	}

	VOID UIEditBox::KillFocus()
	{
		::SetFocus(Nave::hMainWnd);
		return;
	}

	VOID UIEditBox::SetFocus()
	{
		PostMessageEx(NULL, WM_OBJECTEVENT, MAKEWPARAM(m_dID, SET_FOCUS), (LPARAM)this);

		if(m_bReadOnly == TRUE)
			return;

		::SetFocus(GetIME());

		int Len = GetStringLen()+1;
		::SendMessageW(GetIME(), EM_SETSEL, Len, Len);

		m_Caret.InitCaretTime();
		m_Caret.SetCaretSize(2, (float)m_CaretSize.cy);
	}

	int UIEditBox::GetStringLen()
	{
		return GetWindowTextLength(GetIME());
	}

	int UIEditBox::GetString(WCHAR* buff)
	{	
		int Size = ::GetWindowTextLengthW(GetIME());
		ZeroMemory(buff,sizeof(buff));
		::GetWindowTextW(GetIME(), buff, Size+1);

		return (int)wcslen(buff);
	}

	VOID UIEditBox::SetString(const WCHAR *buff)
	{
		::SetWindowTextW(GetIME(), buff);
		::SendMessageW(GetIME(), EM_SETSEL, wcslen(buff)+1, wcslen(buff)+1);
		OnPaint();
	}

	// ��� ��Ʈ���� ��´�. (��Ƽ������ �����Ѵ�.)
	VOID UIEditBox::SetText(const WCHAR* buff)
	{
		if(m_dwStyle == ES_MULTILINE)
		{
			// ��Ƽ�����̸� ���κ��� ©�� AddLine�� ������Ѵ�.
			Nave::NFTokenizerW token(buff, L"\r\n");

			WCHAR* strBuff = NULL;
			int iLine = token.CountTokens();
			for(int i = 0; i < iLine-1; ++i)
			{
				SetString(token.NextToken().c_str());
				AddLine();
			}
			
			SetString(token.NextToken().c_str());
		}
		else
		{
			SetString(buff);
		}
	}

	const Nave::String UIEditBox::GetText()
	{
		Nave::String strBuff = L"";

		WCHAR szBuff[256];
		::GetWindowTextW(GetIME(), szBuff, 256);

		int iLine = m_vecItem.size();
		for(int i = 0; i < iLine; ++i)
		{
			strBuff += m_vecItem[i]->strText;
			strBuff += L"\r\n";
		}

		strBuff += szBuff;

		return strBuff;
	}

	int UIEditBox::GetTextLen()
	{
		int iLen = GetStringLen();

		int iChange = wcslen(L"\r\n");// �ٹٲ޹��� ���μ� ����

		int iLine = m_vecItem.size();
		for(int i = 0; i < iLine; ++i)
		{
			iLen += m_vecItem[i]->strText.size();
			iLen += iChange;
		}

		return iLen;
	}
	
	VOID UIEditBox::SetLimitText(int Max)
	{
		::SendMessageW(GetIME(), EM_LIMITTEXT, (WPARAM)Max, 0);
	}

	VOID UIEditBox::SetPasswordMode(BOOL bPassMode)
	{
		m_bPasswordMode = bPassMode;
	}

	VOID UIEditBox::OnRender()
	{
		if(!IsVisible()) return;

		// �⺻ ��� �׸���
		if(m_Skin.dwTexture != 0)
		{
			UIControl::OnRender();
		}

		int iLine = m_vecItem.size();
		float fY = 0;
		for(int i = m_iTopLine; i < iLine; ++i)
		{
			m_vecItem[i]->dcText.OnRender((int)(m_fX+2), (int)(m_fY+2+fY));
			fY += m_fItemHeight+2;
		}

		m_Text.OnRender((int)(m_fX+2), (int)(m_fY+2+fY));

		if(m_bFocus)
		{
			WCHAR Buff[256];
			::GetWindowTextW(GetIME(), Buff, 256);
			if(m_bPasswordMode)
			{
				int len = wcslen(Buff);
				wcscpy(Buff, L"");
				for(int i = 0; i < len; ++i)
					wcscat(Buff, L"*");
			}
			CSize Size = Nave::GetStringSize(Buff, m_hFont);

			if(Size.cx > (int)m_fWidth)
				Size.cx = (int)m_fWidth-4;

			if(!m_bIMEComp) // �Է� �� ���¿� ���� ĳ�� ��ġ�� �����Ѵ�.
				NFVideoDX9::GetInstance().OnDraw(&m_Caret, (int)(m_fX+2+Size.cx), (int)(m_fY+2+fY));
			else
				NFVideoDX9::GetInstance().OnDraw(&m_Caret, (int)(m_fX+2+Size.cx-m_CaretSize.cx), (int)(m_fY+2+fY));
		}
	}

	VOID UIEditBox::OnPaint()
	{
		WCHAR Buff[256];
		GetString(Buff);

		if(m_bPasswordMode)
		{
			int len = wcslen(Buff);
			wcscpy(Buff, L"");
			for(int i = 0; i < len; ++i)
				wcscat(Buff, L"*");
		}

		CSize Size = Nave::GetStringSize(Buff, m_hFont);

		RECT Rect = { 0, 0, (int)m_fWidth-4, (int)m_fItemHeight+2 };

		if(Size.cx >= m_fWidth-4)
    		m_Text.DrawText(255, Rect, m_TextColor, DT_RIGHT|DT_SINGLELINE, Buff);
		else
    		m_Text.DrawText(255, Rect, m_TextColor, DT_LEFT|DT_SINGLELINE, Buff);
	}

	BOOL UIEditBox::OnLButtonDClick(int iMouseX, int iMouseY)
	{
		return UIEditBox::OnLButtonDown(iMouseX, iMouseY);
	}

	BOOL UIEditBox::OnLButtonDown(int iMouseX, int iMouseY)
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
		return FALSE;
	}

}