
#include "main.h"
#include <Nave2D/UIDialogManager.h>

CTestApp* g_pApp = NULL;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strLine, INT iLineCnt)
{
	NFLog::SetLogOutput(TRUE);
	NFLog::SetLogLimit(NFLog::Info);

	NFException::EnableUnhandledExceptioinFilter(true);
	NFException::EnableSaveDump(true);

	LOG_IMPORTANT((L"Test Client Init"));

	g_pApp = new CTestApp();
	if (!g_pApp)
		return 0;

	BOOL WIndowMode = TRUE;
	if(!g_pApp->CreateWindows(hInst, 0, L"Client", 1024, 768, FALSE, WIndowMode, MIS(IDI_APPLICATION)))
	{
		MessageBoxW(GetActiveWindow(), L"�ʱ�ȭ ����", L"Error", MB_OK);
		goto goto_exit;
	}

	g_pApp->StartProcess(WIndowMode);


goto_exit:
	LOG_IMPORTANT((L"Test Client End"));

	// EnableLog�� ������ �����忡�� ������ �������� �� NFLog�� Close ���ش�.
	NFLog::CloseLog();		

	_DELETE(g_pApp);

	return 0;
}