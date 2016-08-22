#include "Nave.h"
#include "NFStringUtil.h"
#include <list>

#include "NFLog.h"

namespace Nave { 
	
	HWND			NFLog::s_hLogHandel = NULL;
	BOOL			NFLog::s_bEnableLogPrint = TRUE;
	BOOL			NFLog::s_bSaveLogFile = FALSE;
	int				NFLog::s_iLogLimit = NFLog::Info;
	BOOL			NFLog::s_bLogDetail = FALSE;
	NFLogManager*	NFLog::s_pLogManager = NULL;


	// Message를 출력할 윈도우 핸들을 셋팅한다.
	void NFLog::SetLogHandel(HWND hListWnd)
	{
		s_hLogHandel = hListWnd;
	}

	void NFLog::SetLogPrint(BOOL enable)
	{
		s_bEnableLogPrint = enable;
	}

	void NFLog::SetLogOutput(BOOL enable, DWORD dwSize)
	{
		s_bSaveLogFile = enable;
		SetLogPrint(enable);

		_DELETE(s_pLogManager);
		if(s_bSaveLogFile)
		{
			s_pLogManager = new NFLogManager();
			s_pLogManager->Initialize(FALSE);
			s_pLogManager->SetLogMaxSize(dwSize);
		}
	}

	void NFLog::CloseLog()
	{
		if(s_pLogManager)
		{
			s_pLogManager->Flush();
			_DELETE(s_pLogManager);
		}
	}

	void NFLog::SetLogLimit(int limit) 
	{
		s_iLogLimit=limit;
	}

	void NFLog::SetLogDetail(BOOL detail)
	{
		s_bLogDetail = detail;
	}

	void NFLog::AddLogMsg( HWND hWnd, WCHAR* String, int Len ) 
	{
		// 삭제
		int iCount = (int)SendMessageW( hWnd, LB_GETCOUNT, 0, 0L )-128;
		for(int i = 0; i < iCount; ++i)
			SendMessageW( hWnd, LB_DELETESTRING, 0, 0L );

		// 추가
		WCHAR* p;
		int k;
		p = String;

		int iAdd = 0;

		k = 0;
		while( k++ < Len ) 
		{
			switch( *String ) 
			{
			case L'\n':
				*String = 0;
				++iAdd;
				SendMessageW( hWnd, LB_ADDSTRING, 0, (LPARAM)(LPWSTR)p );
				p = ++String;
				break;
			default :
				++String;
			}
		}

		if( *p )
		{
			++iAdd;
			SendMessageW( hWnd, LB_ADDSTRING, 0, (LPARAM)(LPWSTR)p );
		}

		int Top = (int)SendMessageW( hWnd, LB_GETTOPINDEX, 0, 0L );
		SendMessageW( hWnd, LB_SETTOPINDEX, Top+iAdd, 0L );
	}

	void NFLog::LogPrintf( WCHAR* msg, ... )
	{
		va_list v;
		WCHAR buf[1024];
		int len;

		va_start( v, msg );
		len = vswprintf( buf, msg, v );

		va_end( v );

		LogPrintf( Info, buf);
	}

	void NFLog::OutputLog(const WCHAR* log)
	{
		static CHAR stDot[4] = "\r\n";
		static CHAR stTime[32];

		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		sprintf(stTime, "[%04d.%02d.%02d-%02d:%02d:%02d]", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		Nave::StringA str = stTime;
		str += ToASCII(log);
		str += stDot;

		int iSize = str.size();
		char* pPoint = s_pLogManager->ReserveBuffer(iSize);
		memcpy(pPoint, str.c_str(), iSize);
	}

	void NFLog::LogPrintf( int group, WCHAR* msg )
	{
		if (group < s_iLogLimit) 
			return;

		if(s_bEnableLogPrint)
		{
			if(s_hLogHandel == NULL)
			{
				// setlocale 함수를 지정해야 한글이 출력됨
				wprintf(msg);
				wprintf(L"\n");
			}
			else
				AddLogMsg( s_hLogHandel, msg, (int)wcslen(msg) );
		}

#ifdef _DEBUG
		OutputDebugStringW( msg );
		OutputDebugStringW( L"\r\n" );
#endif
		if(s_bSaveLogFile)
		{
			Nave::StringW tmp = msg;
			Nave::StringW logmsg;
			switch (group) {
			case Info:
				logmsg=L"[INF] " + tmp;
				break;
			case Warning:
				logmsg=L"[WAR] " + tmp;
				break;
			case Error:
				logmsg=L"[ERR] " + tmp;
				break;
			case Exception:
				logmsg=L"[EXP] " + tmp;
				break;
			case Important:
				logmsg=L"[DET] " + tmp;
				break;
			};

			OutputLog(logmsg.c_str());
		}
	}

	void NFLog::LogPrintf( int group, const WCHAR* pFile, int pLinenum, const WCHAR* pFunc, WCHAR* msg )
	{
		if (group < s_iLogLimit) 
			return;

		if(s_bEnableLogPrint)
		{
			if(s_hLogHandel == NULL)
			{
				wprintf(msg);
				wprintf(L"\n");
			}
			else
				AddLogMsg( s_hLogHandel, msg, (int)wcslen(msg) );
		}

#ifdef _DEBUG
		OutputDebugStringW( msg );
		OutputDebugStringW( L"\r\n" );
#endif

		if(s_bSaveLogFile)
		{
			Nave::StringW tmp = msg;
			Nave::StringW logmsg;
			switch (group) {
			case Info:
				logmsg=L"[INF]" + tmp;
				break;
			case Warning:
				logmsg=L"[WAR]" + tmp;
				break;
			case Error:
				logmsg=L"[ERR]" + tmp;
				break;
			case Exception:
				logmsg=L"[EXP]" + tmp;
				break;
			case Important:
				logmsg=L"[DET]" + tmp;
				break;
			};

			OutputLog(logmsg.c_str());

			static WCHAR deta[1024];
			swprintf(deta, L"                 -> [%s,%d,%s]", pFile, pLinenum, pFunc);
			OutputLog(deta);
		}
	}
    
	NFLog::Proxy::Proxy(int pGroup, const WCHAR* pFile, int pLinenum, const WCHAR* pFunc) : file(pFile),linenum(pLinenum), func(pFunc), group(pGroup) 
	{
	}

	void NFLog::Proxy::Log(const WCHAR* msg, ...) 
	{
		if (group < NFLog::s_iLogLimit) 
			return;

		WCHAR txt[1024];
		va_list l;
		va_start(l,msg);
		_vsnwprintf( txt, 1024, msg, l );
		va_end(l); 
		if(!NFLog::s_bLogDetail)
			LogPrintf(group,txt);
		else
			LogPrintf(group,file,linenum,func,txt);		// 이건 파일과 기타 정보를 남길때.. 
	}

}