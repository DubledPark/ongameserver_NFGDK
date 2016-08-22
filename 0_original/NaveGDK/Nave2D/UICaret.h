/** 
 *  @file		UICaret.h
 *  @brief		Ŀ���� �����Ѵ�.
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UISprite.h"

namespace Nave2D { 

	/** 
	 *  @class        UICaret
	 *  @brief        NFEditBox���� Ŀ���� �����Ѵ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UICaret : public UISprite
	{
	public:
		/// UICaret ������
		UICaret(void);
		/// UICaret �Ҹ���
		virtual ~UICaret(void);

	public:
		/// ĳ���� �����մϴ�.
		virtual VOID OnCreate();
		/// ĳ���� �����մϴ�.
		virtual VOID OnDestroy();
		/// ĳ���� Ȱ��ȭ ����
		virtual	BOOL IsVisible();

		/// ĳ���� �÷��� �����մϴ�.
		VOID SetCaretColor(COLORREF Color);
		/// ĳ���� ����� �����մϴ�.
		VOID SetCaretSize(float Width, float Height);

		/// ĳ���� ���ø��� �����մϴ�.
		inline void InitCaretTime() { m_CaretTick = timeGetTime(); m_CaretFlag = TRUE; }
	private:
		/// ĳ�� �÷���
		BOOL    m_CaretFlag;
		/// ĳ�� ���� Ÿ�̸�
		DWORD	m_CaretTick;
		/// ĳ�� �ؽ��� �ε���
		DWORD	m_dwTexture;
	};

}
