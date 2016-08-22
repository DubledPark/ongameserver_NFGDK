#pragma once

#include "Packet.h"

// LoginServer�� UserConnect�� ���Ӽ��Ҷ� ���Ǵ� Connect..
class TestConnection : public NaveServer::NFConnection
{
	NaveNet::NFPacket				m_SendPacket;

public:
	void Clear();

	void DispatchPacket( NaveNet::NFPacket& Packet );
	
//------------------------ �� �� �� �� -------------------------//
protected:
	void  (TestConnection::*OnMsgMap[MaxPacket])(CHAR* Packet, int Len);

	void OnConnect(BOOL bConnect);
	void OnDisconnect();

public:
	void Parsed_IRC(CHAR* Packet, int Len);
	void Parsed_TEST(CHAR* Packet, int Len);

	void Send_IRC(const char* strCommand, const char* strMsg);
public:
	TestConnection(void);
	~TestConnection(void);
};
