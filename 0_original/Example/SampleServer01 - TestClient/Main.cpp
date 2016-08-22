// LabyrinthChat.cpp : Defines the entry point for the application.
//

#include "Global.h"
#include "TestUI.h"
#include "resource.h"

#include <Nave/NFRandom.h>
#include <Nave/NFTokenizer.h>

#include <Nave/NFLogManager.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	Nave::NFException::EnableUnhandledExceptioinFilter(true);
	Nave::NFException::EnableSaveDump(true);

	Nave::NFLog::SetLogLimit(Nave::NFLog::Warning);
	//	Nave::NFLog::SetLogDetail(true);

	Nave::NFLogManager* pLog = new Nave::NFLogManager();
	pLog->Initialize(TRUE);

	_DELETE(pLog);

	// 유니코드 변경 테스트
	WCHAR strCmd[512];
	_tcscpy(strCmd, Nave::ToString(lpCmdLine).c_str() );

	CTestUI MainWnd;

	g_pMainWnd = &MainWnd;

	if(!MainWnd.Init(hInstance, nCmdShow, L"Test Consol", MIS(IDI_ICON1)))
		return 0;	
	
	MainWnd.StartCommand();

	// 로그파일을 종료한다.
	Nave::NFLog::CloseLog();

	return 0;
}
