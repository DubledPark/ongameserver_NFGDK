// AlphaServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "Global.h"
#include <Nave/NFException.h>
#include <Nave/NFLog.h>
#include <Nave/NFTokenizer.h>
#include <Nave/NFStringUtil.h>
#include "ServerCtrl.h"

int wmain(int argc, WCHAR* argv[])
{
	// wprintf 를 사용하기 위해서
	// 로그를 콘솔에 출력하기 위해서는 아래와 같이 로케일 설정을 해줘야한다.
	setlocale(LC_ALL, "Korean");


	// Exception을 처리하기 위한 함수
	Nave::NFException::EnableUnhandledExceptioinFilter(true);
	Nave::NFException::EnableSaveDump(true);

	Nave::NFLog::SetLogLimit(Nave::NFLog::Warning);
//	Nave::NFLog::SetLogDetail(true);

	g_Server = new ServerCtrl();

	if(!g_Server)
	{
		return 0;
	}

	// 메인서버 연결
	if(argc == 1)
	{
		g_Config.Load();
	}
	else
	{
		if(argc > 1)
			g_Config.Load(argv[1]);
		else
			g_Config.Load();
	}

	int iLogSave=0;
	iLogSave = g_Config.GetLogSave();
	if(iLogSave)
		Nave::NFLog::SetLogOutput(true);

	WCHAR	szTitle[32];
	_stprintf(szTitle, L"%s", g_Config.GetTitle());

	if(!g_Server->Init())
		return 0;

	g_Server->StartCommand();

	_DELETE(g_Server);

	// 로그파일을 종료한다.
	Nave::NFLog::CloseLog();
	return 0;
}
