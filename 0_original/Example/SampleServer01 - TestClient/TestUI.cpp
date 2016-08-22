// LabyrinthUI.cpp: implementation of the CTestUI class.
//
//////////////////////////////////////////////////////////////////////

#include "Global.h"
#include "TestUI.h"
#include <Nave/NFTokenizer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ����� Ŀ�ǵ��� ����.
class CCmdHelp : public NaveTest::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		if(g_pMainWnd) g_pMainWnd->ShowCommand();
        return TRUE;
    }
};

class CCmdClear : public NaveTest::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		NaveTest::ClearMsgView();
        return TRUE;
    }
};

class CCmdClose : public NaveTest::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		if(g_pMainWnd) g_pMainWnd->EndCommand();
        return TRUE;
    }
};

class CCmdOpen : public NaveTest::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		if(g_pMainWnd) g_pMainWnd->Connect(lpParam);
        return TRUE;
    }
};

class CCmdTest : public NaveTest::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		if(g_pMainWnd) g_pMainWnd->SendTest();
        return TRUE;
    }
};

class CCmdMsg : public NaveTest::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		if(g_pMainWnd) g_pMainWnd->SendMsg(lpParam);
        return TRUE;
    }
};


CTestUI::CTestUI()
{
}

CTestUI::~CTestUI()
{
	
}

VOID CTestUI::InitializeCommand()
{
	ADD_COMMAND(L"help", CCmdHelp, L"ȭ�鿡 Ŀ��帮��Ʈ�� �����ݴϴ�.");
	ADD_COMMAND(L"clear", CCmdClear, L"ȭ���� �ʱ�ȭ �մϴ�.");	
	ADD_COMMAND(L"close", CCmdClose, L"������ �����մϴ�.");
	ADD_COMMAND(L"open", CCmdOpen, L"�׽�Ʈ�� �����մϴ�. (ip,port)");
	ADD_COMMAND(L"test", CCmdTest, L"send auto test");
	ADD_COMMAND(L"msg", CCmdMsg, L"send msg");

	DoCommand(L"help");
}

void CTestUI::Connect(WCHAR* strParam)
{
	Nave::NFTokenizer token(strParam, L",");

	if(token.CountTokens() != 2)
	{
		ShowCommand();
		return;
	}

	Nave::String ip = token.NextToken();
	Nave::String port = token.NextToken();

	if(m_Net.Init(Nave::ToASCII(ip).c_str(), Nave::ToInt(port)))
	{
		LOG_IMPORTANT((L"������ ����."));
	}
	else
	{
		LOG_IMPORTANT((L"������ ����."));
	}
}

void CTestUI::SendTest()
{
	m_bSend = !m_bSend;
}

void CTestUI::SendMsg(WCHAR* strParam)
{
	m_Net.Send_IRC("/MSG", Nave::ToASCII(strParam).c_str());
}

// WM_COMMAND
VOID CTestUI::OnCommand(HWND hWnd, INT nID, INT iEvent, LPARAM lParam)
{
}

// ����� ������Ʈ�� �ʱ�ȭ �մϴ�. (�����켳���� ����� �Ŀ� ȣ��)
VOID CTestUI::InitObject()
{
	m_Net.SetEventListener(this);

	m_bSend = FALSE;
	m_iRecv = 0;
	m_iSend = 0;
	// 400byte
	strcpy(m_strSend, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
	int iLen = strlen(m_strSend); 
	LOG_IMPORTANT((L"AutoSend Packet Size : %d", iLen));
}

// ����� ������Ʈ�� �����մϴ�. (���η����� ������(EndProcessȣ����) ȣ��)
VOID CTestUI::ReleaseObject()
{
	LOG_IMPORTANT((L"������ Close�մϴ�."));
}

VOID CTestUI::UpdateInfo()
{
	WCHAR	szTime[32];
	WCHAR	szDate[32];

	_tzset();
	_tstrdate( szDate );
	_tstrtime( szTime );

	WCHAR strInfo[512];
	_stprintf(strInfo, L"UpdateTime : %s-%s\r\nRecv  : %d, Send : %d.", szDate, szTime, m_iRecv, m_iSend); 

	NaveTest::UpdateInfo(strInfo);
}


VOID CTestUI::Update()
{
	m_Net.Update();

	if(timeGetTime()-m_PrevTick > 1000)
	{
		UpdateInfo();
		m_PrevTick = timeGetTime();
		m_iRecv = 0;
		m_iSend = 0;
	}

	if(m_bSend && m_Net.IsConnect())
	{
		++m_iSend;
		m_Net.Send_TEST(m_strSend);
		Sleep(1);
	}
}

void CTestUI::EventIRC(CHAR* strCmd, CHAR* strMsg)
{
	LOG_IMPORTANT((L"%s %s", Nave::ToString(strCmd).c_str(), Nave::ToString(strMsg).c_str()));
}

void CTestUI::EventTEST(CHAR* strMsg)
{
	++m_iRecv;

	static int Count = 0;
	static unsigned int Tick = timeGetTime();
	Count++;
	if(Count >= 1000)
	{
		LOG_IMPORTANT((L"TEST Packet %d Count Completion (%dTick)", Count, timeGetTime()-Tick));
		Count = 0;
		Tick = timeGetTime();
	}	
}

void CTestUI::EventConnect(BOOL bConnect)
{
	if(bConnect)
	{
		LOG_IMPORTANT((L"���Ӽ���"));
//		m_bSend = TRUE;
	}
	else
	{
		LOG_IMPORTANT((L"���ӽ���"));
//		m_bSend = FALSE;
	}
}

void CTestUI::EventSocketClose()
{
	LOG_IMPORTANT((L"��������"));
	m_bSend = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	Descrip	: ���״�� ����� MsgProc , WindowProc���� ȣ��ȴ�.
//	Date	: 2001-04-11���� 10:41:02
///////////////////////////////////////////////////////////////////////////////
LRESULT CTestUI::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
//		return TRUE;
		EndCommand();
		return FALSE;
	}
	return NaveTest::UIWindow::MsgProc( hWnd, uMsg, wParam, lParam );
}

//--------------------------- Ÿ�̸� ���� ��ƾ �߰�
VOID CTestUI::StartCommand()
{
	MSG		msg;

	while( !IsExit() )
	{
		// ������ �޽����� ȭ�鿡 ����ϱ� ���ؼ�.
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		Update();

		Sleep(1);
	}

	LOG_IMPORTANT((L"Programe Quitting."));

	ReleaseObject();

	InvalidateRect(NaveTest::hMainWnd, NULL, FALSE);

	// ���� �޽��� ǥ�� �� �ֱ� ���ؼ�
	for(int i = 0; i < 10; i++)
	{
		// ������ �޽����� ȭ�鿡 ����ϱ� ���ؼ�.
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	// ��Ʈ�� ���߿� ������� �ϱ�����
	NaveTest::ReleaseGlobalObject();

	Sleep(1000);
}

