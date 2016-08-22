/** 
 *  @file		NFIni.h
 *  @brief		INI ���� ������
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class        NFIni
	 *  @brief        INI������ ����ϱ� ���� ���� ���� Ŭ����
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
		/// NFIni ������
		NFIni();
		/// NFIni �Ҹ���
		~NFIni();

	public:
		/**
		 * @brief		INI������ �����մϴ�.
		 * @param fileName	���ϸ�
		 * @return		��������
		 */
		BOOL	Open(LPCWSTR fileName);

		/**
		 * @brief		INI������ �����մϴ�.
		 * @return		��������
		 */
		BOOL	Close();

		/**
		 * @brief		INI���Ͽ� ���� �ֽ��ϴ�.
		 * @param keyName		Key �̸�
		 * @param valueName		Value �̸�
		 * @param value			��
		 * @return		��������
		 */
		BOOL	SetValue(LPCWSTR keyName, LPCWSTR valueName, LPCWSTR value);
		/**
		 * @brief		INI���Ͽ� ���� �ֽ��ϴ�.
		 * @param keyName		Key �̸�
		 * @param valueName		Value �̸�
		 * @param value			��
		 * @return		��������
		 */
		BOOL	SetValue(LPCWSTR keyName, LPCWSTR valueName, DWORD value);
		/**
		 * @brief		INI���Ͽ� ���� �ֽ��ϴ�.
		 * @param keyName		Key �̸�
		 * @param valueName		Value �̸�
		 * @param value			��
		 * @return		��������
		 */
		BOOL	SetValue(LPCWSTR keyName, LPCWSTR valueName, FLOAT value);

		/**
		 * @brief		INI���Ͽ� ���� �����ɴϴ�.
		 * @param keyName		Key �̸�
		 * @param valueName		Value �̸�
		 * @param value			���� ����
		 * @param bufferLength	�۹��� ����
		 * @return		��������
		 */
		BOOL	GetValue(LPCWSTR keyName, LPCWSTR valueName, LPWSTR value, DWORD bufferLength);
		/**
		 * @brief		INI���Ͽ� ���� �����ɴϴ�.
		 * @param keyName		Key �̸�
		 * @param valueName		Value �̸�
		 * @param value			���� ����
		 * @return		��������
		 */
		BOOL	GetValue(LPCWSTR keyName, LPCWSTR valueName, LPDWORD value);
		/**
		 * @brief		INI���Ͽ� ���� �����ɴϴ�.
		 * @param keyName		Key �̸�
		 * @param valueName		Value �̸�
		 * @param value			���� ����
		 * @return		��������
		 */
		BOOL	GetValue(LPCWSTR keyName, LPCWSTR valueName, FLOAT *value);

	private:
		/// INI ���ϸ�
		WCHAR	m_szFileName[MAX_PATH];
	};

} 