/** 
 *  @file		NFText.h
 *  @brief		화면에 텍스트를 출력합니다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NFVideoDX9.h>
#include "UISprite.h"

namespace Nave2D { 

	/** 
	 *  @class         NFText
	 *  @brief		   화면에 텍스트를 출력합니다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFText
	{
	public:
		/// NFText 생성자
		NFText(void);
		/// NFText 소멸자
		~NFText(void);

	public:
		/// 투명도 얻기
		inline DWORD		GetOpacity()			{ return m_dwOpacity; }
		/// 투명도 설정.
		VOID				SetOpacity( DWORD iOpacity );

		/// 텍스트 폰트 설정
		VOID				SetFont(HFONT hFont)	{ m_hFont = hFont; }

		/// 객체 소멸
		VOID				OnDestroy();
		/// 객체 생성
		HRESULT				OnCreate(LPD3DDEVICE pDevice = NULL);	// 폰트 DC 생성

		HRESULT				TextOut(DWORD iOpacity, COLORREF TextColor, const WCHAR *szStr);
		HRESULT				TextOutEx(DWORD iOpacity, COLORREF TextColor, WCHAR *szStr, ...);

		HRESULT				DrawText(DWORD iOpacity, RECT DrawRect, COLORREF TextColor, UINT uFormat, const WCHAR *szStr);
		HRESULT				DrawTextEx(DWORD iOpacity, RECT DrawRect, COLORREF TextColor, UINT uFormat, WCHAR *szStr, ...);

		HRESULT				DrawTooltopText(DWORD iOpacity, RECT DrawRect, COLORREF TextColor, UINT uFormat, const WCHAR *szStr);
		HRESULT				DrawTooltopTextEx(DWORD iOpacity, RECT DrawRect, COLORREF TextColor, UINT uFormat, WCHAR *szStr, ...);

		HRESULT				ShadowText(DWORD iOpacity, int ShadowDistance, COLORREF TextColor, COLORREF ShadowColor, const WCHAR *szStr);
		HRESULT				ShadowTextEx(DWORD iOpacity, int ShadowDistance, COLORREF TextColor, COLORREF ShadowColor, WCHAR *szStr, ...);

		HRESULT				OutLineText(DWORD iOpacity, int OutLineThick, RECT DrawRect, COLORREF TextColor, COLORREF OutLineColor, BOOL bShadow, UINT uFormat, const WCHAR *szStr);
		HRESULT				OutLineTextEx(DWORD iOpacity, int OutLineThick, RECT DrawRect, COLORREF TextColor, COLORREF OutLineColor, BOOL bShadow, UINT uFormat, WCHAR *szStr, ...);

		/// 화면에 렌더링
		VOID				OnRender(int X, int Y);

	private:
		/// Text용 Bitmap 생성하기
		VOID				CreateTextBitmap(float Width, float Height);

	private:
		/// D3D Device 객체
		LPD3DDEVICE				m_pDevice;
		
		/// 사용하는 텍스트의 ID
		DWORD					m_dwTexture;
		/// 텍스트용 텍스쳐 객체
		LPD3DTEXTURE			m_pTexture;

		/// 텍스트 출력용 스프라이트 객체
		UISprite				m_Sprite;

		/// 투명값
		DWORD					m_dwOpacity;
		/// 텍스쳐의 가로크기
		float					m_fWidth;
		/// 텍스쳐의 세로크기
		float					m_fHeight;

		/// 텍스쳐 사용 폰트
		HFONT					m_hFont;
	};

}