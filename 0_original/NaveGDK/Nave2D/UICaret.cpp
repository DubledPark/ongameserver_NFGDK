#include "Nave2D.h"
#include "UICaret.h"
#include <NaveFramework/NFVideoDX9.h>
#include "NFTextureManager.h"

using namespace NaveFramework;

namespace Nave2D { 

	UICaret::UICaret(void)
	{
		m_CaretFlag = FALSE;
		m_CaretTick = timeGetTime();
		m_fWidth = 16;
		m_fHeight = 16;
	}

	UICaret::~UICaret(void)
	{
		OnDestroy();
	}

	// 기본적인 처리값이다.
	VOID UICaret::OnCreate( )
	{
		NFSprite::OnCreate(m_fWidth, m_fHeight);
		
		LPD3DTEXTURE pFade;
		m_dwTexture = NFTextureManager::GetInstance().CreateColorTexture(NFVideoDX9::GetInstance().Get3DDevice(), D3DCOLOR_XRGB(255, 255, 255), &pFade);

		NFSprite::SetImage(pFade, 0, 0, m_fWidth, m_fHeight);
		NFSprite::SetOpacity(128);
	}

	VOID UICaret::OnDestroy()
	{
		NFTextureManager::GetInstance().ReleaseTexture(m_dwTexture);
		NFSprite::OnDestroy();
	}

	BOOL UICaret::IsVisible()
	{
		if( !NFSprite::IsVisible())
			return FALSE;

		if(abs((int)timeGetTime()-(int)m_CaretTick) > 600)
		{
			m_CaretFlag = !m_CaretFlag;
			m_CaretTick = timeGetTime();
		}

		if(!m_CaretFlag)
			return FALSE;

		return TRUE;
	}

	VOID UICaret::SetCaretSize(float Width, float Height)
	{
		// 텍스쳐 크기를 구한다.
		m_fWidth = Width;
		m_fHeight = Height;

		m_BltSrcRect.left = 0;
		m_BltSrcRect.top = 0;
		m_BltSrcRect.right = (int)m_fWidth;
		m_BltSrcRect.bottom = (int)m_fHeight;
	}

	VOID UICaret::SetCaretColor(COLORREF Color)
	{
		BYTE R = GetRValue(Color);
		BYTE G = GetGValue(Color);
		BYTE B = GetBValue(Color);

		m_dwColor = D3DCOLOR_RGBA(R, G, B, (BYTE)m_fOpacity);
	}

}