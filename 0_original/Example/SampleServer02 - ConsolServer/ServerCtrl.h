#pragma once

class ServerCtrl : public NaveServer::NFServerCtrl
					,public NaveServer::UIConsol
{
	NaveServer::NFPacketPool		m_PacketPool;				// 메모리 처리 변수 
	TestConnection*					m_pLogin;					// Client List 변수

	DWORD							m_iPrevTick;
	int								m_iMaxPacket;

public:
	// 사용자 검색타이머.
	long						m_uTimerID;
	long						m_uTimerPeriod;
	static void __stdcall		CALLBACK	TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	
	VOID						Update();									// Process 처리 함수

	VOID						ShowServerInfo();							// 시작시 화면에 서버 정보 표시

	BOOL						CreateSession(SOCKET sckListener);
	BOOL						ReleaseSession();

	virtual VOID				InitializeCommand();

	// 사용자 오브젝트를 초기화 합니다. (윈도우설정이 종료됀 후에 호출)
	virtual VOID				InitObject();
	// 사용자 오브젝트를 삭제합니다. (메인루프가 끊난후(EndProcess호출후) 호출)
	virtual VOID				ReleaseObject();

public:	
	VOID						UpdateInfo();
	VOID						SendMsg(WCHAR* strParam);

public:
	ServerCtrl(void);
	~ServerCtrl(void);
};
