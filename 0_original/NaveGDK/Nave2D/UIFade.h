/** 
 *  @file		UIFade.h
 *  @brief		화면에 Fade 이팩트를 출력합니다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UISprite.h"

namespace Nave2D {

	/// Fade Notify 메시지
	#define WM_FADE_NOTIFY		WM_USER + 0x0100

	/** 
	 *  @class        UIFade
	 *  @brief        화면에 Fade 이팩트를 출력합니다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIFade : public UISprite
	{
	public:
		/// UIFade 생성자
		UIFade(void);
		/// UIFade 소멸자
		virtual ~UIFade(void);

	public:
		/// Fade 상태
		enum FADERSTATUS 
		{ 
			FADE_OFF,		/// Fade 꺼짐
			FADE_OUT,		/// Fade Out
			FADE_IN,		/// Fade In
			FADE_SETIN,		/// Fade In이 설정됨
			FADE_SETOUT		/// Fade Sut이 설정됨
		};

		/// 객체를 생성한다.
		VOID		OnCreate( float fWidth , float fHeight, int iFadeLevel=2);
		/// 객체를 삭제한다.
		VOID		OnDestroy();

		/// 투명도를 설정합니다.
		VOID		SetOpacity( float fOpacity );

		/// 페이드 상태를 업데이트 합니다.
		VOID		OnUpdate(float dt);

		/// 페이드 인아웃 설정을 합니다.
		VOID		SetFadeInOut( FADERSTATUS fsStatus=FADE_OFF, int iFadeLevel=2 );

		/// 렌더링 활성화 상태를 처리합니다.
		virtual	BOOL IsVisible();

	private:
		/// 페이드 상태
		FADERSTATUS m_fStatus;
		/// 페이트당 진행되는 레벨
		int			m_iFadeLevel;

		/// 사용하는 텍스쳐 번호
		DWORD		m_dwTexture;
	};

}