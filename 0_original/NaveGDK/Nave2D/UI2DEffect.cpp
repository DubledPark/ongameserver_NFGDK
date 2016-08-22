#include <Nave/Nave.h>
#include "UI2DEffect.h"

using namespace NaveFramework;

namespace Nave2D { 

	UI2DEffect::UI2DEffect(void)
	{
	}

	UI2DEffect::~UI2DEffect(void)
	{
	}

	VOID UI2DEffect::SetImageInfo(LPSPRITEINFO pImage)
	{
		SetImage(pImage->pTexture, pImage->fX, pImage->fY, pImage->fWidth, pImage->fHeight);
	}

	VOID UI2DEffect::SetSize(float fWidth, float fHeight)
	{
		NFSprite::SetSize(fWidth, fHeight);
		SetRotationCenter(fWidth/2, fHeight/2);
	}

	VOID UI2DEffect::GetMatrix(D3DXMATRIX& mat, float fX, float fY)
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

		// 이렇게 해주면 회전은 이미지의 중앙.
		// 이미지의 출력위치는 2D 이미지와 동일하게 실제 위치의 좌측상단.
		//mat._41 = (float)iX;
		//mat._42 = (float)iY;
		mat._41 = (float)fX+m_RotCenter.x;
		mat._42 = (float)fY+m_RotCenter.y;

		// Dx9에서만 정식지원.
	//	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  m_dbSrc  );
	//	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, m_dbDest );

	//	g_Video.OnDraw( m_pTexture, &m_BltSrcRect, &mat, &D3DXVECTOR3((float)m_RotCenter.x, (float)m_RotCenter.y, 0.0f),
	//		&D3DXVECTOR3((float)0.0f, (float)0.0f, 0.0f), m_dwColor) ;
		// 맨 마지막은 폴리곤의 컬러값을 말한다.
	}

}