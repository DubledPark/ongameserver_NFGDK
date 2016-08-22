/** 
 *  @file		NFConfig.h
 *  @brief		Config.xml �� �����ϱ� ���� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include "Nave.h"
#include "tinyxml.h"

namespace Nave { 

	/** 
	 *  @class        NFConfig
	 *  @brief        XML�� ������ Config ������ �����մϴ�. 
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFConfig
	{
	public:
		NFConfig();
		~NFConfig();

	public:
		/**
		 * @brief	Config���� ���� ���ɴϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���ϰ�
		 */
		void	Get(WCHAR* category, WCHAR* name, WCHAR* value);
		/**
		 * @brief	Config���� ���� ���ɴϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���ϰ�
		 */
		void	Get(WCHAR* category, WCHAR* name, SHORT& value);
		/**
		 * @brief	Config���� ���� ���ɴϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���ϰ�
		 */
		void	Get(WCHAR* category, WCHAR* name, USHORT& value);
		/**
		 * @brief	Config���� ���� ���ɴϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���ϰ�
		 */
		void	Get(WCHAR* category, WCHAR* name, INT& value);
		/**
		 * @brief	Config���� ���� ���ɴϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���ϰ�
		 */
		void	Get(WCHAR* category, WCHAR* name, UINT& value);
		/**
		 * @brief	Config���� ���� ���ɴϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���ϰ�
		 */
		void	Get(WCHAR* category, WCHAR* name, bool& value);
		/**
		 * @brief	Config���� ���� ���ɴϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���ϰ�
		 */
		void	Get(WCHAR* category, WCHAR* name, DOUBLE& value);
		/**
		 * @brief	Config���� ���� ���ɴϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���ϰ�
		 */
		void	Get(WCHAR* category, WCHAR* name, FLOAT& value);

		/**
		 * @brief	Config���� ���� �����մϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���� ��
		 */
		void	Set(WCHAR* category, WCHAR* name, WCHAR* value);
		/**
		 * @brief	Config���� ���� �����մϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���� ��
		 */
		void	Set(WCHAR* category, WCHAR* name, SHORT value);
		/**
		 * @brief	Config���� ���� �����մϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���� ��
		 */
		void	Set(WCHAR* category, WCHAR* name, USHORT value);
		/**
		 * @brief	Config���� ���� �����մϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���� ��
		 */
		void	Set(WCHAR* category, WCHAR* name, INT value);
		/**
		 * @brief	Config���� ���� �����մϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���� ��
		 */
		void	Set(WCHAR* category, WCHAR* name, UINT value);
		/**
		 * @brief	Config���� ���� �����մϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���� ��
		 */
		void	Set(WCHAR* category, WCHAR* name, bool value);
		/**
		 * @brief	Config���� ���� �����մϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���� ��
		 */
		void	Set(WCHAR* category, WCHAR* name, DOUBLE value);
		/**
		 * @brief	Config���� ���� �����մϴ�.
		 * @param category	ī�װ� �̸�
		 * @param name		�̸�
		 * @param value		���� ��
		 */
		void	Set(WCHAR* category, WCHAR* name, FLOAT value);

		/**
		 @brief Config�� �����մϴ�.
		 @return	���
		 @param		lpszFilename	xml���ϸ�
		 @warning	
		*/
		BOOL	Save(WCHAR* lpszFilename )
		{
			if(m_pXmlDoc == NULL)
			{
				m_pXmlDoc = new TiXmlDocument();
				m_pXmlDoc->InsertDeclaration(L"1.0", L"");
			}

			BOOL okey = m_pXmlDoc->SaveFile(lpszFilename);
			return okey;
		}

		/**
		 @brief Config�� �ε��մϴ�.
		 @return	���
		 @param		lpszFilename	xml���ϸ�
		 @warning	
		*/
		BOOL	Open(WCHAR* lpszFilename = L"Config.xml")
		{
			if(m_pXmlDoc == NULL)
			{
				m_pXmlDoc = new TiXmlDocument();
			}

			return Parse_XML(lpszFilename);
		}

		/// Config�� �����մϴ�.
		void	Close();

	private:
		/**
		 * @brief	Config�� �Ľ��մϴ�.
		 * @param lpszFilename �Ľ��� xml���ϸ�
		 * @return ��������
		 */
		BOOL	Parse_XML(WCHAR* lpszFilename);

	private:
		/// XML Document
		TiXmlDocument*	m_pXmlDoc;
		/// XML Root Node
		TiXmlNode*		m_pRoot;
	};

}

