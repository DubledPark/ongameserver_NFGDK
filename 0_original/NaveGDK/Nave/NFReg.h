/** 
 *  @file		NFReg.h
 *  @brief		레지스트리 관리 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class			NFReg  
	 *  @brief			레지스트리 관리클래스
	 *  @remarks      NFReg reg;									\r\n
	 *                reg.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Preflib");\r\n
	 *                reg.GetValue(L"Last Counter", &Index);		\r\n
	 *                reg.Close()									\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFReg
	{
	public:
		/// NFReg 생성자
		NFReg();
		/// NFReg 소멸자
		~NFReg();

	public:

		/**
		 * @brief	레지스트리를 오픈합니다.
		 * @param rootKey	루트키
		 * @param subKey	서브키
		 * @return	성공여부
		 */
		BOOL Open(HKEY rootKey, LPCWSTR subKey);

		/**
		 * @brief	레지스트리를 종료합니다.
		 * @return  성공여부
		 */
		BOOL Close();

		/**
		 * @brief	새로운 키를 생성합니다.
		 * @param rootKey 루트키
		 * @param subKey  생성키
		 * @return	성공여부
		 */
		BOOL CreateKey(HKEY rootKey, LPCWSTR subKey);

		/**
		 * @brief	키를 삭제합니다.
		 * @param rootKey 루트키
		 * @param subKey  삭제키
		 * @return  성공여부
		 */
		BOOL DeleteKey(HKEY rootKey, LPCWSTR subKey);

		/**
		 * @brief	현재지정된 키에 문자열(REG_SZ)값을 추가합니다.
		 * @param valueName 키이름
		 * @param value		추가내용
		 * @return  성공여부
		 */
		BOOL SetValue(LPCWSTR valueName, LPCWSTR value);

		/**
		 * @brief	현재지정된 키에 DWORD값을 추가합니다.
		 * @param valueName 키이름
		 * @param value		추가내용
		 * @return  성공여부
		 */
		BOOL SetValue(LPCWSTR valueName, DWORD value);

		/**
		 * @brief	현재지정된 키에 멀티문자열(REG_MULTI_SZ)값을 추가합니다.
		 * @param valueName 키이름
		 * @param value		추가내용
		 * @param byteLength 길이
		 * @return  성공여부
		 */
		BOOL SetValueForMultiSz(LPCWSTR valueName, LPCWSTR value, DWORD byteLength);

		/**
		 * @brief	키의 값을 얻어옵니다.
		 * @param valueName		키이름
		 * @param value			저장변수
		 * @param bufferLength	변수길이
		 * @return  성공여부
		 */
		BOOL GetValue(LPCWSTR valueName, LPWSTR value, LPDWORD bufferLength);

		/**
		 * @brief	키의 값을 얻어옵니다.
		 * @param valueName		키이름
		 * @param value			저장변수
		 * @return	성공여부
		 */
		BOOL GetValue(LPCWSTR valueName, LPDWORD value);

		/**
		 * @brief	해당 키를 삭제합나다.
		 * @param valueName 삭제키이름
		 * @return  성공여부
		 */
		BOOL DeleteValue(LPCWSTR valueName);

	private:
		/// 레지스트리의 RootKey
		HKEY	m_hRootKey;
		/// 레지스트리 Open 여부
		BOOL	m_bIsOpened;
	};

} 