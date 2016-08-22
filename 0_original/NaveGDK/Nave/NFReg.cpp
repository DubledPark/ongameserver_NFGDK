#include "Nave.h"
#include "NFReg.h"

namespace Nave { 

	NFReg::NFReg() : m_hRootKey(NULL), m_bIsOpened(FALSE)
	{
	}

	NFReg::~NFReg()
	{
	}

	BOOL NFReg::Open(HKEY rootKey, LPCWSTR subKey)
	{
		if (!subKey)
			return FALSE;

		if (m_bIsOpened)
			return FALSE;

		if (RegOpenKeyW(rootKey, subKey, &m_hRootKey) != ERROR_SUCCESS)
			return FALSE;

		m_bIsOpened = TRUE;

		return TRUE;
	}

	BOOL NFReg::Close()
	{
		if (RegCloseKey(m_hRootKey) != ERROR_SUCCESS)
			return FALSE;

		m_bIsOpened = FALSE;

		return TRUE;
	}

	BOOL NFReg::CreateKey(HKEY rootKey, LPCWSTR subKey)
	{
		if (!subKey)
			return FALSE;
		
		if (m_bIsOpened)
			return FALSE;

		if (RegCreateKeyW(rootKey, subKey, &m_hRootKey) != ERROR_SUCCESS)
			return FALSE;

		m_bIsOpened = TRUE;

		return TRUE;
	}

	BOOL NFReg::DeleteKey(HKEY rootKey, LPCWSTR subKey)
	{
		if (!subKey)
			return FALSE;

		if (RegDeleteKeyW(rootKey, subKey) != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	BOOL NFReg::SetValue(LPCWSTR valueName, LPCWSTR value)
	{
		if (!valueName || !value)
			return FALSE;

		if (!m_bIsOpened)
			return FALSE;

		if (RegSetValueExW(m_hRootKey, valueName, 0, REG_SZ, (BYTE*) value, (DWORD) wcslen(value) * sizeof(WCHAR))
			!= ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	BOOL NFReg::SetValueForMultiSz(LPCWSTR valueName, LPCWSTR value, DWORD byteLength)
	{
		if (!valueName || !value)
			return FALSE;

		if (!m_bIsOpened)
			return FALSE;

		if (RegSetValueExW(m_hRootKey, valueName, 0, REG_MULTI_SZ, (BYTE*) value, byteLength)
			!= ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	BOOL NFReg::SetValue(LPCWSTR valueName, DWORD value)
	{
		if (!valueName)
			return FALSE;

		if (!m_bIsOpened)
			return FALSE;

		if (RegSetValueExW(m_hRootKey, valueName, 0, REG_DWORD, (BYTE*) &value, sizeof(DWORD))
			!= ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	BOOL NFReg::GetValue(LPCWSTR valueName, LPWSTR value, LPDWORD bufferLength)
	{
		DWORD ValueType = 0;

		if (!valueName || !value || !bufferLength)
			return FALSE;

		if (!m_bIsOpened)
			return FALSE;

		if (RegQueryValueExW(m_hRootKey, valueName, 0, &ValueType, (BYTE*) value, bufferLength)
			!= ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	BOOL NFReg::GetValue(LPCWSTR valueName, LPDWORD value)
	{
		DWORD	ValueType		= 0;
		DWORD	BufferLength	= sizeof(DWORD);

		if (!valueName || !value)
			return FALSE;

		if (!m_bIsOpened)
			return FALSE;

		if (RegQueryValueExW(m_hRootKey, valueName, 0, &ValueType, (BYTE*) value, &BufferLength)
			!= ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	BOOL NFReg::DeleteValue(LPCWSTR valueName)
	{
		if (!valueName)
			return FALSE;

		if (!m_bIsOpened)
			return FALSE;

		if (RegDeleteValueW(m_hRootKey, valueName) != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

} 