#include "Nave2D.h"
#include "UIStatic.h"

#include <NaveFramework/NFVideoDX9.h>

using namespace NaveFramework;

namespace Nave2D { 

	UIStatic::UIStatic(void)
	{
	}

	UIStatic::~UIStatic(void)
	{
	}

	BOOL UIStatic::OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID)
	{
		m_dID = nID;

		m_fLocalX = m_fX = fX;
		m_fLocalY = m_fY = fY;
		m_fWidth = fWidth;
		m_fHeight = fHeight;

		m_Sprite.OnCreate(fWidth, fHeight);
		OnSetParent(pParent);
		SetVisible(TRUE);

		m_Title.OnCreate();
		return TRUE;
	}

	BOOL UIStatic::OnDestroy()
	{
		m_Title.OnDestroy();
		return TRUE;
	}

	VOID UIStatic::SetText(const WCHAR* buff)
	{
		m_szText = buff;

		RECT rt = { (LONG)0, (LONG)0, (LONG)m_fWidth, (LONG)m_fHeight};
		m_Title.DrawText(GetOpacity(), rt, RGB(255,255,255), m_dwAlign, buff);
	}

	const Nave::String UIStatic::GetText()
	{
		return m_szText;
	}

	VOID UIStatic::SetAlign(DWORD dwAlign)
	{
		m_dwAlign = dwAlign;
	}

	VOID UIStatic::OnRender()
	{
		if(!IsVisible()) return;

		NFVideoDX9::GetInstance().OnDraw(&m_Sprite, (int)m_fX, (int)m_fY);

		m_Title.OnRender((int)m_fX, (int)m_fY);
	}
}
