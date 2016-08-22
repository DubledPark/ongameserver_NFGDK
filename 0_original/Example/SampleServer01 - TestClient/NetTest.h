#pragma once

#include <Nave/Nave.h>
#include <NaveNet/NaveNet.h>
#include <NaveNet/NFNetClientS.h>

#include "Packet.h"

class CNetTest : public NaveNet::NFNetClientS
{
public:
	CNetTest(void);
	~CNetTest(void);

public:
	//////////////////////////////////////////////////////////////////////
	// Socket -> Client�� ��Ŷ�� �����ϱ� ���� �����ϴ� Listener�̴�.
	//////////////////////////////////////////////////////////////////////
	class IEventListener
	{
	public:
		virtual ~IEventListener() {}
		virtual void EventIRC(CHAR* strCmd, CHAR* strMsg)=0;
		virtual void EventTEST(CHAR* strMsg)=0;
		virtual void EventConnect(BOOL bConnect)=0;
		virtual void EventSocketClose()=0;
	};

	void	SetEventListener(IEventListener* event)
	{
		m_event = event;
	}

public:
	virtual void	Update();

	//////////////////////////////////////////////////////////////////////
	// ToServer�� ���ǵ� ��Ŷ���
	// ���޹��� ��Ŷ�� ó���� ��Ŷ�Լ� ������ Recv_��Ŷ�̸�(CHAR* Packet, int Len) �̵ȴ�.
	//////////////////////////////////////////////////////////////////////
	void	Send_IRC(const char* strCommand, const  char* strMsg);

	void	Send_TEST(const  char* strMsg);

	void	Disconnect();

private:
	//////////////////////////////////////////////////////////////////////
	// CNetTest���� ���Ǵ� ��������
	//////////////////////////////////////////////////////////////////////
	//	Nave::Sync::CSync					m_Cs;

	// Send�� �ϱ����� ��Ŷ�� ���鶧 ���ȴ�.
	NaveNet::NFPacket					m_SendPacket;

	IEventListener*						m_event;

private:
	void	(CNetTest::*OnMsgMap[MaxPacket])(CHAR* Packet, int Len);

	// ���� �κп��� �������� �̺�Ʈ�� ó���Ҷ�
	void	OnSocketEvent(DWORD dID, DWORD dEvent);

	//////////////////////////////////////////////////////////////////////
	// FromServer�� ���ǵ� ��Ŷ��� (Ŭ���̾�Ʈ�� �������.)
	// ���޹��� ��Ŷ�� ó���� ��Ŷ�Լ� ������ Recv_��Ŷ�̸�(CHAR* Packet, int Len) �̵ȴ�.
	//////////////////////////////////////////////////////////////////////

	// IRC Packet ó���Լ�
	void	Parsed_IRC(CHAR* Packet, int Len);
	void	Parsed_TEST(CHAR* Packet, int Len);

	void	ProcessPacket(NaveNet::NFPacket* pPacket, int PacketLen);

};

