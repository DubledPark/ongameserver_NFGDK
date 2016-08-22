/** 
 *  @file		NFIni.h
 *  @brief		INI 파일 관리자
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class        NFIni
	 *  @brief        INI파일을 사용하기 쉽게 만든 관리 클래스
	 *  @remarks      NFIni ini;										\r\n
	 *                ini.Open(L"c:\\Init.ini");						\r\n
	 *                ini.SetValue(L"SERVER CONFIG", L"PORT", dwPort);	\r\n
	 *                ini.Close();										\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFIni
	{
	public:
		/// NFIni 생성자
		NFIni();
		/// NFIni 소멸자
		~NFIni();

	public:
		/**
		 * @brief		INI파일을 오픈합니다.
		 * @param fileName	파일명
		 * @return		성공여부
		 */
		BOOL	Open(LPCWSTR fileName);

		/**
		 * @brief		INI파일을 종료합니다.
		 * @return		성공여부
		 */
		BOOL	Close();

		/**
		 * @brief		INI파일에 값을 넣습니다.
		 * @param keyName		Key 이름
		 * @param valueName		Value 이름
		 * @param value			값
		 * @return		성공여부
		 */
		BOOL	SetValue(LPCWSTR keyName, LPCWSTR valueName, LPCWSTR value);
		/**
		 * @brief		INI파일에 값을 넣습니다.
		 * @param keyName		Key 이름
		 * @param valueName		Value 이름
		 * @param value			값
		 * @return		성공여부
		 */
		BOOL	SetValue(LPCWSTR keyName, LPCWSTR valueName, DWORD value);
		/**
		 * @brief		INI파일에 값을 넣습니다.
		 * @param keyName		Key 이름
		 * @param valueName		Value 이름
		 * @param value			값
		 * @return		성공여부
		 */
		BOOL	SetValue(LPCWSTR keyName, LPCWSTR valueName, FLOAT value);

		/**
		 * @brief		INI파일에 값을 가져옵니다.
		 * @param keyName		Key 이름
		 * @param valueName		Value 이름
		 * @param value			저장 버퍼
		 * @param bufferLength	퍼버의 길이
		 * @return		성공여부
		 */
		BOOL	GetValue(LPCWSTR keyName, LPCWSTR valueName, LPWSTR value, DWORD bufferLength);
		/**
		 * @brief		INI파일에 값을 가져옵니다.
		 * @param keyName		Key 이름
		 * @param valueName		Value 이름
		 * @param value			저장 버퍼
		 * @return		성공여부
		 */
		BOOL	GetValue(LPCWSTR keyName, LPCWSTR valueName, LPDWORD value);
		/**
		 * @brief		INI파일에 값을 가져옵니다.
		 * @param keyName		Key 이름
		 * @param valueName		Value 이름
		 * @param value			저장 버퍼
		 * @return		성공여부
		 */
		BOOL	GetValue(LPCWSTR keyName, LPCWSTR valueName, FLOAT *value);

	private:
		/// INI 파일명
		WCHAR	m_szFileName[MAX_PATH];
	};

} 