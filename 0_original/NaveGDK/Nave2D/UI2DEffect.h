/** 
 *  @file		UI2DEffect.h
 *  @brief		NFSprite�� �̿��� 2D Effect ��ü
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NFSprite.h>
#include "NFImage.h"

namespace Nave2D { 

	/** 
	 *  @class        UI2DEffect
	 *  @brief        NFSprite�� �̿��� 2D Effect ��ü
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UI2DEffect : public NaveFramework::NFSprite
	{
	public:
		/// UI2DEffect ������
		UI2DEffect(void);
		/// UI2DEffect �Ҹ���
		virtual ~UI2DEffect(void);

	public:
		/**
		 * @brief	�̹����� �����մϴ�.
		 * @param pImage �̹��� ������
		 */
		VOID				SetImageInfo(LPSPRITEINFO pImage);

		/**
		 * @brief	����� �����մϴ�.
		 * @param fWidth ����ũ��
		 * @param fHeight ����ũ��
		 */
		VOID				SetSize(float fWidth, float fHeight);

		/**
		 * @brief	��� ��Ʈ������ ���ɴϴ�.
		 * @param mat ��� ��Ʈ���� 
		 * @param fX  ��� X
		 * @param fY  ���� Y
		 */
		VOID				GetMatrix(D3DXMATRIX& mat, float fX = -1, float fY = -1);

		/**
		 * @brief	ȸ�� �߽��� �����մϴ�.
		 * @param rotX ȸ���� X
		 * @param rotY ȸ���� Y
		 */
		inline	void		SetRotationCenter( float rotX, float rotY )	{	m_RotCenter.x = rotX ;	m_RotCenter.y = rotY ;	}
	};

}