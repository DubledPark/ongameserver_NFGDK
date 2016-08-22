#include "NaveServer.h"
#include "NFUpdateManager.h"

namespace NaveServer {

//	INT				iMaxProcPacket = 0;
//	INT				iProcPacket = 0;								// 이 객체가 클라이언트와 연결 검사
//	DWORD			uProcTickCnt = 0;								// 수신 Tick Count 

	NFUpdateManager::NFUpdateManager(VOID) : m_Head(0), m_Tail(0)
	{
	}

	NFUpdateManager::~NFUpdateManager(VOID)
	{
	}

	VOID NFUpdateManager::Draw()
	{
		Nave::NFSyncLock Sync(&m_Lock);

		INT Tail = m_Tail;
		INT S = Tail-m_Head;
		if(Tail < m_Head)
			S = (MaxQueCount-m_Head)+Tail;

		LOG_IMPORTANT((L"UpdateManager PacketCount : %d", S));
	}

	VOID NFUpdateManager::Update()
	{
		Nave::NFSyncLock Sync(&m_Lock);

		INT i;
		INT Tail = m_Tail;
/*
		if(timeGetTime() - uProcTickCnt > 1000)
		{
			InterlockedExchange((LONG*)&iMaxProcPacket,iProcPacket);
			InterlockedExchange((LONG*)&iProcPacket,0);
			InterlockedExchange((LONG*)&uProcTickCnt,timeGetTime());
		}
*/
		// 없다.
		if(m_Head == Tail)
			return;

		INT iCount = 0;
		// Tail이 앞에 있다.
		if(Tail < m_Head)
		{
			for(i = m_Head; i < MaxQueCount; ++i)
			{
				if(m_Conn[i] == NULL)
					continue;

				m_Conn[i]->UpdatePacket(m_Packet[i]);
				m_Conn[i] = NULL;
				iCount++;
			}
			for(i = 0; i < Tail; ++i)
			{
				if(m_Conn[i] == NULL)
					continue;

				m_Conn[i]->UpdatePacket(m_Packet[i]);
				m_Conn[i] = NULL;
				iCount++;
			}
		}
		else // Tail이 뒤에있다.
		{
			for(i = m_Head; i < Tail; ++i)
			{
				if(m_Conn[i] == NULL)
					continue;

				m_Conn[i]->UpdatePacket(m_Packet[i]);
				m_Conn[i] = NULL;
				iCount++;
			}

		}
//		InterlockedExchange((LONG*)&iProcPacket,iProcPacket+iCount);

		m_Head = Tail;
	}

	VOID NFUpdateManager::Add(NFConnection* pUser, NaveNet::NFPacket* pPacket)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		if(!pUser)
			return;

		INT Tail = m_Tail;
		m_Conn[Tail] = pUser;
		
		if(pPacket)
			memcpy(&m_Packet[Tail], pPacket, sizeof(NaveNet::NFPacket));
		else
			m_Packet[Tail].Init();

		if(Tail+1 >= MaxQueCount)
			m_Tail = 0;
		else 
			m_Tail = Tail+1;

//		if(m_Tail == m_Head)
//			NAVE_ERROR(("UpdateManager Overflow. (H:%d, T:%d)", m_Head, m_Tail));
	}

}