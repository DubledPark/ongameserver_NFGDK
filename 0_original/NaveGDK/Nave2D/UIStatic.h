/** 
 *  @file		UIStatic.h
 *  @brief		����ƽ ��Ʈ��
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIControl.h"
#include "NFText.h"

namespace Nave2D { 

	/** 
	 *  @class        UIStatic
	 *  @brief        ����ƽ�� �����ϰ� �����ϴ� Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIStatic : public UIControl
	{
	public:
		/// UIStatic ������
		UIStatic(void);
		/// UIStatic �Ҹ���
		virtual ~UIStatic(void);

	public:
		/**
		 * @brief	����ƽ�� �����մϴ�.
		 * @param pParent	�θ�����Ʈ
		 * @param fX		����X
		 * @param fY		����Y
		 * @param fWidth	����ũ��
		 * @param fHeight	����ũ��
		 * @param nID		��ü ���̵�
		 * @warning	fX,fY�� ���� �θ��� ������������ ������ǥ�̴�.
		 * @return	��������
		 */
		virtual BOOL OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID);
		/// ����ƽ�� �����Ѵ�.
		virtual BOOL OnDestroy();

		/// ����ƽ�� Ÿ��Ʋ�� �Է��մϴ�.
		virtual VOID	SetText(const WCHAR* buff);
		const Nave::String GetText();

		/// ������Ʈ ���� ��Ʈ��
	    virtual const char* ToString() { return "UIStatic"; }

		/// ������Ʈ�� ȭ�鿡 �׸���.
		virtual VOID	OnRender();

		VOID	SetAlign(DWORD dwAlign);

	private:
		NFText	m_Title;

		DWORD	m_dwAlign;

		Nave::String	m_szText;

	};

}