/** 
 *  @file		UICheckBox.h
 *  @brief		체크 박스 컨트롤
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIButton.h"

namespace Nave2D { 

	/** 
	 *  @class			UICheckBox
	 *  @brief			체크박스를 생성하고 관리하는 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UICheckBox : public UIButton
	{
	public:
		/// UICheckBox 생성자
		UICheckBox(void);
		/// UICheckBox 소멸자
		virtual ~UICheckBox(void);

	public:
		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY);
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY);
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY);

		/// 오브젝트 고유 스트링
	    virtual const char* ToString() { return "UICheckBox"; }

		/// 컨트롤 Rect를 업데이트 합니다.
		virtual VOID	UpdateControlRect();

	private:
		/// 체크 컨트롤의 푸시 상태
		BOOL	m_bPushFlag;

	};

}