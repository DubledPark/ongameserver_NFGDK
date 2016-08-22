#include "Nave.h"

namespace Nave {

	HWND			hMainWnd = NULL;
	HINSTANCE		hInstance = NULL;	

	void EnableLFH()
	{
		HANDLE hHeap[1025];
		UINT uEA = GetProcessHeaps(1024, hHeap);

		for (UINT i=0; i<uEA; ++i)
		{
			ULONG  HeapFragValue = 2;

			HeapSetInformation(hHeap[i],
				HeapCompatibilityInformation,
				&HeapFragValue,
				sizeof(HeapFragValue) );
		}
	}

	void SetLocale(CHAR* pLocale)
	{
		::setlocale(LC_ALL, pLocale);
	}

	DWORD GetGUID()
	{
		static DWORD s_dwPerGUID = 0;

		return ++s_dwPerGUID;
	}

	SIZE GetStringSize( const WCHAR* string, HFONT font )
	{
		HWND hWnd = Nave::hMainWnd;
		HDC hDC = ::GetDC(hWnd);

		SIZE size;
		HFONT hOldFont = (HFONT)SelectObject(hDC, font);

		GetTextExtentPoint32W(hDC, string, (INT)wcslen(string), &size);

		TEXTMETRICW	tm; 
		GetTextMetricsW(hDC, &tm);
		size.cx -= tm.tmOverhang;

		SelectObject(hDC, hOldFont);
		::ReleaseDC(hWnd, hDC);

		return size;
	}

	POINT GetMousePos()
	{
		POINT Point;
		GetCursorPos(&Point);
		ScreenToClient(hMainWnd, &Point);
		return Point;
	}

	POINT GetMousePos( HWND wnd )
	{
		POINT Point;
		GetCursorPos(&Point);
		ScreenToClient(wnd, &Point);
		return Point;
	}

	HWND IsWindowFromPoint()
	{
		POINT Point;
		GetCursorPos(&Point);
		return WindowFromPoint(Point);
	}

	INT	GetProgramName( LPWSTR buf, INT len )
	{
		WCHAR szDrive[MAX_PATH], szDir[MAX_PATH], szFilename[MAX_PATH], szExt[MAX_PATH];

		// Figure out what the report file will be named, and store it away
		GetModuleFileNameW(0, buf, len);

		LPWSTR pszDot = buf;
	    
		// Look for the '.' before the "EXE" extension. Replace '.' to '\0'
		if((pszDot = wcsrchr( pszDot, L'.'))) 
		{
			*pszDot = 0;
		}

		_wsplitpath(buf, szDrive, szDir, szFilename, szExt);
		wcsncpy(buf, szFilename, len);

		return (INT)wcslen( buf );
	}

	INT GetProgramDir( LPWSTR buf, INT len ) 
	{
		WCHAR* ProgDir;
		INT result;

		result = GetModuleFileNameW( hInstance, buf, len - 1 );
		if( result == 0 )
			return 0;

		WCHAR pgmPtr[1024];
		wcscpy( pgmPtr, buf );

		ProgDir = buf;
		ProgDir += wcslen( pgmPtr ) - 1;
		while( *ProgDir != L'\\' && ProgDir != pgmPtr )
			ProgDir--;

		*ProgDir = 0;
		return (INT)wcslen( buf );
	}

	INT GetDir( LPCWSTR path, LPWSTR buf )
	{
		wcscpy(buf, path);

		WCHAR* ProgDir;
		ProgDir = buf;
		ProgDir += wcslen(buf);
		while( *ProgDir != L'\\')
		{
			ProgDir[0] = 0;
			ProgDir--;
		}

		ProgDir[0] = 0;
		return (INT)wcslen( buf );
	}

	BOOL GetLine( const CHAR* sBuf, INT maxsBuf, CHAR* tBuf, INT maxtBuf, LONG& index )
	{
		ZeroMemory(tBuf, sizeof(tBuf));
		INT DestIndex = index;

		CHAR* pLine = "\n";
		INT pLen = strlen(pLine);

		for(INT i = 0; i < maxsBuf; i++)
		{
			if(index+i >= maxtBuf)
				break;

			if(strncmp(&sBuf[index+i], pLine, pLen) == 0)
			{
				CopyMemory(tBuf, sBuf+index, i);
				index += (i+pLen);
				tBuf[i] = 0;
				return TRUE;
			}
		}
		INT len = maxsBuf-index;
		if(maxtBuf <= len)
			len = maxtBuf-index;

		CopyMemory(tBuf, sBuf+index, len);
		tBuf[len] = 0;
		index += len;
		return FALSE;
	}

	VOID GetString( const CHAR* sBuf, CHAR* tBuf, INT len, LONG& index )
	{
		CopyMemory(tBuf, sBuf+index, len);
		tBuf[len] = 0;
		index += len;
	}

	BYTE GetByte( const CHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(BYTE);
		return *(BYTE*)(sBuf+t_index);
	}

	SHORT GetShort( const CHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(SHORT);
		return *(SHORT*)(sBuf+t_index);
	}

	INT GetInt( const CHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(INT);
		return *(INT*)(sBuf+t_index);
	}

	DWORD GetDWORD( const CHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(DWORD);
		return *(DWORD*)(sBuf+t_index);
	}

	FLOAT GetFloat( const CHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(FLOAT);
		return *(FLOAT*)(sBuf+t_index);
	}

	DOUBLE GetDouble( const CHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(DOUBLE);
		return *(DOUBLE*)(sBuf+t_index);
	}

	VOID SetString( CHAR* tBuf, const CHAR* sBuf, INT len, LONG& index )
	{
		CopyMemory(tBuf+index, sBuf, len);
		index += len;
	}

	VOID SetByte( CHAR* tBuf, BYTE sByte, LONG& index )
	{
		*(tBuf+index) = (BYTE)sByte;
		index++;
	}

	VOID SetShort( CHAR* tBuf, SHORT sShort, LONG& index )
	{
		SHORT temp = (SHORT)sShort;

		CopyMemory( tBuf+index, &temp, sizeof(SHORT));
		index += sizeof(SHORT);
	}

	VOID SetInt( CHAR* tBuf, INT sInt, LONG& index )
	{
		CopyMemory( tBuf+index, &sInt, sizeof(INT));
		index += sizeof(INT);
	}

	VOID SetDWORD( CHAR* tBuf, DWORD sDword, LONG& index )
	{
		CopyMemory( tBuf+index, &sDword, sizeof(DWORD));
		index += sizeof(DWORD);
	}

	VOID SetFloat( CHAR* tBuf, FLOAT sFloat, LONG& index )
	{
		CopyMemory( tBuf+index, &sFloat, sizeof(FLOAT));
		index += sizeof(FLOAT);
	}

	VOID SetDouble( CHAR* tBuf, DOUBLE sDouble, LONG& index )
	{
		CopyMemory( tBuf+index, &sDouble, sizeof(DOUBLE));
		index += sizeof(DOUBLE);
	}

	BOOL GetLine( const WCHAR* sBuf, INT maxsBuf, WCHAR* tBuf, INT maxtBuf, LONG& index )
	{
		ZeroMemory(tBuf, sizeof(tBuf));
		INT DestIndex = index;

		WCHAR* pLine = L"\n";
		INT pLen = wcslen(pLine);

		for(INT i = 0; i < maxsBuf; i++)
		{
			if(index+i >= maxtBuf)
				break;

			if(wcsncmp(&sBuf[index+i], pLine, pLen) == 0)
			{
				CopyMemory(tBuf, sBuf+index, sizeof(WCHAR)*i);
				index += (i+pLen);
				tBuf[index-1] = 0;
				return TRUE;
			}
		}
		INT len = maxsBuf-index;
		CopyMemory(tBuf, sBuf+index, sizeof(WCHAR)*len);
		tBuf[len] = 0;
		index += len;
		return FALSE;
	}

	VOID GetString( const WCHAR* sBuf, WCHAR* tBuf, INT len, LONG& index )
	{
		CopyMemory(tBuf, sBuf+index, sizeof(WCHAR)*len);
		tBuf[len] = 0;
		index += len;
	}

	BYTE GetByte( const WCHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(BYTE);
		return *(BYTE*)(sBuf+t_index);
	}

	SHORT GetShort( const WCHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(SHORT);
		return *(SHORT*)(sBuf+t_index);
	}

	INT GetInt( const WCHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(INT);
		return *(INT*)(sBuf+t_index);
	}

	DWORD GetDWORD( const WCHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(DWORD);
		return *(DWORD*)(sBuf+t_index);
	}

	FLOAT GetFloat( const WCHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(FLOAT);
		return *(FLOAT*)(sBuf+t_index);
	}

	DOUBLE GetDouble( const WCHAR* sBuf, LONG& index )
	{
		INT t_index = index;
		index += sizeof(DOUBLE);
		return *(DOUBLE*)(sBuf+t_index);
	}

	VOID SetString( WCHAR* tBuf, const WCHAR* sBuf, INT len, LONG& index )
	{
		CopyMemory(tBuf+index, sBuf, sizeof(WCHAR)*len);
		index += len;
	}

	VOID SetByte( WCHAR* tBuf, BYTE sByte, LONG& index )
	{
		*(tBuf+index) = (BYTE)sByte;
		index++;
	}

	VOID SetShort( WCHAR* tBuf, SHORT sShort, LONG& index )
	{
		SHORT temp = (SHORT)sShort;

		CopyMemory( tBuf+index, &temp, sizeof(SHORT));
		index += sizeof(SHORT);
	}

	VOID SetInt( WCHAR* tBuf, INT sInt, LONG& index )
	{
		CopyMemory( tBuf+index, &sInt, sizeof(INT));
		index += sizeof(INT);
	}

	VOID SetDWORD( WCHAR* tBuf, DWORD sDword, LONG& index )
	{
		CopyMemory( tBuf+index, &sDword, sizeof(DWORD));
		index += sizeof(DWORD);
	}

	VOID SetFloat( WCHAR* tBuf, FLOAT sFloat, LONG& index )
	{
		CopyMemory( tBuf+index, &sFloat, sizeof(FLOAT));
		index += sizeof(FLOAT);
	}

	VOID SetDouble( WCHAR* tBuf, DOUBLE sDouble, LONG& index )
	{
		CopyMemory( tBuf+index, &sDouble, sizeof(DOUBLE));
		index += sizeof(DOUBLE);
	}

	VOID Read( const char* sBuf, void* tBuf, INT len, LONG& seek )
	{
		CopyMemory(tBuf, &sBuf[seek], len);
		seek += len;
	}

	VOID Write( const void* sBuf, char* tBuf, INT len, LONG& seek )
	{
		CopyMemory(&tBuf[seek], sBuf, len);
		seek += len;
	}


}