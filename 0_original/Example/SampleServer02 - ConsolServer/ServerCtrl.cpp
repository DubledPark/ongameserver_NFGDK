#include "Global.h"
#include <Nave/NFLog.h>
#include <Nave/NFStringUtil.h>
#include "ServerCtrl.h"

// 등록한 커맨드의 정보.
class CCmdHelp : public NaveServer::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		if(g_Server) g_Server->ShowCommand();
        return TRUE;
    }
};

class CCmdClear : public NaveServer::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		NaveServer::UICmdMsgView::ClrarMsgView();
        return TRUE;
    }
};

class CCmdClose : public NaveServer::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		if(g_Server) g_Server->EndCommand();
        return TRUE;
    }
};

class CCmdMsg : public NaveServer::UICommand
{
public:
    virtual BOOL DoProcess(WCHAR* lpParam)
    {
		if(g_Server) g_Server->SendMsg(lpParam);
        return TRUE;
    }
};

ServerCtrl::ServerCtrl(void)
{
	m_pLogin	= NULL;					// Client List 변수
	m_uTimerID		= 0;
	m_uTimerPeriod	= 1;
}

ServerCtrl::~ServerCtrl(void)
{
}

VOID ServerCtrl::InitializeCommand()
{
	ADD_COMMAND(L"help", CCmdHelp, L"Show Command List.");
	ADD_COMMAND(L"clear", CCmdClear, L"Clear Display.");	
	ADD_COMMAND(L"close", CCmdClose, L"Close Server.");
	ADD_COMMAND(L"msg", CCmdMsg, L"Send IRC Message");
}

VOID ServerCtrl::SendMsg(WCHAR* strParam)
{
	NaveNet::NFPacket packet;

	packet.SetCommand(IRC);
	packet.SetSize(sizeof(PKIRC));

	LPPKIRC lpIRC = (LPPKIRC)packet.m_Packet;

	strcpy(lpIRC->Key, "/SVR");
	strcpy(lpIRC->Message, Nave::ToASCII(strParam).c_str());

	g_UManager.SendPostAll(packet);
}

VOID ServerCtrl::UpdateInfo()
{
	WCHAR	szTime[32];
	WCHAR	szDate[32];

	_tzset();
	_tstrdate( szDate );
	_tstrtime( szTime );

	WCHAR strInfo[512];

	_stprintf(strInfo, L"UpdateTime : %s-%s", szDate, szTime); 

	NaveServer::UICmdMsgView::UpdateInfo(strInfo);
}

VOID ServerCtrl::InitObject()
{
	m_iMaxPacket = 0;
	m_iPrevTick = timeGetTime();

	INT Port = g_Config.GetPort();
	INT MaxConn = g_Config.GetMaxConn();
	Start(Port, MaxConn);
}

// 사용자 오브젝트를 삭제합니다. (메인루프가 끊난후(EndProcess호출후) 호출)
VOID ServerCtrl::ReleaseObject()
{
	Stop();
}

VOID ServerCtrl::ShowServerInfo()
{
	// Join 시켰으니 Leave시켜야한다.
	WCHAR	szDate[32],
			szTime[32];

	
	_tzset();
	_tstrdate( szDate );
	_tstrtime( szTime );
	LOG_IMPORTANT((L"------------------------------------------------"));
	LOG_IMPORTANT((L" %s initialized at %s, %s", g_Config.GetTitle(), szDate, szTime) );
	LOG_IMPORTANT((L"------------------------------------------------"));
	//////////////////////////////////////////////////////////////////////
	//							Server start							//
	//////////////////////////////////////////////////////////////////////
	LOG_IMPORTANT((L"------------------------------------------------"));
	LOG_IMPORTANT((L"|                 SERVER START                 |"));
	LOG_IMPORTANT((L"------------------------------------------------"));

	WCHAR Ip[32];
	GetLocalIP(Ip);
	LOG_IMPORTANT((L"IP(%s), Port(%d), MaxLogin(%d)", Ip, m_iPort, m_iMaxConn));

//	Nave::IOCPServer::UpdateInfo(L"시작"));
}

BOOL ServerCtrl::CreateSession(SOCKET sckListener)
{
	if(m_PacketPool.Create(m_iMaxConn) == NULL)
		return FALSE;

	// pClientArray(Client Controler..)
	if((m_pLogin = new TestConnection[m_iMaxConn]) == NULL) 
		return FALSE;		// Create MAXUSER(1000) m_pLogin

	// Initialize pClientArray
	for(int nCnt = 0; nCnt < m_iMaxConn; nCnt++)						
	{
		if(m_pLogin[nCnt].Create(nCnt,
								m_hIOCP,
								sckListener,
								&m_PacketPool,
								DEF_PACKETSIZE) == 0) 
			return FALSE;
	}

	g_UManager.Init(m_iMaxConn);

	// Timer Setting 
	timeBeginPeriod(m_uTimerPeriod);
	m_uTimerID = timeSetEvent(1000,0, (LPTIMECALLBACK)TimerProc,(DWORD)0,TIME_PERIODIC);

	return TRUE;
}

BOOL ServerCtrl::ReleaseSession()
{
	// Close All User Sockets
	if( m_pLogin )
	{
		for(int nCnt = 0 ; nCnt < m_iMaxConn ; nCnt++)
		{
			m_pLogin[nCnt].Disconnect();
		}
		delete [] m_pLogin;
	}
	m_pLogin = NULL;

	// [03] 타이머 죽이기
	timeKillEvent(m_uTimerID); 
	timeEndPeriod(m_uTimerPeriod);

	// [01] Shutdown the Packet Pool
	m_PacketPool.Release();
	LOG_IMPORTANT((L"shutdown packet pool.." ));

	return TRUE;
}

// Process Update...
VOID ServerCtrl::Update()
{
	NaveServer::NFServerCtrl::Update();
}

//////////////////////////////////////////////////////////////////
// [1]DESCRIPTION : 타이머 함수						 			//
// [2]PARAMETER : dwUser  - 연결 객체를 넘기는 변수 			//
// [3]RETURN : void							 					//
// [4]DATE : 2000년 11월 21일									//
//////////////////////////////////////////////////////////////////
void ServerCtrl::TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	UpdateInfomation();
}