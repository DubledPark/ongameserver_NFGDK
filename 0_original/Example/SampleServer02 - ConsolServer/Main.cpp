// AlphaServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "Global.h"
#include <Nave/NFException.h>
#include <Nave/NFLog.h>
#include <Nave/NFTokenizer.h>
#include <Nave/NFStringUtil.h>
#include "ServerCtrl.h"

int wmain(int argc, WCHAR* argv[])
{
	// wprintf �� ����ϱ� ���ؼ�
	// �α׸� �ֿܼ� ����ϱ� ���ؼ��� �Ʒ��� ���� ������ ������ ������Ѵ�.
	setlocale(LC_ALL, "Korean");


	// Exception�� ó���ϱ� ���� �Լ�
	Nave::NFException::EnableUnhandledExceptioinFilter(true);
	Nave::NFException::EnableSaveDump(true);

	Nave::NFLog::SetLogLimit(Nave::NFLog::Warning);
//	Nave::NFLog::SetLogDetail(true);

	g_Server = new ServerCtrl();

	if(!g_Server)
	{
		return 0;
	}

	// ���μ��� ����
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

	// �α������� �����Ѵ�.
	Nave::NFLog::CloseLog();
	return 0;
}
