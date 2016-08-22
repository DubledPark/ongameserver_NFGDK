/** 
 *  @file		UIFade.h
 *  @brief		ȭ�鿡 Fade ����Ʈ�� ����մϴ�.
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UISprite.h"

namespace Nave2D {

	/// Fade Notify �޽���
	#define WM_FADE_NOTIFY		WM_USER + 0x0100

	/** 
	 *  @class        UIFade
	 *  @brief        ȭ�鿡 Fade ����Ʈ�� ����մϴ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIFade : public UISprite
	{
	public:
		/// UIFade ������
		UIFade(void);
		/// UIFade �Ҹ���
		virtual ~UIFade(void);

	public:
		/// Fade ����
		enum FADERSTATUS 
		{ 
			FADE_OFF,		/// Fade ����
			FADE_OUT,		/// Fade Out
			FADE_IN,		/// Fade In
			FADE_SETIN,		/// Fade In�� ������
			FADE_SETOUT		/// Fade Sut�� ������
		};

		/// ��ü�� �����Ѵ�.
		VOID		OnCreate( float fWidth , float fHeight, int iFadeLevel=2);
		/// ��ü�� �����Ѵ�.
		VOID		OnDestroy();

		/// ������ �����մϴ�.
		VOID		SetOpacity( float fOpacity );

		/// ���̵� ���¸� ������Ʈ �մϴ�.
		VOID		OnUpdate(float dt);

		/// ���̵� �ξƿ� ������ �մϴ�.
		VOID		SetFadeInOut( FADERSTATUS fsStatus=FADE_OFF, int iFadeLevel=2 );

		/// ������ Ȱ��ȭ ���¸� ó���մϴ�.
		virtual	BOOL IsVisible();

	private:
		/// ���̵� ����
		FADERSTATUS m_fStatus;
		/// ����Ʈ�� ����Ǵ� ����
		int			m_iFadeLevel;

		/// ����ϴ� �ؽ��� ��ȣ
		DWORD		m_dwTexture;
	};

}