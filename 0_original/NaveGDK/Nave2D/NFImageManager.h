/** 
 *  @file		NFImageManager.h
 *  @brief		NFImage�� �����ϴ� �޴��� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <map>
#include <Nave/NFSingleton.h>
#include <Nave/NFSync.h>
#include "NFImage.h"

namespace Nave2D {

	/** 
	 *  @class        NFImageManager
	 *  @brief        Image�� �ε��ϰ� �����Ѵ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFImageManager : public Nave::NFSingleton<NFImageManager>
	{
	public:
		/// NFImageManager ������
		NFImageManager(void);
		/// NFImageManager �Ҹ���
		~NFImageManager(void);

	public:
		/// ��� ��ü�� �����մϴ�.
		VOID		ReleaseAll();
		/**
		 * @brief	�̹����� �˻��մϴ�.
		 * @param dwID	�̹������̵�
		 * @return	�̹���������
		 */
		NFImage*	FindImage(DWORD dwID);

		/**
		 * @brief	���ο� �̹����� �ε��մϴ�.
		 * @param dwID	�̹������̵�
		 * @param ScriptFile	��ũ��Ʈ �̸�
		 * @param ImageFile		�̹��� �̸�
		 * @return	�̹���������
		 */
		NFImage*	AddFromFile(DWORD dwID, const WCHAR* ImageFile);
		/**
		 * @brief	���ο� �̹����� �ε��մϴ�.
		 * @param dwID			�̹������̵�
		 * @param TextureID		�ؽ����νľ��̵�
		 * @param ScriptPoint	��ũ��Ʈ ������
		 * @param ScriptLen		��ũ��Ʈ ����
		 * @param FilePoint		�ؽ��� ������
		 * @param FileLen		�ؽ��� ����
		 * @return  �̹���������
		 */
		NFImage*	AddFromMemory(DWORD dwID, const WCHAR* TextureID, char* FilePoint, DWORD FileLen);

	private:
		/**
		 * @brief	�̹��� ��ü�� �����մϴ�.
		 * @param dwID	�̹��� ���̵�
		 * @return  �̹���������
		 */
		NFImage*		CreateImage(DWORD dwID);

	private:
		/// �̹��� ��
		std::map<DWORD, NFImage*>					m_ImageMap;
		/// �̹��� ���ͷ�����
		typedef std::map<DWORD, NFImage*>::iterator	ImageIter;

		/// Sync ��ü
		Nave::NFSync								m_Lock;

	};

}