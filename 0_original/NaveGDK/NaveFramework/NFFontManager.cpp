#include <Nave/Nave.h>
#include "NFFontManager.h"

namespace NaveFramework { 

	NFFontManager::NFFontManager(void)
	{
		// init GDI+
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

		m_gdiFont = new Gdiplus::PrivateFontCollection();

		m_pFontFamily = NULL;
		m_iFound = 0;

		m_hDefaultFont = CreateFontEx(12, L"Arial", FW_SEMIBOLD);

		m_pDefaultFontFamily = new Gdiplus::FontFamily(L"Arial");
		m_pDefaultFont = new Gdiplus::Font(m_pDefaultFontFamily, 12, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	}

	NFFontManager::~NFFontManager(void)
	{
		RemoveAllFonts();

		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}

	HFONT NFFontManager::CreateFontEx(INT FontSize, LPCWSTR lpszFontName, INT FontWeight)
	{
		return ::CreateFontW(FontSize, 0, 0, 0, FontWeight, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH|FF_ROMAN, lpszFontName);
	}

	void NFFontManager::SetDefaultFont(const WCHAR* strFontName)
	{
		if(m_hDefaultFont)
		{
			DeleteObject(m_hDefaultFont);
			m_hDefaultFont = NULL;
		}

		_DELETE(m_pDefaultFontFamily);
		_DELETE(m_pDefaultFont);

		m_hDefaultFont = CreateFontEx(12, strFontName, FW_SEMIBOLD);

		m_pDefaultFontFamily = new Gdiplus::FontFamily(strFontName);
		m_pDefaultFont = new Gdiplus::Font(m_pDefaultFontFamily, 12, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	}

	int NFFontManager::FindFont(const WCHAR* strFontName)
	{
		WCHAR strName[32];

		for(int i = 0; i < m_iFound; ++i)
		{
			m_pFontFamily[i].GetFamilyName(strName);

			if(wcscmp(strFontName, strName) == 0)
				return i;
		}

		return 0;
	}

	int NFFontManager::CreateFont(Gdiplus::REAL FontSize, int iFontType, INT FontStyle)
	{
		Gdiplus::FontFamily* pFamily = GetFontFamily(iFontType);
		Gdiplus::Font* pFont = new Gdiplus::Font(pFamily, FontSize, FontStyle, Gdiplus::UnitPixel);
		
		LOGFONTW logFont;
		Gdiplus::Graphics g(Nave::hMainWnd);
		pFont->GetLogFontW(&g, &logFont);

		HFONT hFont = CreateFontIndirectW(&logFont);

		m_vetFonts.push_back(pFont);
		m_vetHFonts.push_back(hFont);

		return (int)m_vetFonts.size();
	}

	void NFFontManager::UpdateFontFamily()
	{
		if(!m_gdiFont)
			return;

		INT iNum = m_gdiFont->GetFamilyCount();
		_DELETE_ARRAY(m_pFontFamily);

		if(iNum == 0)
			return;

		m_pFontFamily = new Gdiplus::FontFamily[iNum];

		m_gdiFont->GetFamilies(iNum, m_pFontFamily, &m_iFound);	// GetFamilies는 1번인자의 개수만큼 등록된 폰트를 가져오게 한다. fontFamily가 배열로 들어가야함.

		SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	}

	Gdiplus::Font* NFFontManager::GetFont(int iIndex)
	{
		if(iIndex >= 0 && iIndex < (int)m_vetFonts.size())
			return m_vetFonts[iIndex];

		return m_pDefaultFont;
	}

	HFONT NFFontManager::GetHFont(int iIndex)
	{
		if(iIndex >= 0 && iIndex < (int)m_vetHFonts.size())
			return m_vetHFonts[iIndex];

		return m_hDefaultFont;
	}

	Nave::String NFFontManager::GetFontName(int iIndex)
	{
		WCHAR strName[32];
		if(iIndex >= 0 && iIndex < m_iFound)
		{
			m_pFontFamily[iIndex].GetFamilyName(strName);
		}
		else
		{
			m_pDefaultFontFamily->GetFamilyName(strName);
		}
		
		return Nave::String(strName);
	}

	Gdiplus::FontFamily* NFFontManager::GetFontFamily(int iIndex)
	{
		if(iIndex >= 0 && iIndex < m_iFound)
			return &m_pFontFamily[iIndex];

		return m_pDefaultFontFamily;
	}

	void NFFontManager::AddFont(const WCHAR* strFont)
	{
		Gdiplus::Status st;
		if(m_gdiFont)
			st = m_gdiFont->AddFontFile(strFont);

		// 이렇게 하면 API에서도 사용가능
		m_vetFontFile.push_back(strFont);
		AddFontResourceW(strFont);
	}

	void NFFontManager::AddFontMem(PVOID pMem, DWORD dwSize)
	{
		Gdiplus::Status st;
		if(m_gdiFont)
			st = m_gdiFont->AddMemoryFont(pMem, (int)dwSize);
	}

	void NFFontManager::RemoveAllFonts()
	{
		if(m_hDefaultFont)
		{
			DeleteObject(m_hDefaultFont);
			m_hDefaultFont = NULL;
		}


		_DELETE(m_pDefaultFontFamily);
		_DELETE(m_pDefaultFont);

		_DELETE_ARRAY(m_pFontFamily);

		int iSize = (int)m_vetFonts.size();
		for(int i = 0; i < iSize; ++i)
		{
			_DELETE(m_vetFonts[i]);
		}

		iSize = (int)m_vetFontFile.size();
		for(int i = 0; i < iSize; ++i)
		{
			RemoveFontResourceW(m_vetFontFile[i].c_str()) ;
		}

		m_vetFonts.clear();
		m_vetFontFile.clear();
		m_iFound = 0;

		SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0) ;
	}

}