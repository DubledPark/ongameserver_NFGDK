/** 
 *  @file		NFConfig.h
 *  @brief		Config.xml 을 관리하기 위한 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include "Nave.h"
#include "tinyxml.h"

namespace Nave { 

	/** 
	 *  @class        NFConfig
	 *  @brief        XML로 구성된 Config 파일을 관리합니다. 
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
		 * @brief	Config에서 값을 얻어옵니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		리턴값
		 */
		void	Get(WCHAR* category, WCHAR* name, WCHAR* value);
		/**
		 * @brief	Config에서 값을 얻어옵니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		리턴값
		 */
		void	Get(WCHAR* category, WCHAR* name, SHORT& value);
		/**
		 * @brief	Config에서 값을 얻어옵니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		리턴값
		 */
		void	Get(WCHAR* category, WCHAR* name, USHORT& value);
		/**
		 * @brief	Config에서 값을 얻어옵니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		리턴값
		 */
		void	Get(WCHAR* category, WCHAR* name, INT& value);
		/**
		 * @brief	Config에서 값을 얻어옵니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		리턴값
		 */
		void	Get(WCHAR* category, WCHAR* name, UINT& value);
		/**
		 * @brief	Config에서 값을 얻어옵니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		리턴값
		 */
		void	Get(WCHAR* category, WCHAR* name, bool& value);
		/**
		 * @brief	Config에서 값을 얻어옵니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		리턴값
		 */
		void	Get(WCHAR* category, WCHAR* name, DOUBLE& value);
		/**
		 * @brief	Config에서 값을 얻어옵니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		리턴값
		 */
		void	Get(WCHAR* category, WCHAR* name, FLOAT& value);

		/**
		 * @brief	Config에서 값을 저장합니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		저장 값
		 */
		void	Set(WCHAR* category, WCHAR* name, WCHAR* value);
		/**
		 * @brief	Config에서 값을 저장합니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		저장 값
		 */
		void	Set(WCHAR* category, WCHAR* name, SHORT value);
		/**
		 * @brief	Config에서 값을 저장합니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		저장 값
		 */
		void	Set(WCHAR* category, WCHAR* name, USHORT value);
		/**
		 * @brief	Config에서 값을 저장합니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		저장 값
		 */
		void	Set(WCHAR* category, WCHAR* name, INT value);
		/**
		 * @brief	Config에서 값을 저장합니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		저장 값
		 */
		void	Set(WCHAR* category, WCHAR* name, UINT value);
		/**
		 * @brief	Config에서 값을 저장합니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		저장 값
		 */
		void	Set(WCHAR* category, WCHAR* name, bool value);
		/**
		 * @brief	Config에서 값을 저장합니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		저장 값
		 */
		void	Set(WCHAR* category, WCHAR* name, DOUBLE value);
		/**
		 * @brief	Config에서 값을 저장합니다.
		 * @param category	카테고리 이름
		 * @param name		이름
		 * @param value		저장 값
		 */
		void	Set(WCHAR* category, WCHAR* name, FLOAT value);

		/**
		 @brief Config를 저장합니다.
		 @return	결과
		 @param		lpszFilename	xml파일명
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
		 @brief Config를 로드합니다.
		 @return	결과
		 @param		lpszFilename	xml파일명
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

		/// Config를 종료합니다.
		void	Close();

	private:
		/**
		 * @brief	Config를 파싱합니다.
		 * @param lpszFilename 파싱할 xml파일명
		 * @return 성공여부
		 */
		BOOL	Parse_XML(WCHAR* lpszFilename);

	private:
		/// XML Document
		TiXmlDocument*	m_pXmlDoc;
		/// XML Root Node
		TiXmlNode*		m_pRoot;
	};

}

