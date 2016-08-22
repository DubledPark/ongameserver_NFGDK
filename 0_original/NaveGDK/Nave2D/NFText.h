/** 
 *  @file		NFText.h
 *  @brief		ȭ�鿡 �ؽ�Ʈ�� ����մϴ�.
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NFVideoDX9.h>
#include "UISprite.h"

namespace Nave2D { 

	/** 
	 *  @class         NFText
	 *  @brief		   ȭ�鿡 �ؽ�Ʈ�� ����մϴ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFText
	{
	public:
		/// NFText ������
		NFText(void);
		/// NFText �Ҹ���
		~NFText(void);

	public:
		/// ���� ���
		inline DWORD		GetOpacity()			{ return m_dwOpacity; }
		/// ���� ����.
		VOID				SetOpacity( DWORD iOpacity );

		/// �ؽ�Ʈ ��Ʈ ����
		VOID				SetFont(HFONT hFont)	{ m_hFont = hFont; }

		/// ��ü �Ҹ�
		VOID				OnDestroy();
		/// ��ü ����
		HRESULT				OnCreate(LPD3DDEVICE pDevice = NULL);	// ��Ʈ DC ����

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

		/// ȭ�鿡 ������
		VOID				OnRender(int X, int Y);

	private:
		/// Text�� Bitmap �����ϱ�
		VOID				CreateTextBitmap(float Width, float Height);

	private:
		/// D3D Device ��ü
		LPD3DDEVICE				m_pDevice;
		
		/// ����ϴ� �ؽ�Ʈ�� ID
		DWORD					m_dwTexture;
		/// �ؽ�Ʈ�� �ؽ��� ��ü
		LPD3DTEXTURE			m_pTexture;

		/// �ؽ�Ʈ ��¿� ��������Ʈ ��ü
		UISprite				m_Sprite;

		/// ����
		DWORD					m_dwOpacity;
		/// �ؽ����� ����ũ��
		float					m_fWidth;
		/// �ؽ����� ����ũ��
		float					m_fHeight;

		/// �ؽ��� ��� ��Ʈ
		HFONT					m_hFont;
	};

}