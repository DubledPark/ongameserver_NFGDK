#pragma once

#include "Packet.h"

// LoginServer의 UserConnect가 접속속할때 사용되는 Connect..
class TestConnection : public NaveServer::NFConnection
{
	NaveNet::NFPacket				m_SendPacket;

public:
	void Clear();

	void DispatchPacket( NaveNet::NFPacket& Packet );
	
//------------------------ 멤 버 변 수 -------------------------//
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
