#include <Nave/Nave.h>
#include "NFImage.h"
#include "NFTextureManager.h"
#include <NaveFramework/NFVideoDX9.h>

using namespace NaveFramework;

namespace Nave2D {

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	NFImage::NFImage()
	{
		// 로드한 이미지가 저장될 객체
		m_dwTexture = 0;
		m_pTexture = NULL;

		m_fWidth = 0.0f;
		m_fHeight = 0.0f;

		m_iSpriteCnt = 0;	// 이미지 개수로 m_pSpriteInfo의 배열개수를 갖는다.
		m_pSpriteInfo = NULL;   

		m_bKeyColor = FALSE;
		m_dwKeyColor = D3DCOLOR_XRGB(0,0,0);

		SetDefaultSpriteInfo();
	}

	NFImage::~NFImage()
	{
		ReleaseAll();
	}

	VOID NFImage::ReleaseAll()
	{
		NFTextureManager::GetInstance().ReleaseTexture(m_dwTexture);

		if(m_pSpriteInfo != NULL)
			_DELETE_ARRAY(m_pSpriteInfo);

		m_pSpriteInfo = NULL;
		m_iSpriteCnt = 0;
	}

	VOID NFImage::SetKeyColor(COLORREF KeyColor)
	{
		m_bKeyColor = TRUE;
		BYTE r = GetRValue(KeyColor);
		BYTE g = GetGValue(KeyColor);
		BYTE b = GetBValue(KeyColor);
		m_dwKeyColor = D3DCOLOR_XRGB(r, g, b);
	}

	BOOL NFImage::CreateEmptyTexture(int iWidth, int iHeight, D3DFORMAT d3dFormat)
	{
		m_dwTexture = NFTextureManager::GetInstance().CreateTexture(NFVideoDX9::GetInstance().Get3DDevice(), iWidth, iHeight, d3dFormat, &m_pTexture);

		if(m_pTexture==NULL)
			return FALSE;

		return TRUE;
	}

	VOID NFImage::FillTexture( BYTE* pByte) 
	{
		D3DSURFACE_DESC dTextureSurf;
		LPD3DTEXTURE	lpEmptyTexture;

		lpEmptyTexture = (LPD3DTEXTURE)m_pTexture;
		lpEmptyTexture->GetLevelDesc( 0, &dTextureSurf );

		D3DLOCKED_RECT lRect;
		HRESULT hr = lpEmptyTexture->LockRect( 0, &lRect, NULL,  0);
		if(hr == D3DERR_INVALIDCALL)
			return;

		byte *pSurface = (BYTE *)lRect.pBits;
		unsigned int iImageSize = dTextureSurf.Width * dTextureSurf.Height;

		memcpy( pSurface, pByte, sizeof(BYTE)*iImageSize);
		
		lpEmptyTexture->UnlockRect( 0 );
	}

	VOID NFImage::SetBitTexture16( int iWidth, int iHeight, WORD* pSrc )
	{
		D3DSURFACE_DESC		dTextureSurf;
		LPD3DTEXTURE		lpEmptyTexture;
		D3DLOCKED_RECT		lRect;

		lpEmptyTexture = ( LPD3DTEXTURE )m_pTexture;
		lpEmptyTexture->GetLevelDesc( 0, &dTextureSurf );
		
		HRESULT hr = lpEmptyTexture->LockRect( 0, &lRect, NULL, 0 );
		if ( hr == D3DERR_INVALIDCALL )
			return;

		WORD* pSurface = ( WORD * )lRect.pBits;

		BYTE* pbyTempBuff = ( BYTE* )pSrc;
		WORD* pwTempBuff  = ( WORD* )( pbyTempBuff + 1 );
		
		int CurSrc = 0;
		int CurDest = 0;

		for ( int ix = 0; ix < iHeight; ++ix )
		{
			CurDest = dTextureSurf.Width * ix;
			for ( int iy = 0; iy < iWidth; ++iy )
			{
				pSurface[ CurDest ] = pwTempBuff[ CurSrc ];
				++ CurDest;
				++ CurSrc;
			}
		}

		lpEmptyTexture->UnlockRect( 0 );
	}

	VOID NFImage::SetBitTexture32( int iWidth, int iHeight, DWORD* pSrc )
	{
		D3DSURFACE_DESC		dTextureSurf;
		LPD3DTEXTURE			lpEmptyTexture;
		D3DLOCKED_RECT		lRect;

		lpEmptyTexture = ( LPD3DTEXTURE )m_pTexture;
		lpEmptyTexture->GetLevelDesc( 0, &dTextureSurf );

		HRESULT hr = lpEmptyTexture->LockRect( 0, &lRect, NULL, 0 );
		if ( hr == D3DERR_INVALIDCALL )
			return;

		DWORD* pSurface = ( DWORD * )lRect.pBits;
		DWORD* pwTempBuff = ( DWORD *)pSrc;

		int CurSrc = 0;
		int CurDest = 0;

		for ( int ix = 0;ix < iHeight; ++ix )
		{
			CurDest = dTextureSurf.Width * ix;
			for ( int iy = 0; iy < iWidth; ++iy )
			{
				pSurface[ CurDest ] = pwTempBuff[ CurSrc ];
				++ CurDest;
				++ CurSrc;
			}
		}

		lpEmptyTexture->UnlockRect( 0 );
	}

	BOOL NFImage::CreateImageArea(int iMaxSpriteArea)
	{
		if(iMaxSpriteArea == 0)
			return FALSE;

		m_iSpriteCnt = iMaxSpriteArea;
		m_pSpriteInfo = new SPRITEINFO[iMaxSpriteArea];

		if(m_pSpriteInfo == NULL)
			return FALSE;

		return TRUE;
	}

	BOOL NFImage::SetImageArea(int Index, float X, float Y, float Width, float Height)
	{
		if(Index >= m_iSpriteCnt)
			return FALSE;

		if(m_pSpriteInfo == NULL)
			return FALSE;

		m_pSpriteInfo[Index].dwTexture = m_dwTexture;
		m_pSpriteInfo[Index].fX = X;
		m_pSpriteInfo[Index].fY = Y;
		m_pSpriteInfo[Index].fWidth = Width;
		m_pSpriteInfo[Index].fHeight = Height;
		m_pSpriteInfo[Index].pTexture = m_pTexture;
		return TRUE;
	}

	BOOL NFImage::LoadResourceFile(LPD3DDEVICE pDevice, const WCHAR* ImageFile)
	{
		if(m_bKeyColor == TRUE)
			m_dwTexture = NFTextureManager::GetInstance().CreateTexture(pDevice, ImageFile, TRUE, m_dwKeyColor, &m_pTexture);
		else
			m_dwTexture = NFTextureManager::GetInstance().CreateTexture(pDevice, ImageFile, &m_pTexture);

		if(m_pTexture == NULL)
			return FALSE;

		D3DSURFACE_DESC d3dsd;
		m_pTexture->GetLevelDesc( 0, &d3dsd );
		m_fWidth = (float)d3dsd.Width;		// 이미지의 사이즈
		m_fHeight = (float)d3dsd.Height;

		SetDefaultSpriteInfo();
		return TRUE;
	}

	BOOL NFImage::LoadResourceMemory(LPD3DDEVICE pDevice, const WCHAR* TextureID, char* FilePoint, DWORD FileLen)
	{
		if(m_bKeyColor == TRUE)
			m_dwTexture = NFTextureManager::GetInstance().CreateTextureMemory(pDevice, TextureID, FilePoint, FileLen, TRUE, m_dwKeyColor, &m_pTexture);
		else
			m_dwTexture = NFTextureManager::GetInstance().CreateTextureMemory(pDevice, TextureID, FilePoint, FileLen, &m_pTexture);

		if(m_pTexture == NULL)
			return FALSE;

		D3DSURFACE_DESC d3dsd;
		m_pTexture->GetLevelDesc( 0, &d3dsd );
		m_fWidth = (float)d3dsd.Width;		// 이미지의 사이즈
		m_fHeight = (float)d3dsd.Height;

		SetDefaultSpriteInfo();
		return TRUE;
	}

	void NFImage::SetDefaultSpriteInfo()
	{
		m_stDefault.pTexture = m_pTexture;
		m_stDefault.fX = 0;
		m_stDefault.fY = 0;
		m_stDefault.fWidth = m_fWidth;
		m_stDefault.fHeight = m_fHeight;
	}

	BOOL NFImage::LoadFile(LPD3DDEVICE pDevice, const WCHAR* ImageFile)
	{
		LoadResourceFile(pDevice, ImageFile);
		return TRUE;
	}

	BOOL NFImage::LoadMemory(LPD3DDEVICE pDevice, const WCHAR* TextureID, char* FilePoint, DWORD FileLen)
	{
		LoadResourceMemory(pDevice, TextureID, FilePoint, FileLen);
		return TRUE;
	}

}