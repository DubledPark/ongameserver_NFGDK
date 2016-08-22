#include "Nave.h"
#include "NFIni.h"

namespace Nave { 

	NFIni::NFIni()
	{
		ZeroMemory(m_szFileName, sizeof(m_szFileName));
	}

	NFIni::~NFIni()
	{
	}

	BOOL NFIni::Open(LPCWSTR fileName)
	{
		if (!fileName)
			return FALSE;

		wcsncpy(m_szFileName, fileName, MAX_PATH);

		return TRUE;
	}

	BOOL NFIni::Close()
	{
		return TRUE;
	}

	BOOL NFIni::GetValue(LPCWSTR keyName, LPCWSTR valueName, LPDWORD value)
	{
		if (!keyName || !valueName || !value)
			return FALSE;

		*value = GetPrivateProfileIntW(keyName, valueName, 0, m_szFileName);

		return TRUE;
	}

	BOOL NFIni::GetValue(LPCWSTR keyName, LPCWSTR valueName, FLOAT *value)
	{
		if (!keyName || !valueName || !value)
			return FALSE;

		WCHAR Value[16] = {0,};

		GetPrivateProfileStringW(keyName, valueName, L"", Value, 16, m_szFileName);

		*value = (FLOAT)_wtof(Value);

		return TRUE;
	}

	BOOL NFIni::GetValue(LPCWSTR keyName, LPCWSTR valueName, LPWSTR value, DWORD bufferLength)
	{
		if (!keyName || !valueName || !value || bufferLength == 0)
			return FALSE;

		GetPrivateProfileStringW(keyName, valueName, L"", value, bufferLength, m_szFileName);

		return TRUE;
	}

	BOOL NFIni::SetValue(LPCWSTR keyName, LPCWSTR valueName, DWORD value)
	{
		if (!keyName || !valueName)
			return FALSE;

		WCHAR Value[16]	= {0,};
		_itow(value, Value, 10);

		WritePrivateProfileStringW(keyName, valueName, Value, m_szFileName);

		return TRUE;
	}

	BOOL NFIni::SetValue(LPCWSTR keyName, LPCWSTR valueName, LPCWSTR value)
	{
		if (!keyName || !valueName || !value)
			return FALSE;

		WritePrivateProfileStringW(keyName, valueName, value, m_szFileName);

		return TRUE;
	}

	BOOL NFIni::SetValue(LPCWSTR keyName, LPCWSTR valueName, FLOAT value)
	{
		if (!keyName || !valueName)
			return FALSE;

		WCHAR Value[16]	= {0,};
		swprintf(Value, L"%f", value);

		WritePrivateProfileStringW(keyName, valueName, Value, m_szFileName);

		return TRUE;
	}

} 