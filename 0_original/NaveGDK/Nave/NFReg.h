/** 
 *  @file		NFReg.h
 *  @brief		������Ʈ�� ���� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class			NFReg  
	 *  @brief			������Ʈ�� ����Ŭ����
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
		/// NFReg ������
		NFReg();
		/// NFReg �Ҹ���
		~NFReg();

	public:

		/**
		 * @brief	������Ʈ���� �����մϴ�.
		 * @param rootKey	��ƮŰ
		 * @param subKey	����Ű
		 * @return	��������
		 */
		BOOL Open(HKEY rootKey, LPCWSTR subKey);

		/**
		 * @brief	������Ʈ���� �����մϴ�.
		 * @return  ��������
		 */
		BOOL Close();

		/**
		 * @brief	���ο� Ű�� �����մϴ�.
		 * @param rootKey ��ƮŰ
		 * @param subKey  ����Ű
		 * @return	��������
		 */
		BOOL CreateKey(HKEY rootKey, LPCWSTR subKey);

		/**
		 * @brief	Ű�� �����մϴ�.
		 * @param rootKey ��ƮŰ
		 * @param subKey  ����Ű
		 * @return  ��������
		 */
		BOOL DeleteKey(HKEY rootKey, LPCWSTR subKey);

		/**
		 * @brief	���������� Ű�� ���ڿ�(REG_SZ)���� �߰��մϴ�.
		 * @param valueName Ű�̸�
		 * @param value		�߰�����
		 * @return  ��������
		 */
		BOOL SetValue(LPCWSTR valueName, LPCWSTR value);

		/**
		 * @brief	���������� Ű�� DWORD���� �߰��մϴ�.
		 * @param valueName Ű�̸�
		 * @param value		�߰�����
		 * @return  ��������
		 */
		BOOL SetValue(LPCWSTR valueName, DWORD value);

		/**
		 * @brief	���������� Ű�� ��Ƽ���ڿ�(REG_MULTI_SZ)���� �߰��մϴ�.
		 * @param valueName Ű�̸�
		 * @param value		�߰�����
		 * @param byteLength ����
		 * @return  ��������
		 */
		BOOL SetValueForMultiSz(LPCWSTR valueName, LPCWSTR value, DWORD byteLength);

		/**
		 * @brief	Ű�� ���� ���ɴϴ�.
		 * @param valueName		Ű�̸�
		 * @param value			���庯��
		 * @param bufferLength	��������
		 * @return  ��������
		 */
		BOOL GetValue(LPCWSTR valueName, LPWSTR value, LPDWORD bufferLength);

		/**
		 * @brief	Ű�� ���� ���ɴϴ�.
		 * @param valueName		Ű�̸�
		 * @param value			���庯��
		 * @return	��������
		 */
		BOOL GetValue(LPCWSTR valueName, LPDWORD value);

		/**
		 * @brief	�ش� Ű�� �����ճ���.
		 * @param valueName ����Ű�̸�
		 * @return  ��������
		 */
		BOOL DeleteValue(LPCWSTR valueName);

	private:
		/// ������Ʈ���� RootKey
		HKEY	m_hRootKey;
		/// ������Ʈ�� Open ����
		BOOL	m_bIsOpened;
	};

} 