/** 
 *  @file		UISprite.h
 *  @brief		ȭ�鿡 �̹����� ������ �ɱ����� ��ü
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NFSprite.h>
#include "NFImage.h"

namespace Nave2D { 

	/** 
	 *  @class        UISprite
	 *  @brief        ȭ�鿡 �̹����� ������ �ɱ����� ��ü
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UISprite : public NaveFramework::NFSprite
	{
	public:
		/// UISprite ������
		UISprite(void);
		/// UISprite �Ҹ���
		virtual ~UISprite(void);

	public:
		/**
		* @brief	�̹����� �����մϴ�.
		* @param pImage �̹��� ������
		*/
		VOID				SetImageInfo(LPSPRITEINFO pImage);
	};

}