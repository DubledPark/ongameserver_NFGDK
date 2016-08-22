#include <Nave/Nave.h>
#include "NFSprite.h"

namespace NaveFramework { 

	NFSprite::NFSprite(void)
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

	NFSprite::~NFSprite(void)
	{
		OnDestroy();
	}

	// 기본적인 처리값이다.
	VOID NFSprite::OnCreate( float fWidth, float fHeight)
	{
		m_BltSrcRect.left = 0;
		m_BltSrcRect.top = 0;
		m_BltSrcRect.right = (int)fWidth;
		m_BltSrcRect.bottom = (int)fHeight;

		SetSize(fWidth, fHeight);
	}

	VOID NFSprite::OnDestroy()
	{
		m_pTexture = NULL;
	}

	VOID NFSprite::SetSize(float fWidth, float fHeight)
	{
		m_fWidth	= fWidth;
		m_fHeight	= fHeight;

		float tW = (float)m_BltSrcRect.right-m_BltSrcRect.left;
		float tH = (float)m_BltSrcRect.bottom-m_BltSrcRect.top;

		// 가로 세로 스케일 값을 조절한다.
		m_fScaleX = m_fWidth/tW;
		m_fScaleY = m_fHeight/tH;
	}

	VOID NFSprite::SetColor(COLORREF Color)
	{
		BYTE R = GetRValue(Color);
		BYTE G = GetGValue(Color);
		BYTE B = GetBValue(Color);

		m_dwColor = D3DCOLOR_RGBA(R, G, B, (BYTE)m_fOpacity);
	}

	VOID NFSprite::SetOpacity( float fOpacity )
	{
		if(fOpacity < 0)			fOpacity = 0;
		else if(fOpacity > 255)		fOpacity = 255;

		m_fOpacity = fOpacity;
	//	if(m_fOpacity == 255)
	//		SetBlendMode(BLEND_NONE);
	//	else
	//		SetBlendMode(BLEND_ALPHA);

		BYTE R = GetRColor(m_dwColor);
		BYTE G = GetGColor(m_dwColor);
		BYTE B = GetBColor(m_dwColor);
		m_dwColor = D3DCOLOR_RGBA(R, G, B, (BYTE)m_fOpacity);
	}

	VOID NFSprite::SetImage(LPD3DTEXTURE pTexture, float fSX, float fSY, float fWidth, float fHeight)
	{
		m_pTexture = pTexture;

		m_BltSrcRect.left = (int)fSX;
		m_BltSrcRect.top = (int)fSY;
		m_BltSrcRect.right = (int)(fSX+fWidth);
		m_BltSrcRect.bottom = (int)(fSY+fHeight);

		SetSize(fWidth, fHeight);
	}

	VOID NFSprite::SetPos(float fX, float fY)
	{
		m_fX = fX;
		m_fY = fY;
	}

	VOID NFSprite::SetBlendMode(BLEND BlendMode)
	{
		switch(BlendMode)
		{
		default:
		case BLEND_NONE:	// 그냥 찍기
			m_dbSrc  = D3DBLEND_ONE;
			m_dbDest = D3DBLEND_ZERO;
			break;
		case BLEND_ADD: 	// 덧셈연산. 강한 광원
			m_dbSrc  = D3DBLEND_ONE;
			m_dbDest = D3DBLEND_ONE;
			break;
		// 알파 계열
		case BLEND_ALPHA:	// 알파 반투명
			m_dbSrc  = D3DBLEND_SRCALPHA;
			m_dbDest = D3DBLEND_INVSRCALPHA;
			break;
		case BLEND_TRANSTOWHITEOUT:	// 덧셈연산같이 되면서 투명->화이트아웃 효과.. 102보다 부작용이 덜해보임
			m_dbSrc  = D3DBLEND_SRCALPHA;
			m_dbDest = D3DBLEND_DESTALPHA;
			break;
		case BLEND_SPRBRIGHT:	// 스프라이트의 자체 명도 조절이 된다. 유용할까 -_-
			m_dbSrc  = D3DBLEND_SRCALPHA;
			m_dbDest = D3DBLEND_INVDESTALPHA;
			break;
		case BLEND_LIGHTSRC:	// 일종의 광원
			m_dbSrc  = D3DBLEND_SRCALPHA;
			m_dbDest = D3DBLEND_ONE;
			break;
		case BLEND_SCREEN:		// 덜 흰부분이 덧씌워짐.. 셀로판지랄까.. 검은색일땐 202랑 비슷
			m_dbSrc  = D3DBLEND_INVSRCCOLOR;
			m_dbDest = D3DBLEND_SRCCOLOR;
			break;
		case BLEND_HARDLIGHT:	// 흰색이면 Dodge 검은색이면 Burn
			m_dbSrc  = D3DBLEND_DESTCOLOR;
			m_dbDest = D3DBLEND_DESTCOLOR;
			break;
		case BLEND_INVERSE:		// 반전한다. -_- 흰색이면 좀 흰거같고 검정색이면 좀 검은거같다
			m_dbSrc  = D3DBLEND_INVDESTCOLOR;
			m_dbDest = D3DBLEND_INVDESTCOLOR;
			break;
		} 	
	}

	VOID NFSprite::GetMatrix(D3DXMATRIX& mat, float fX, float fY)
	{
		if( !m_IsVisible )
			return;

		if(m_pTexture == NULL)
			return;

		if(fX == -1 && fY == -1)
		{
			fX = m_fX;
			fY = m_fY;
		}

		// 회전시킨다.
		// 이 스프라이트는 폴리곤이 아니라 2d 오브젝트이기 때문에 z 회전만 지원한다.
		D3DXMATRIX matRotateZ;
		// Scale the world matrix to the size
		D3DXMatrixIdentity( &mat );

		// Scale the world matrix to the size
		mat._11 = m_fScaleX;
		mat._22 = m_fScaleY;
		mat._33 = 1.0f;

		// 중점에서 회전시킨다.
		D3DXMatrixRotationZ( &matRotateZ, m_RotaionZ );
		D3DXMatrixMultiply( &mat, &mat, &matRotateZ );

		mat._41 = fX;
		mat._42 = fY;

		// Dx9에서만 정식지원.
	//	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  m_dbSrc  );
	//	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, m_dbDest );

	//	g_Video.OnDraw( m_pTexture, &m_BltSrcRect, &mat, &D3DXVECTOR3((float)m_RotCenter.x, (float)m_RotCenter.y, 0.0f),
	//		&D3DXVECTOR3((float)0.0f, (float)0.0f, 0.0f), m_dwColor) ;
		// 맨 마지막은 폴리곤의 컬러값을 말한다.
	}

}