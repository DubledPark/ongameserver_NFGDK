#include "Global.h"
#include <Nave/NFLog.h>
#include <Nave/NFStringUtil.h>
#include "TestConnection.h"

using namespace NaveNet;

TestConnection::TestConnection(void)
{
	// 함수를 Map으로 선언한다.
	OnMsgMap[IRC] = &TestConnection::Parsed_IRC;
	OnMsgMap[TEST] = &TestConnection::Parsed_TEST;
}

TestConnection::~TestConnection(void)
{
}

//////////////////////////////////////////////////////////////////////
// 변수 초기화..
//////////////////////////////////////////////////////////////////////
void TestConnection::Clear()
{
	m_SendPacket.Init();
}

void TestConnection::DispatchPacket(NFPacket& Packet)
{
//	InterlockedExchange((LONG*)&g_iProcessPacket,g_iProcessPacket+1);

	// 함수 호출
	(this->*OnMsgMap[Packet.GetCommand()])(Packet.m_Packet, Packet.GetSize());
/*
	if(Packet.GetCommand() == 0)
		Parsed_IRC(Packet.m_Packet, Packet.GetSize());
	else
		Parsed_TEST(Packet.m_Packet, Packet.GetSize());
*/
}

void TestConnection::OnConnect(BOOL bConnect)
{
	if(bConnect)
	{
		LOG_IMPORTANT((L"[%04d] Connect User", GetIndex()));

		g_UManager.Join(this);
	}
}

void TestConnection::OnDisconnect()
{
	g_UManager.Levae(GetIndex());
	LOG_IMPORTANT((L"[%04d] Disconnect User", GetIndex()));
	m_SendPacket.Init();
}

void TestConnection::Parsed_IRC(CHAR* Packet, int Len)
{
	LPPKIRC pkIRC = (LPPKIRC)Packet;

	CHAR Buff[DEF_PACKETSIZE];
	ZeroMemory(Buff, sizeof(Buff));
	sprintf(Buff, "%s : %s", pkIRC->Key, pkIRC->Message);

	String strCmd = Nave::ToString(Buff);
	LOG_IMPORTANT((strCmd.c_str()));
}

void TestConnection::Parsed_TEST(CHAR* Packet, int Len)
{
	LPPKTEST pkTest = (LPPKTEST)Packet;

	m_SendPacket.SetCommand(TEST);
	m_SendPacket.SetSize(sizeof(PKTEST));
	memcpy(m_SendPacket.m_Packet, pkTest->Message, Len);

	SendPost(m_SendPacket);
}

void TestConnection::Send_IRC(const char* strCommand, const char* strMsg)
{
	m_SendPacket.SetCommand(IRC);
	m_SendPacket.SetSize(sizeof(PKIRC));

	LPPKIRC lpIRC = (LPPKIRC)m_SendPacket.m_Packet;

	strcpy(lpIRC->Key, strCommand);
	strcpy(lpIRC->Message, strMsg);

	SendPost(m_SendPacket);
}

