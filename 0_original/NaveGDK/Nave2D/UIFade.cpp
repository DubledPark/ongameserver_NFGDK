#include "Nave2D.h"
#include "UIFade.h"
#include <NaveFramework/NFVideoDX9.h>
#include "NFTextureManager.h"

using namespace NaveFramework;

namespace Nave2D {

	UIFade::UIFade(void)
	{
		m_dwTexture = 0;
		m_fOpacity = 0;
		m_fStatus = FADE_OFF;
		m_iFadeLevel = 1;
		m_pTexture = NULL;
	}

	UIFade::~UIFade(void)
	{
 		OnDestroy();
	}

	VOID UIFade::OnCreate( float fWidth , float fHeight, int iFadeLevel)
	{
		NFSprite::OnCreate(fWidth, fHeight);

		m_fStatus = FADE_OFF;
		m_iFadeLevel = iFadeLevel;

		m_dwTexture = NFTextureManager::GetInstance().CreateColorTexture(NFVideoDX9::GetInstance().Get3DDevice(), D3DCOLOR_XRGB(255, 255, 255), &m_pTexture);

		NFSprite::SetImage(m_pTexture, 0, 0, 128, 128);
		NFSprite::SetColor(RGB(0,0,0));

		SetOpacity(255);
	}

	VOID UIFade::OnDestroy()
	{
		NFTextureManager::GetInstance().ReleaseTexture(m_dwTexture);
		NFSprite::OnDestroy();
	}

	VOID UIFade::OnUpdate(float dt)
	{
		float level = (255.0f*dt)/(float)m_iFadeLevel;
		float Opacity = m_fOpacity;
		switch(m_fStatus)
		{
		case FADE_OUT:	Opacity += level;	break;
		case FADE_IN:	Opacity -= level;	break;
	//	case FADE_OUT:	Opacity += m_iFadeLevel;	break;
	//	case FADE_IN:	Opacity -= m_iFadeLevel;	break;
		default: return;
		}

		if( Opacity > 255.0f )
		{ 
			m_fOpacity = 255.0f;	m_fStatus = FADE_OFF; 
			::PostMessage(GetActiveWindow(), WM_FADE_NOTIFY, FADE_OUT, (LPARAM)m_fOpacity);
		}
		else if( Opacity <   0 )
		{ 
			m_fOpacity = 0;	m_fStatus = FADE_OFF; 
			::PostMessage(GetActiveWindow(), WM_FADE_NOTIFY, FADE_IN, (LPARAM)m_fOpacity);
		}
		else 
			m_fOpacity = Opacity;


		SetOpacity(m_fOpacity);
	}

	VOID UIFade::SetOpacity( float fOpacity )
	{
		if(fOpacity < 0.0f)			fOpacity = 0.0f;
		else if(fOpacity > 255.0f)		fOpacity = 255.0f;

		m_fOpacity = fOpacity;
		if(m_fOpacity == 255.0f)
			SetBlendMode(BLEND_NONE);
		else
			SetBlendMode(BLEND_ALPHA);

		BYTE R = GetRColor(m_dwColor);
		BYTE G = GetGColor(m_dwColor);
		BYTE B = GetBColor(m_dwColor);
		m_dwColor = D3DCOLOR_RGBA(R, G, B, (BYTE)m_fOpacity);
	}

	VOID UIFade::SetFadeInOut( FADERSTATUS fsStatus, int iFadeLevel)
	{
		m_fStatus = fsStatus;
		m_iFadeLevel = iFadeLevel;

		switch(fsStatus)
		{
		case FADE_OUT:	m_fOpacity = 0.0f;		break;
		case FADE_IN:	m_fOpacity = 255.0f;	break;
		case FADE_SETIN: m_fOpacity = 0.0f;		break;
		case FADE_SETOUT: m_fOpacity = 255.0f;	break;
		default: return;
		}

		if(m_fStatus == FADE_SETIN || m_fStatus == FADE_SETOUT)
			m_fStatus = FADE_OFF;

		SetOpacity(m_fOpacity);
	}

	BOOL UIFade::IsVisible()
	{
		if(m_fStatus == FADE_OFF && m_fOpacity == 0.0f)	// 투명이면 건너 뛴다.
			return FALSE;

		return TRUE;
	}

}