/** 
 *  @file		UI2DEffect.h
 *  @brief		NFSprite를 이용한 2D Effect 객체
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NFSprite.h>
#include "NFImage.h"

namespace Nave2D { 

	/** 
	 *  @class        UI2DEffect
	 *  @brief        NFSprite를 이용한 2D Effect 객체
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UI2DEffect : public NaveFramework::NFSprite
	{
	public:
		/// UI2DEffect 생성자
		UI2DEffect(void);
		/// UI2DEffect 소멸자
		virtual ~UI2DEffect(void);

	public:
		/**
		 * @brief	이미지를 연결합니다.
		 * @param pImage 이미지 포인터
		 */
		VOID				SetImageInfo(LPSPRITEINFO pImage);

		/**
		 * @brief	사이즈를 설정합니다.
		 * @param fWidth 가로크기
		 * @param fHeight 세로크기
		 */
		VOID				SetSize(float fWidth, float fHeight);

		/**
		 * @brief	출력 메트릭스를 얻어옵니다.
		 * @param mat 출력 메트릭스 
		 * @param fX  출력 X
		 * @param fY  츨력 Y
		 */
		VOID				GetMatrix(D3DXMATRIX& mat, float fX = -1, float fY = -1);

		/**
		 * @brief	회전 중심을 설정합니다.
		 * @param rotX 회전점 X
		 * @param rotY 회전점 Y
		 */
		inline	void		SetRotationCenter( float rotX, float rotY )	{	m_RotCenter.x = rotX ;	m_RotCenter.y = rotY ;	}
	};

}