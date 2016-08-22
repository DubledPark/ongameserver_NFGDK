/** 
 *  @file		NFTextureLoader.h
 *  @brief		텍스쳐 로드 함수
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <map>

namespace Nave2D {

	/** 
	 *  @class        NFTextureLoader
	 *  @brief        텍스쳐 로드 합수
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFTextureLoader
	{
	public:
		//-----------------------------------------------------------------------------
		// Name: CreateTexture()
		// Desc: Helper function to create a texture. It checks the root path first,
		//       then tries the DXSDK media path (as specified in the system registry).
		//-----------------------------------------------------------------------------
		static HRESULT CreateTexture( LPD3DDEVICE pd3dDevice, const WCHAR* strTexture,
									LPD3DTEXTURE* ppTexture,
									D3DFORMAT d3dFormat = D3DFMT_UNKNOWN );

		static HRESULT CreateTextureEx( LPD3DDEVICE pd3dDevice, const WCHAR* strTexture,
									LPD3DTEXTURE* ppTexture,
									BOOL bUseTransparency, D3DCOLOR TransparencyColor,
									D3DFORMAT d3dFormat = D3DFMT_UNKNOWN, UINT MipMap = 1 );

		static HRESULT CreateTextureMemory( LPD3DDEVICE pd3dDevice, LPCVOID pSrcData, UINT SrcDataSize,
											LPD3DTEXTURE* ppTexture);

		static HRESULT CreateTextureMemoryEx( LPD3DDEVICE pd3dDevice, LPCVOID pSrcData, UINT SrcDataSize,
											LPD3DTEXTURE* ppTexture,
											BOOL bUseTransparency, D3DCOLOR TransparencyColor, UINT MipMap = 1);

		static HRESULT CreateColorTexture( LPD3DDEVICE pd3dDevice, D3DCOLOR dcColor,
											LPD3DTEXTURE* ppTexture);

		//-----------------------------------------------------------------------------
		// Name: SetColorKey()
		// Desc: Changes all texels matching the colorkey to transparent, black.
		//-----------------------------------------------------------------------------
		static HRESULT SetColorKey( LPD3DTEXTURE pTexture, D3DCOLOR ColorKey = D3DCOLOR_XRGB(255,0,255) );
	};
}