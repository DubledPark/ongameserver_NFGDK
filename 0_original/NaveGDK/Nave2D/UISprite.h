/** 
 *  @file		UISprite.h
 *  @brief		화면에 이미지를 렌더링 걸기위한 객체
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NFSprite.h>
#include "NFImage.h"

namespace Nave2D { 

	/** 
	 *  @class        UISprite
	 *  @brief        화면에 이미지를 렌더링 걸기위한 객체
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UISprite : public NaveFramework::NFSprite
	{
	public:
		/// UISprite 생성자
		UISprite(void);
		/// UISprite 소멸자
		virtual ~UISprite(void);

	public:
		/**
		* @brief	이미지를 연결합니다.
		* @param pImage 이미지 포인터
		*/
		VOID				SetImageInfo(LPSPRITEINFO pImage);
	};

}