/** 
 *  @file		UICheckBox.h
 *  @brief		üũ �ڽ� ��Ʈ��
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIButton.h"

namespace Nave2D { 

	/** 
	 *  @class			UICheckBox
	 *  @brief			üũ�ڽ��� �����ϰ� �����ϴ� Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UICheckBox : public UIButton
	{
	public:
		/// UICheckBox ������
		UICheckBox(void);
		/// UICheckBox �Ҹ���
		virtual ~UICheckBox(void);

	public:
		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY);
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY);
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY);

		/// ������Ʈ ���� ��Ʈ��
	    virtual const char* ToString() { return "UICheckBox"; }

		/// ��Ʈ�� Rect�� ������Ʈ �մϴ�.
		virtual VOID	UpdateControlRect();

	private:
		/// üũ ��Ʈ���� Ǫ�� ����
		BOOL	m_bPushFlag;

	};

}