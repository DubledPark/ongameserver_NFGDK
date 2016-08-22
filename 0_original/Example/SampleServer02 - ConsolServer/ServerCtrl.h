#pragma once

class ServerCtrl : public NaveServer::NFServerCtrl
					,public NaveServer::UIConsol
{
	NaveServer::NFPacketPool		m_PacketPool;				// �޸� ó�� ���� 
	TestConnection*					m_pLogin;					// Client List ����

	DWORD							m_iPrevTick;
	int								m_iMaxPacket;

public:
	// ����� �˻�Ÿ�̸�.
	long						m_uTimerID;
	long						m_uTimerPeriod;
	static void __stdcall		CALLBACK	TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	
	VOID						Update();									// Process ó�� �Լ�

	VOID						ShowServerInfo();							// ���۽� ȭ�鿡 ���� ���� ǥ��

	BOOL						CreateSession(SOCKET sckListener);
	BOOL						ReleaseSession();

	virtual VOID				InitializeCommand();

	// ����� ������Ʈ�� �ʱ�ȭ �մϴ�. (�����켳���� ����� �Ŀ� ȣ��)
	virtual VOID				InitObject();
	// ����� ������Ʈ�� �����մϴ�. (���η����� ������(EndProcessȣ����) ȣ��)
	virtual VOID				ReleaseObject();

public:	
	VOID						UpdateInfo();
	VOID						SendMsg(WCHAR* strParam);

public:
	ServerCtrl(void);
	~ServerCtrl(void);
};
