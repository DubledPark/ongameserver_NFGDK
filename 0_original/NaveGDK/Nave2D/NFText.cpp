#include <Nave/Nave.h>
#include "NFText.h"
#include "NFTextureManager.h"
#include <NaveFramework/NFFontManager.h>

using namespace NaveFramework;

namespace Nave2D { 

	NFText::NFText(void) : m_fWidth(32), m_fHeight(32), m_dwTexture(0), m_pTexture(NULL), m_hFont(NULL)
	{
	}

	NFText::~NFText(void)
	{
		OnDestroy();
	}

	VOID NFText::OnDestroy()
	{
		NFTextureManager::GetInstance().ReleaseTexture( m_dwTexture );

		m_dwTexture = 0;
		m_pTexture = NULL;
		m_hFont = NULL;
	}

	HRESULT NFText::OnCreate(LPD3DDEVICE pDevice)			// 폰트 DC 생성
	{
		if(pDevice == NULL)
			m_pDevice = NFVideoDX9::GetInstance().Get3DDevice();
		else
			m_pDevice = pDevice;

		m_fWidth = 32;
		m_fHeight = 32;

		m_hFont = NFFontManager::GetInstance().GetHFont();

		m_Sprite.OnCreate(m_fWidth, m_fHeight);
		return S_OK;
	}

	VOID NFText::SetOpacity( DWORD iOpacity )
	{
		m_Sprite.SetOpacity((BYTE)iOpacity);
		m_Sprite.SetBlendMode(UISprite::BLEND_ALPHA);
	}

	VOID NFText::CreateTextBitmap(float Width, float Height)
	{
		float PerW = m_fWidth;
		float PerH = m_fHeight;
		if(Width <= 8)			m_fWidth = 8;
		else if(Width <= 16)	m_fWidth = 16;
		else if(Width <= 32)	m_fWidth = 32;
		else if(Width <= 64)	m_fWidth = 64;
		else if(Width <= 128)	m_fWidth = 128;
		else if(Width <= 256)	m_fWidth = 256;
		else if(Width <= 512)	m_fWidth = 512;
		else					m_fWidth = 1024;

		if(Height <= 8)			m_fHeight = 8;
		else if(Height <= 16)	m_fHeight = 16;
		else if(Height <= 32)	m_fHeight = 32;
		else if(Height <= 64)	m_fHeight = 64;
		else if(Height <= 128)	m_fHeight = 128;
		else if(Height <= 256)	m_fHeight = 256;
		else if(Height <= 512)	m_fHeight = 512;
		else					m_fHeight = 1024;

		if(m_pTexture && PerW == m_fWidth && PerH == m_fHeight)
			return;

		NFTextureManager::GetInstance().ReleaseTexture(m_dwTexture);

		m_dwTexture = NFTextureManager::GetInstance().CreateTexture(m_pDevice, (int)m_fWidth, (int)m_fHeight, D3DFMT_A1R5G5B5, &m_pTexture);

		// 텍스쳐 연결하기
		m_Sprite.SetImage(m_pTexture, 0, 0, m_fWidth, m_fHeight);
		m_Sprite.SetSize(m_fWidth, m_fHeight);
		SetOpacity(m_dwOpacity);
	}

	HRESULT NFText::TextOut(DWORD iOpacity, COLORREF TextColor, const WCHAR *szStr)
	{
		if(iOpacity < 0)			iOpacity = 0;
		else if(iOpacity > 255)		iOpacity = 255;

		m_dwOpacity = iOpacity;

		SIZE Size = Nave::GetStringSize(szStr, m_hFont);
		CreateTextBitmap((float)Size.cx, (float)Size.cy);

		HRESULT			hr;
		LPD3DTEXTURE	pTexture;
		hr = D3DXCreateTexture(m_pDevice, (int)m_fWidth, (int)m_fHeight, D3DPOOL_MANAGED, 0
						, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED, &pTexture);					// 불투명 Texture를 만든다.
		if(FAILED(hr))
			return hr;

		LPDIRECT3DSURFACE9		pSurface	= NULL;
		LPDIRECT3DSURFACE9		pSurfaceDest	= NULL;
		D3DSURFACE_DESC			dsc;

		hr  = pTexture->GetSurfaceLevel(0, &pSurface);
		hr  = pSurface->GetDesc(&dsc);

		HDC	hDC;
		pSurface->GetDC(&hDC);

		if(!hDC)
		{
			pSurface->ReleaseDC(hDC);
			_RELEASE(pTexture);
			_RELEASE(pSurface);
			return E_FAIL;
		}

		HFONT hFontOld;
		hFontOld = (HFONT)SelectObject( hDC, m_hFont );

		SetTextColor(hDC, TextColor);
		SetBkMode(hDC, TRANSPARENT);

		RECT DrawRect = { 0, 0, dsc.Width, dsc.Height };
		HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,255));
		::FillRect(hDC, &DrawRect, hBrush);
		::DeleteObject(hBrush);

		SetRect(&DrawRect, 0, 0, Size.cx, Size.cy);
		::DrawTextW(hDC, szStr, (int)wcslen(szStr), &DrawRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

		SelectObject(hDC, hFontOld);

		hr = pSurface->ReleaseDC(hDC);
		if(FAILED(hr))
			return E_FAIL;

		m_pTexture->GetSurfaceLevel(0, &pSurfaceDest);
		pSurfaceDest->GetDesc(&dsc);

		// 불투명 텍스춰를 반투명으로 옮기되 불투명의 배경으로 사용한 검정색을 컬러키로 설정한다.
		hr = D3DXLoadSurfaceFromSurface( pSurfaceDest, NULL, NULL,						
										pSurface,  NULL, NULL,
										D3DX_DEFAULT, 0xFFFF00FF);

		_RELEASE(pSurface);
		_RELEASE(pSurfaceDest);
		_RELEASE(pTexture);
		return S_OK;
	}

	HRESULT NFText::TextOutEx(DWORD iOpacity, COLORREF TextColor, WCHAR *szStr, ...)
	{
		WCHAR	chBuffer[1024];
		va_list va;
		va_start(va, szStr);
		vswprintf(chBuffer, szStr, va);
		va_end(va);

		return TextOut(iOpacity, TextColor, chBuffer);
	}

	HRESULT	NFText::DrawText(DWORD iOpacity, RECT DrawRect, COLORREF TextColor, UINT uFormat, const WCHAR *szStr)
	{
		if(iOpacity < 0)			iOpacity = 0;
		else if(iOpacity > 255)		iOpacity = 255;

		m_dwOpacity = iOpacity;

		CreateTextBitmap((float)DrawRect.right, (float)DrawRect.bottom);

		HRESULT			hr;
		LPD3DTEXTURE	pTexture;
		hr = D3DXCreateTexture(m_pDevice, (int)m_fWidth, (int)m_fHeight, D3DPOOL_MANAGED, 0
						, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED, &pTexture);					// 불투명 Texture를 만든다.
		if(FAILED(hr))
			return hr;

		LPDIRECT3DSURFACE9		pSurface	= NULL;
		LPDIRECT3DSURFACE9		pSurfaceDest	= NULL;
		D3DSURFACE_DESC			dsc;
		hr  = pTexture->GetSurfaceLevel(0, &pSurface);
		hr  = pSurface->GetDesc(&dsc);

		HDC	hDC;
		pSurface->GetDC(&hDC);

		if(!hDC)
		{
			pSurface->ReleaseDC(hDC);
			_RELEASE(pTexture);
			_RELEASE(pSurface);
			return E_FAIL;
		}

		HFONT hFontOld;
		hFontOld = (HFONT)SelectObject( hDC, m_hFont );

		SetTextColor(hDC, TextColor);
		SetBkMode(hDC, TRANSPARENT);

		RECT FillRect = { 0, 0, dsc.Width, dsc.Height };
		HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,255));
		::FillRect(hDC, &FillRect, hBrush);
		::DeleteObject(hBrush);

		::DrawTextW(hDC, szStr, (int)wcslen(szStr), &DrawRect, uFormat|DT_NOPREFIX);

		SelectObject(hDC, hFontOld);

		hr = pSurface->ReleaseDC(hDC);
		if(FAILED(hr))
			return E_FAIL;

		m_pTexture->GetSurfaceLevel(0, &pSurfaceDest);
		pSurfaceDest->GetDesc(&dsc);

		hr = D3DXLoadSurfaceFromSurface( pSurfaceDest, NULL, NULL,						// 불투명 텍스춰를 반투명으로 옮기되 불투명의 배경으로 사용한 검정색을 컬러키로 설정한다.
										pSurface,  NULL, NULL,
										D3DX_DEFAULT, 0xFFFF00FF);

		_RELEASE(pSurface);
		_RELEASE(pSurfaceDest);
		_RELEASE(pTexture);
		return S_OK;
	}

	HRESULT	NFText::DrawTextEx(DWORD iOpacity, RECT DrawRect, COLORREF TextColor, UINT uFormat, WCHAR *szStr, ...)
	{
		WCHAR	chBuffer[1024];
		va_list va;
		va_start(va, szStr);
		vswprintf(chBuffer, szStr, va);
		va_end(va);

		return DrawText(iOpacity, DrawRect, TextColor, uFormat, chBuffer);
	}

	HRESULT	NFText::DrawTooltopText(DWORD iOpacity, RECT DrawRect, COLORREF TextColor, UINT uFormat, const WCHAR *szStr)
	{
		if(iOpacity < 0)			iOpacity = 0;
		else if(iOpacity > 255)		iOpacity = 255;

		m_dwOpacity = iOpacity;

		CreateTextBitmap((float)DrawRect.right, (float)DrawRect.bottom);

		HRESULT			hr;
		LPD3DTEXTURE	pTexture;
		hr = D3DXCreateTexture(m_pDevice, (int)m_fWidth, (int)m_fHeight, D3DPOOL_MANAGED, 0
						, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED, &pTexture);					// 불투명 Texture를 만든다.
		if(FAILED(hr))
			return hr;

		LPDIRECT3DSURFACE9		pSurface	= NULL;
		LPDIRECT3DSURFACE9		pSurfaceDest	= NULL;
		D3DSURFACE_DESC			dsc;
		hr  = pTexture->GetSurfaceLevel(0, &pSurface);
		hr  = pSurface->GetDesc(&dsc);

		HDC	hDC;
		pSurface->GetDC(&hDC);

		if(!hDC)
		{
			pSurface->ReleaseDC(hDC);
			_RELEASE(pTexture);
			_RELEASE(pSurface);
			return E_FAIL;
		}

		HFONT hFontOld;
		hFontOld = (HFONT)SelectObject( hDC, m_hFont );

		RECT FillRect = { 0, 0, dsc.Width, dsc.Height };
		HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,255));
		::FillRect(hDC, &FillRect, hBrush);
		::DeleteObject(hBrush);

		SIZE sz = Nave::GetStringSize(szStr, m_hFont);
		DrawRect.right = sz.cx+4;
		DrawRect.bottom = sz.cy+4;

		hBrush = ::CreateSolidBrush(RGB(255,255,225));
		HBRUSH OldBursh = (HBRUSH)SelectObject(hDC, hBrush);

		::Rectangle(hDC, 0, 0, DrawRect.right, DrawRect.bottom);

		SelectObject(hDC, OldBursh);
		::DeleteObject((HBRUSH)hBrush);

		SetTextColor(hDC, TextColor);
		SetBkMode(hDC, TRANSPARENT);

		::DrawTextW(hDC, szStr, (int)wcslen(szStr), &DrawRect, uFormat|DT_NOPREFIX);

		SelectObject(hDC, hFontOld);

		hr = pSurface->ReleaseDC(hDC);
		if(FAILED(hr))
			return E_FAIL;

		m_pTexture->GetSurfaceLevel(0, &pSurfaceDest);
		pSurfaceDest->GetDesc(&dsc);

		hr = D3DXLoadSurfaceFromSurface( pSurfaceDest, NULL, NULL,						// 불투명 텍스춰를 반투명으로 옮기되 불투명의 배경으로 사용한 검정색을 컬러키로 설정한다.
										pSurface,  NULL, NULL,
										D3DX_DEFAULT, 0xFFFF00FF);

		_RELEASE(pSurface);
		_RELEASE(pSurfaceDest);
		_RELEASE(pTexture);
		return S_OK;
	}

	HRESULT	NFText::DrawTooltopTextEx(DWORD iOpacity, RECT DrawRect, COLORREF TextColor, UINT uFormat, WCHAR *szStr, ...)
	{
		WCHAR	chBuffer[1024];
		va_list va;
		va_start(va, szStr);
		vswprintf(chBuffer, szStr, va);
		va_end(va);

		return DrawTooltopText(iOpacity, DrawRect, TextColor, uFormat, chBuffer);
	}

	HRESULT	NFText::ShadowText(DWORD iOpacity, int ShadowDistance, COLORREF TextColor, COLORREF ShadowColor, const WCHAR *szStr)
	{
		if(iOpacity < 0)			iOpacity = 0;
		else if(iOpacity > 255)		iOpacity = 255;

		if(ShadowColor == RGB(0,0,0))
			ShadowColor = RGB(8,8,8);

		m_dwOpacity = iOpacity;

		SIZE Size = Nave::GetStringSize(szStr, m_hFont);
		CreateTextBitmap((float)Size.cx+ShadowDistance, (float)Size.cy+ShadowDistance);

		HRESULT			hr;
		LPD3DTEXTURE	pTexture;
		hr = D3DXCreateTexture(m_pDevice, (int)m_fWidth, (int)m_fHeight, D3DPOOL_MANAGED, 0
						, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED, &pTexture);					// 불투명 Texture를 만든다.
		if(FAILED(hr))
			return hr;

		LPDIRECT3DSURFACE9		pSurface	= NULL;
		LPDIRECT3DSURFACE9		pSurfaceDest	= NULL;
		D3DSURFACE_DESC			dsc;
		hr  = pTexture->GetSurfaceLevel(0, &pSurface);
		hr  = pSurface->GetDesc(&dsc);

		HDC	hDC;
		pSurface->GetDC(&hDC);

		if(!hDC)
		{
			pSurface->ReleaseDC(hDC);
			_RELEASE(pTexture);
			_RELEASE(pSurface);
			return E_FAIL;
		}

		HFONT hFontOld;
		hFontOld = (HFONT)SelectObject( hDC, m_hFont );

		SetBkMode(hDC, TRANSPARENT);
		SetBkColor(hDC, RGB(0,0,0));

		RECT FillRect = { 0, 0, dsc.Width, dsc.Height };
		HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,255));
		::FillRect(hDC, &FillRect, hBrush);
		::DeleteObject(hBrush);

		RECT DrawRect;

		// 그림자
		SetTextColor(hDC, ShadowColor);
		SetRect(&DrawRect, ShadowDistance, ShadowDistance, Size.cx+ShadowDistance, Size.cy+ShadowDistance);
		::DrawTextW(hDC, szStr, (int)wcslen(szStr), &DrawRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

		SetTextColor(hDC, TextColor);
		SetRect(&DrawRect, 0, 0, Size.cx, Size.cy );
		::DrawTextW(hDC, szStr, (int)wcslen(szStr), &DrawRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

		SelectObject(hDC, hFontOld);

		hr = pSurface->ReleaseDC(hDC);
		if(FAILED(hr))
			return E_FAIL;

		m_pTexture->GetSurfaceLevel(0, &pSurfaceDest);
		pSurfaceDest->GetDesc(&dsc);

		hr = D3DXLoadSurfaceFromSurface( pSurfaceDest, NULL, NULL,						// 불투명 텍스춰를 반투명으로 옮기되 불투명의 배경으로 사용한 검정색을 컬러키로 설정한다.
										pSurface,  NULL, NULL,
										D3DX_DEFAULT, 0xFFFF00FF);

		_RELEASE(pSurface);
		_RELEASE(pSurfaceDest);
		_RELEASE(pTexture);
		return S_OK;
	}

	HRESULT	NFText::ShadowTextEx(DWORD iOpacity, int ShadowDistance, COLORREF TextColor, COLORREF ShadowColor, WCHAR *szStr, ...)
	{
		WCHAR	chBuffer[1024];
		va_list va;
		va_start(va, szStr);
		vswprintf(chBuffer, szStr, va);
		va_end(va);

		return ShadowText(iOpacity, ShadowDistance, TextColor, ShadowColor, chBuffer);
	}

	HRESULT	NFText::OutLineText(DWORD iOpacity, int OutLineThick, RECT DrawRect, COLORREF TextColor, COLORREF OutLineColor, BOOL bShadow, UINT uFormat, const WCHAR *szStr)
	{
		if(iOpacity < 0)			iOpacity = 0;
		else if(iOpacity > 255)		iOpacity = 255;

		if(OutLineColor == RGB(0,0,0))
			OutLineColor = RGB(8,8,8);

		m_dwOpacity = iOpacity;

		CreateTextBitmap((float)DrawRect.right+(OutLineThick*3), (float)DrawRect.bottom+(OutLineThick*3));

		HRESULT			hr;
		LPD3DTEXTURE	pTexture;
		hr = D3DXCreateTexture(m_pDevice, (int)m_fWidth, (int)m_fHeight, D3DPOOL_MANAGED, 0
						, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED, &pTexture);					// 불투명 Texture를 만든다.
		if(FAILED(hr))
			return hr;

		LPDIRECT3DSURFACE9		pSurface	= NULL;
		LPDIRECT3DSURFACE9		pSurfaceDest	= NULL;
		D3DSURFACE_DESC			dsc;
		hr  = pTexture->GetSurfaceLevel(0, &pSurface);
		hr  = pSurface->GetDesc(&dsc);

		HDC	hDC;
		pSurface->GetDC(&hDC);

		if(!hDC)
		{
			pSurface->ReleaseDC(hDC);
			_RELEASE(pTexture);
			_RELEASE(pSurface);
			return E_FAIL;
		}

		HFONT hFontOld;
		hFontOld = (HFONT)SelectObject( hDC, m_hFont );

		SetBkMode(hDC, TRANSPARENT);
		SetBkColor(hDC, RGB(0,0,0));

		RECT FillRect = { 0, 0, dsc.Width, dsc.Height };
		HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,255));
		::FillRect(hDC, &FillRect, hBrush);
		::DeleteObject(hBrush);

		// 그림자
		RECT DrawRect1;
		if(bShadow)
		{
			int iDist = OutLineThick*2;
			SetTextColor(hDC, RGB(8,8,8));
			SetRect(&DrawRect1, DrawRect.left+iDist, DrawRect.top+iDist, DrawRect.right+iDist, DrawRect.bottom+iDist );
			::DrawTextW(hDC, szStr, (int)wcslen(szStr), &DrawRect, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		}

		SetTextColor(hDC, OutLineColor);
		for(int x = 0; x < OutLineThick*3; x+=OutLineThick)
		{
			for(int y = 0; y < OutLineThick*3; y+=OutLineThick)
			{
				if(x==OutLineThick && y==OutLineThick)
					continue;

				SetRect(&DrawRect1, DrawRect.left+x, DrawRect.top+y, DrawRect.right+x, DrawRect.bottom+y );
				::DrawTextW(hDC, szStr, (int)wcslen(szStr), &DrawRect1, uFormat|DT_NOPREFIX);
			}
		}

		SetTextColor(hDC, TextColor);
		SetRect(&DrawRect, DrawRect.left+OutLineThick, DrawRect.top+OutLineThick, DrawRect.right+OutLineThick, DrawRect.bottom+OutLineThick );
		::DrawTextW(hDC, szStr, (int)wcslen(szStr), &DrawRect, uFormat|DT_NOPREFIX);

		SelectObject(hDC, hFontOld);

		hr = pSurface->ReleaseDC(hDC);
		if(FAILED(hr))
			return E_FAIL;

		m_pTexture->GetSurfaceLevel(0, &pSurfaceDest);
		pSurfaceDest->GetDesc(&dsc);

		hr = D3DXLoadSurfaceFromSurface( pSurfaceDest, NULL, NULL,						// 불투명 텍스춰를 반투명으로 옮기되 불투명의 배경으로 사용한 검정색을 컬러키로 설정한다.
										pSurface,  NULL, NULL,
										D3DX_DEFAULT, 0xFFFF00FF);

		_RELEASE(pSurface);
		_RELEASE(pSurfaceDest);
		_RELEASE(pTexture);

		return S_OK;
	}

	HRESULT	NFText::OutLineTextEx(DWORD iOpacity, int OutLineThick, RECT DrawRect, COLORREF TextColor, COLORREF OutLineColor, BOOL bShadow, UINT uFormat, WCHAR *szStr, ...)
	{
		WCHAR	chBuffer[1024];
		va_list va;
		va_start(va, szStr);
		vswprintf(chBuffer, szStr, va);
		va_end(va);

		return OutLineText(iOpacity, OutLineThick, DrawRect, TextColor, OutLineColor, bShadow, uFormat, chBuffer);
	}

	VOID NFText::OnRender(int X, int Y)
	{
		NFVideoDX9::GetInstance().OnDraw(&m_Sprite, X, Y);
	}
}