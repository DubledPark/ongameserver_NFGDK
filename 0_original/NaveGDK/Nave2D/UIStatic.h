/** 
 *  @file		UIStatic.h
 *  @brief		스태틱 컨트롤
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIControl.h"
#include "NFText.h"

namespace Nave2D { 

	/** 
	 *  @class        UIStatic
	 *  @brief        스태틱을 생성하고 관리하는 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIStatic : public UIControl
	{
	public:
		/// UIStatic 생성자
		UIStatic(void);
		/// UIStatic 소멸자
		virtual ~UIStatic(void);

	public:
		/**
		 * @brief	스태틱을 생성합니다.
		 * @param pParent	부모포인트
		 * @param fX		로컬X
		 * @param fY		로컬Y
		 * @param fWidth	가로크기
		 * @param fHeight	세로크기
		 * @param nID		객체 아이디
		 * @warning	fX,fY의 값은 부모의 시작점에서의 로컬좌표이다.
		 * @return	성공여부
		 */
		virtual BOOL OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID);
		/// 스태틱을 삭제한다.
		virtual BOOL OnDestroy();

		/// 스태틱의 타이틀을 입력합니다.
		virtual VOID	SetText(const WCHAR* buff);
		const Nave::String GetText();

		/// 오브젝트 고유 스트링
	    virtual const char* ToString() { return "UIStatic"; }

		/// 오브젝트를 화면에 그린다.
		virtual VOID	OnRender();

		VOID	SetAlign(DWORD dwAlign);

	private:
		NFText	m_Title;

		DWORD	m_dwAlign;

		Nave::String	m_szText;

	};

}