#include "Nave2D.h"
#include "UIControl.h"
#include "NFTextureManager.h"
#include <NaveFramework/NFVideoDX9.h>

using namespace NaveFramework;

namespace Nave2D {

	UIControl::UIControl()
	{
		m_bEnable = TRUE;
		m_fLocalX = 0;
		m_fLocalY = 0;
	}

	UIControl::~UIControl()
	{
		UIObject::SetVisible(FALSE);
	}

	BOOL UIControl::OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID)
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

	LRESULT UIControl::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		return UIObject::MsgProc(uMsg, wParam, lParam);
	}

	VOID UIControl::SetBlendMode(UISprite::BLEND BlendMode)
	{
		m_Sprite.SetBlendMode(BlendMode);
	}

	VOID UIControl::SetLocal(float fLocalX, float fLocalY)
	{
		m_fLocalX = fLocalX;
		m_fLocalY = fLocalY;
	}

	VOID UIControl::OnRender()
	{
		if(!IsVisible()) return;

		NFVideoDX9::GetInstance().OnDraw(&m_Sprite, (int)m_fX, (int)m_fY);
	}

	VOID UIControl::OnMoveObject(float fX, float fY, float fWidth, float fHeight)
	{
		m_fX = m_fLocalX+fX;
		m_fY = m_fLocalY+fY;

		if(fWidth != 0)
			m_fWidth = fWidth;

		if(fHeight != 0)
			m_fHeight = fHeight;

		SetSize(m_fWidth, m_fHeight);
		UpdateControlRect();
	}

	VOID UIControl::SetSize(float fWidth, float fHeight)
	{
		m_fWidth = fWidth;
		m_fHeight = fHeight;
		m_Sprite.SetSize(m_fWidth, m_fHeight);
	}

	VOID UIControl::SetSkin(LPSPRITEINFO Skin)
	{
		if(Skin)
			m_Skin = *Skin;

		m_Sprite.SetImageInfo(&m_Skin);

		SetSize(m_Skin.fWidth, m_Skin.fHeight);
		UpdateControlRect();
	}

	VOID UIControl::UpdateControlRect()
	{
	}

	VOID UIControl::SetTexture(const Nave::String& textureName)
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

	VOID UIControl::SetShowArea(float fX, float fY, float fWidth, float fHeight)
	{
		m_Skin.fX = fX;
		m_Skin.fY = fY;
		m_Skin.fWidth = fWidth;
		m_Skin.fHeight = fHeight;

		SetSkin(NULL);
	}
}