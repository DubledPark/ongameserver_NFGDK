/** 
 *  @file		NFFontManager.h
 *  @brief		��Ʈ�� �ε��ϰ� �����Ѵ�.
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <gdiplus.h>
#pragma comment(lib, "GdiPlus")

#include <vector>

namespace NaveFramework { 

	/** 
	 *  @class        NFFontManager
	 *  @brief        ��Ʈ �޴���
	 *  @remarks      
	 *                NFFontManager::GetInstance().AddFont(L".\\Fonts\\MD����üM.ttf");	\r\n
	 *																					\r\n
	 *                // �޸𸮿��� �ε���											\r\n
	 *                WCHAR strBuff[512];												\r\n
	 *                wcscpy(strBuff, L".\\Fonts\\FB�۲�M.ttf");						\r\n
	 *																					\r\n
	 *                Nave::NFFilePtrW fp(strBuff, L"rb");								\r\n
	 *																					\r\n
	 *                if(fp)															\r\n
	 *                {																	\r\n
	 *					fseek(fp, 0, SEEK_END);											\r\n
	 *																					\r\n
	 *					fpos_t pos;														\r\n
	 *                	fgetpos(fp, &pos);												\r\n
	 *																					\r\n
	 *              	fseek(fp, 0, SEEK_SET);											\r\n
	 *              	char* strData = new char[(unsigned int)pos];					\r\n
	 *																					\r\n
	 *              	fread(strData, (size_t)pos, 1, fp);								\r\n
	 *																					\r\n
	 *              	// ���ϵƴ�.. Count���� 2�� ���ϵ���.							\r\n
	 *              	NFFontManager::GetInstance().AddFontMem(strData, (DWORD)pos);	\r\n
	 *																					\r\n
	 *              	_DELETE_ARRAY(strData);											\r\n
	 *                }																	\r\n
	 *																					\r\n
	 *																					\r\n
	 *                // ��Ʈ�� ������Ʈ �Ͽ� ����Ҽ� �ֵ��� ó���Ѵ�.					\r\n
	 *                NFFontManager::GetInstance().UpdateFontFamily();					\r\n
	 *																					\r\n
	 *                // �ش� ��Ʈ�� �����Ѵ�. 0�� �ε����� ��Ʈ�� 50, Bold�� �����Ѵ�.	\r\n
	 *                NFFontManager::GetInstance().CreateFont(50, 0, FontStyleBold);	\r\n
	 *																					\r\n
	 *                // �̿Ͱ��� ��Ʈ�� ����Ѵ�.										\r\n
	 *                m_Frame.SetFont(NFFontManager::GetInstance().GetHFont(0));		\r\n
	 *																					\r\n
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFFontManager : public Nave::NFSingleton<NFFontManager>
	{
	public:
		/// NFFontManager ������
		NFFontManager(void);
		/// NFFontManager �Ҹ���
		~NFFontManager(void);

	public:
		/// ��� ��Ʈ�� �����Ѵ�.
		void								RemoveAllFonts();

		/**
		 * @brief	�⺻ ��Ʈ�� �����մϴ�.
		 * @param strFontName ��Ʈ�̸�
		 */
		void								SetDefaultFont(const WCHAR* strFontName);
		/**
		 * @brief	���ο� ��Ʈ�� �߰��մϴ�.
		 * @param strFont ��Ʈ�̸�
		 */
		void								AddFont(const WCHAR* strFont);
		/**
		 * @brief	���ο� ��Ʈ�� �߰��մϴ�.
		 * @param pMem		�޸�������
		 * @param dwSize	�޸� ũ��
		 */
		void								AddFontMem(PVOID pMem, DWORD dwSize);

		/// �����쿡 ��Ʈ�� ������Ʈ �մϴ�.
		void								UpdateFontFamily();

		/**
		 * @brief	���ο� ��Ʈ�� �����մϴ�.
		 * @param FontSize		��Ʈũ��
		 * @param iFontType		��Ʈ Ÿ��
		 * @param FontStyle		��Ʈ ��Ÿ��
		 * @return	������ ��Ʈ��ȣ
		 */
		int									CreateFont(Gdiplus::REAL FontSize, int iFontType, INT FontStyle);

		/**
		 * @brief	��Ʈ�� �˻��մϴ�.
		 * @param strFontName	��Ʈ�̸�
		 * @return	��Ʈ �ε���
		 */
		int									FindFont(const WCHAR* strFontName);

		/**
		 * @brief	��Ʈ �̸��� �����ɴϴ�.
		 * @param iIndex	��Ʈ �ε���
		 * @return  ��Ʈ �̸�
		 */
		Nave::String						GetFontName(int iIndex = -1);
		/**
		 * @brief	FontFamily�� ���ɴϴ�.
		 * @param iIndex	��Ʈ �ε���
		 * @return	FontFamily��
		 */
		Gdiplus::FontFamily*				GetFontFamily(int iIndex = -1);
		/**
		 * @brief	Font�� ���ɴϴ�.
		 * @param iIndex	��Ʈ �ε���
		 * @return  Font��
		 */
		Gdiplus::Font*						GetFont(int iIndex = -1);
		/**
		 * @brief	HFONT�� ���ɴϴ�.
		 * @param iIndex	��Ʈ �ε���
		 * @return	HFONT��
		 */
		HFONT								GetHFont(int iIndex = -1);

	private:
		/**
		 * @brief	���ο� ��Ʈ�� �����մϴ�.
		 * @param FontSize		��Ʈũ��
		 * @param lpszFontName	��Ʈ�̸�
		 * @param FontWeight	��Ʈ����
		 * @return	������ ��Ʈ
		 */
		HFONT								CreateFontEx(INT FontSize, LPCWSTR lpszFontName, INT FontWeight = 0);

	private:
		/// GDI+ ��Ʈ �ʱ�ȭ ��ü
		ULONG_PTR							m_gdiplusToken;
		/// GDI+ ��Ʈ �ʱ�ȭ ��ü
		Gdiplus::PrivateFontCollection*		m_gdiFont;

		/// �⺻��Ʈ ����
		Gdiplus::FontFamily*				m_pDefaultFontFamily;
		/// ��Ʈ FontFamily ����
		Gdiplus::FontFamily*				m_pFontFamily;
		/// ��Ʈ FontFamily ����
		INT									m_iFound;

		/// �⺻��Ʈ
		HFONT								m_hDefaultFont;
		/// �⺻��Ʈ
		Gdiplus::Font*						m_pDefaultFont;

		/// ������ GDI+��Ʈ
		std::vector<Gdiplus::Font*>			m_vetFonts;

		/// ������ GDI��Ʈ
		std::vector<HFONT>					m_vetHFonts;

		/// ��Ʈ �̸�����Ʈ
		std::vector<Nave::String>			m_vetFontFile;
	};

}