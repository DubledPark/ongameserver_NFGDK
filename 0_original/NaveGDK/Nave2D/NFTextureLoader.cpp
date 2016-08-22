#include <NaveFramework/NaveFramework.h>
#include <NaveFramework/NFAdapter.h>

#include "NFTextureLoader.h"

namespace Nave2D {

	/////////////////////////////////////////////////////////////////////
	// D3D 관련 함수.
	/////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------
	// Name: CreateTexture()
	// Desc: Helper function to create a texture. It checks the root path first,
	//       then tries the DXSDK media path (as specified in the system registry).
	//-----------------------------------------------------------------------------
	HRESULT NFTextureLoader::CreateTexture( LPD3DDEVICE pd3dDevice, const WCHAR* strTexture,
								LPD3DTEXTURE* ppTexture, D3DFORMAT d3dFormat )
	{
		// Get the path to the texture
		// Create the texture using D3DX
		return D3DXCreateTextureFromFile( pd3dDevice, strTexture, ppTexture );
/*		
		return D3DXCreateTextureFromFileEx( pd3dDevice, strTexture, 
					D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, d3dFormat, 
					D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
					D3DX_FILTER_NONE, 0, NULL, NULL, ppTexture );
*/		
	}

	HRESULT NFTextureLoader::CreateTextureEx( LPD3DDEVICE pd3dDevice, const WCHAR* strTexture,
								LPD3DTEXTURE* ppTexture, 
								BOOL bUseTransparency, D3DCOLOR TransparencyColor,
								D3DFORMAT d3dFormat, UINT MipMap )
	{
		// Get the path to the texture
		// 투명컬라
		D3DCOLOR transparencycolor;
		if( bUseTransparency )	transparencycolor = TransparencyColor | 0xff000000;
		else					transparencycolor = 0;

		// Create the texture using D3DX
		// 투명색에 해당하는 색상은 알파값을 0으로 만들어 준다.
		return D3DXCreateTextureFromFileEx( pd3dDevice, strTexture, 
					D3DX_DEFAULT, D3DX_DEFAULT, MipMap, 0, d3dFormat, 
					D3DPOOL_MANAGED, D3DX_FILTER_NONE/*D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR*/, 
					D3DX_FILTER_NONE/*D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR*/, transparencycolor, NULL, NULL, ppTexture );
	}

	HRESULT NFTextureLoader::CreateTextureMemory( LPD3DDEVICE pd3dDevice, LPCVOID pSrcData, UINT SrcDataSize,
										LPD3DTEXTURE* ppTexture)
	{
		return D3DXCreateTextureFromFileInMemory(pd3dDevice, pSrcData, SrcDataSize, ppTexture);
	}

	HRESULT NFTextureLoader::CreateTextureMemoryEx( LPD3DDEVICE pd3dDevice, LPCVOID pSrcData, UINT SrcDataSize,
										LPD3DTEXTURE* ppTexture, 
										BOOL bUseTransparency, D3DCOLOR TransparencyColor, UINT MipMap)
	{
		// 투명컬라
		D3DCOLOR transparencycolor;
		if( bUseTransparency )	transparencycolor = TransparencyColor | 0xff000000;
		else					transparencycolor = 0;

		return D3DXCreateTextureFromFileInMemoryEx(pd3dDevice, pSrcData, SrcDataSize, 
			D3DX_DEFAULT, D3DX_DEFAULT, MipMap, 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_FILTER_NONE/*D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR*/, 
			D3DX_FILTER_NONE/*D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR*/, transparencycolor, NULL, NULL, ppTexture );
	}

	HRESULT NFTextureLoader::CreateColorTexture( LPD3DDEVICE pd3dDevice, D3DCOLOR dcColor, LPD3DTEXTURE* ppTexture)
	{
		DWORD dwWidth  = 128;
		DWORD dwHeight = 128;
		BOOL b555 = FALSE;
		// G550 D3DFMT_X1R5G5B5 지원 못함..
		LPD3DTEXTURE pTexture = NULL;
		if( FAILED( pd3dDevice->CreateTexture( dwWidth, dwHeight, 1, 0, D3DFMT_R5G6B5, D3DPOOL_MANAGED, &pTexture, NULL ) ) ) 
		{
			b555 = TRUE;
			if( FAILED( pd3dDevice->CreateTexture( dwWidth, dwHeight, 1, 0, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED, &pTexture, NULL ) ) ) 
			{
				return E_FAIL;
			}
		}

		if(pTexture == NULL)
			return E_FAIL;

		// Lock the texture
		D3DLOCKED_RECT  d3dlr;
		if( FAILED( pTexture->LockRect( 0, &d3dlr, 0, 0 ) ) )
			return E_FAIL;

		BYTE r = GetRColor(dcColor);
		BYTE g = GetGColor(dcColor);
		BYTE b = GetBColor(dcColor);

		WORD* pSurface = (WORD*)d3dlr.pBits;
		WORD wColor = ((r>>3)<<11) + ((g>>3)<<6) + (b>>3);

		if(b555)
		{
			wColor = ((r>>3)<<10) + ((g>>3)<<5) + (b>>3);
		}

		for(DWORD dwCnt = 0; dwCnt < dwWidth*dwHeight; dwCnt++)
		{
			pSurface[dwCnt] = wColor;
		}

		// Unlock the texture and return OK.
		pTexture->UnlockRect(0);

		*ppTexture = pTexture;

		return S_OK;
	}

	//-----------------------------------------------------------------------------
	// Name: SetColorKey()
	// Desc: Changes all texels matching the colorkey to transparent, black.
	//-----------------------------------------------------------------------------
	HRESULT NFTextureLoader::SetColorKey( LPD3DTEXTURE pTexture, D3DCOLOR ColorKey )
	{
		BYTE r = GetRColor(ColorKey);
		BYTE g = GetGColor(ColorKey);
		BYTE b = GetBColor(ColorKey);

		DWORD dwColorKey = (DWORD)ColorKey;

		// Put the colorkey in the texture's native format
		D3DSURFACE_DESC d3dsd;
		pTexture->GetLevelDesc( 0, &d3dsd );
		if( d3dsd.Format == D3DFMT_A4R4G4B4 )
			dwColorKey = 0xf000 + ((r>>4)<<8) + ((g>>4)<<4) + (b>>4);
		else if( d3dsd.Format == D3DFMT_A1R5G5B5 )	// 
			dwColorKey = 0x8000 + ((r>>3)<<10) + ((g>>3)<<5) + (b>>3);
		else if( d3dsd.Format != D3DFMT_A8R8G8B8 )
			return E_FAIL;

		// Lock the texture
		D3DLOCKED_RECT  d3dlr;
		if( FAILED( pTexture->LockRect( 0, &d3dlr, 0, 0 ) ) )
			return E_FAIL;

		// Scan through each pixel, looking for the colorkey to replace
		if( d3dsd.Format==D3DFMT_A8R8G8B8 )
		{
			for( DWORD y=0; y<d3dsd.Height; y++ )
			{
				for( DWORD x=0; x<d3dsd.Width; x++ )
				{
					// Handle 32-bit formats
					if( ((DWORD*)d3dlr.pBits)[d3dsd.Width*y+x] == dwColorKey )
						((DWORD*)d3dlr.pBits)[d3dsd.Width*y+x] = 0x00000000;
				}
			}
		}
		else
		{
			for( DWORD y=0; y<d3dsd.Height; y++ )
			{
				for( DWORD x=0; x<d3dsd.Width; x++ )
				{
					// Handle 16-bit formats
					if( ((WORD*)d3dlr.pBits)[d3dsd.Width*y+x] == (WORD)dwColorKey )
						((WORD*)d3dlr.pBits)[d3dsd.Width*y+x] = 0x0000;
				}
			}
		}

		// Unlock the texture and return OK.
		pTexture->UnlockRect(0);
		return S_OK;
	}

}
