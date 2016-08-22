/** 
 *  @file		NFFontManager.h
 *  @brief		폰트를 로드하고 관리한다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <gdiplus.h>
#pragma comment(lib, "GdiPlus")

#include <vector>

namespace NaveFramework { 

	/** 
	 *  @class        NFFontManager
	 *  @brief        폰트 메니져
	 *  @remarks      
	 *                NFFontManager::GetInstance().AddFont(L".\\Fonts\\MD으뜸체M.ttf");	\r\n
	 *																					\r\n
	 *                // 메모리에서 로드방법											\r\n
	 *                WCHAR strBuff[512];												\r\n
	 *                wcscpy(strBuff, L".\\Fonts\\FB글꽃M.ttf");						\r\n
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
	 *              	// 리턴됐다.. Count에서 2가 리턴됐음.							\r\n
	 *              	NFFontManager::GetInstance().AddFontMem(strData, (DWORD)pos);	\r\n
	 *																					\r\n
	 *              	_DELETE_ARRAY(strData);											\r\n
	 *                }																	\r\n
	 *																					\r\n
	 *																					\r\n
	 *                // 폰트를 업데이트 하여 사용할수 있도록 처리한다.					\r\n
	 *                NFFontManager::GetInstance().UpdateFontFamily();					\r\n
	 *																					\r\n
	 *                // 해당 폰트를 생성한다. 0번 인덱스의 폰트를 50, Bold로 생성한다.	\r\n
	 *                NFFontManager::GetInstance().CreateFont(50, 0, FontStyleBold);	\r\n
	 *																					\r\n
	 *                // 이와같이 폰트를 사용한다.										\r\n
	 *                m_Frame.SetFont(NFFontManager::GetInstance().GetHFont(0));		\r\n
	 *																					\r\n
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFFontManager : public Nave::NFSingleton<NFFontManager>
	{
	public:
		/// NFFontManager 생성자
		NFFontManager(void);
		/// NFFontManager 소멸자
		~NFFontManager(void);

	public:
		/// 모든 폰트를 삭제한다.
		void								RemoveAllFonts();

		/**
		 * @brief	기본 폰트를 설정합니다.
		 * @param strFontName 폰트이름
		 */
		void								SetDefaultFont(const WCHAR* strFontName);
		/**
		 * @brief	새로운 폰트를 추가합니다.
		 * @param strFont 폰트이름
		 */
		void								AddFont(const WCHAR* strFont);
		/**
		 * @brief	새로운 폰트를 추가합니다.
		 * @param pMem		메모리포인터
		 * @param dwSize	메모리 크기
		 */
		void								AddFontMem(PVOID pMem, DWORD dwSize);

		/// 윈도우에 폰트를 업데이트 합니다.
		void								UpdateFontFamily();

		/**
		 * @brief	새로운 폰트를 생성합니다.
		 * @param FontSize		폰트크기
		 * @param iFontType		폰트 타입
		 * @param FontStyle		폰트 스타일
		 * @return	생성된 폰트번호
		 */
		int									CreateFont(Gdiplus::REAL FontSize, int iFontType, INT FontStyle);

		/**
		 * @brief	폰트를 검색합니다.
		 * @param strFontName	폰트이름
		 * @return	폰트 인덱스
		 */
		int									FindFont(const WCHAR* strFontName);

		/**
		 * @brief	폰트 이름을 가져옵니다.
		 * @param iIndex	폰트 인덱스
		 * @return  폰트 이름
		 */
		Nave::String						GetFontName(int iIndex = -1);
		/**
		 * @brief	FontFamily를 얻어옵니다.
		 * @param iIndex	폰트 인덱스
		 * @return	FontFamily값
		 */
		Gdiplus::FontFamily*				GetFontFamily(int iIndex = -1);
		/**
		 * @brief	Font를 얻어옵니다.
		 * @param iIndex	폰트 인덱스
		 * @return  Font값
		 */
		Gdiplus::Font*						GetFont(int iIndex = -1);
		/**
		 * @brief	HFONT를 얻어옵니다.
		 * @param iIndex	폰트 인덱스
		 * @return	HFONT값
		 */
		HFONT								GetHFont(int iIndex = -1);

	private:
		/**
		 * @brief	새로운 폰트를 생성합니다.
		 * @param FontSize		폰트크기
		 * @param lpszFontName	폰트이름
		 * @param FontWeight	폰트무게
		 * @return	생성된 폰트
		 */
		HFONT								CreateFontEx(INT FontSize, LPCWSTR lpszFontName, INT FontWeight = 0);

	private:
		/// GDI+ 폰트 초기화 객체
		ULONG_PTR							m_gdiplusToken;
		/// GDI+ 폰트 초기화 객체
		Gdiplus::PrivateFontCollection*		m_gdiFont;

		/// 기본폰트 정보
		Gdiplus::FontFamily*				m_pDefaultFontFamily;
		/// 폰트 FontFamily 정보
		Gdiplus::FontFamily*				m_pFontFamily;
		/// 폰트 FontFamily 개수
		INT									m_iFound;

		/// 기본폰트
		HFONT								m_hDefaultFont;
		/// 기본폰트
		Gdiplus::Font*						m_pDefaultFont;

		/// 생성된 GDI+폰트
		std::vector<Gdiplus::Font*>			m_vetFonts;

		/// 생성된 GDI폰트
		std::vector<HFONT>					m_vetHFonts;

		/// 폰트 이름리스트
		std::vector<Nave::String>			m_vetFontFile;
	};

}