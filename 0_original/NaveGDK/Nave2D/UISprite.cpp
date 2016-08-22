#include <Nave/Nave.h>
#include "UISprite.h"

using namespace NaveFramework;

namespace Nave2D { 

	UISprite::UISprite(void)
	{
		m_IsVisible	= TRUE;

		m_fX		= 0;
		m_fY		= 0;
		m_fWidth	= 0;
		m_fHeight	= 0;

		m_fScaleX	= 1.0f;
		m_fScaleY	= 1.0f;
		m_RotaionZ	= 0.0f;

		m_RotCenter.x = 0;
		m_RotCenter.y = 0;

		m_fOpacity = 255;
		m_dwColor = D3DCOLOR_RGBA(255, 255, 255, (BYTE)m_fOpacity);

		m_pTexture	= NULL;

		SetRect(&m_BltSrcRect, 0, 0, 0, 0);

		SetBlendMode(BLEND_NONE);
	}

	UISprite::~UISprite(void)
	{
		OnDestroy();
	}

	VOID UISprite::SetImageInfo(LPSPRITEINFO pImage)
	{
		SetImage(pImage->pTexture, pImage->fX, pImage->fY, pImage->fWidth, pImage->fHeight);
	}

}