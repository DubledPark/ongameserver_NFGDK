// AlphaServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "Global.h"
#include <Nave/NFException.h>
#include <Nave/NFLog.h>
#include <Nave/NFTokenizer.h>
#include <Nave/NFStringUtil.h>
#include "ServerCtrl.h"
#include "resource.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR		lpCmdLine,
                     int       nCmdShow)
{
	// Exception을 처리하기 위한 함수
	Nave::NFException::EnableUnhandledExceptioinFilter(true);
	Nave::NFException::EnableSaveDump(true);

	Nave::NFLog::SetLogLimit(Nave::NFLog::Warning);
//	Nave::NFLog::SetLogDetail(true);

	WCHAR strCmd[512];
	_tcscpy(strCmd, Nave::ToString(lpCmdLine).c_str());

	g_Server = new ServerCtrl();

	if(!g_Server)
	{
		return 0;
	}

	// 메인서버 연결
	if(strlen(lpCmdLine) == 0)
	{
		g_Config.Load();
	}
	else
	{
		Nave::NFTokenizer token(strCmd, L" ");

		if(token.CountTokens() == 1)
			g_Config.Load(token.NextToken().c_str());
		else
			g_Config.Load();
	}

	int iLogSave=0;
	iLogSave = g_Config.GetLogSave();
	if(iLogSave)
		Nave::NFLog::SetLogOutput(true, 1*1024*1024);

	WCHAR	szTitle[32];
	_stprintf(szTitle, L"%s", g_Config.GetTitle());

	if(!g_Server->Init(hInstance, nCmdShow, 320, 300, szTitle, MIS(IDI_ICON1)))
		return 0;

	g_Server->StartCommand();

	_DELETE(g_Server);

	// 로그파일을 종료한다.
	Nave::NFLog::CloseLog();
	return 0;
}
