#include "NaveServer.h"
#include "NFConnectionManager.h"
#include <Nave/NFLog.h>

namespace NaveServer {

	NFConnectionManager::NFConnectionManager(VOID) : m_iCount(0), m_iMaxCount(0)
	{
		m_vecConn.clear();
	}

	NFConnectionManager::~NFConnectionManager(VOID)
	{
		m_vecConn.clear();
	}

	VOID NFConnectionManager::Init(INT iMaxCount)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		m_iCount = 0;
		m_iMaxCount = iMaxCount;
		m_vecConn.reserve(iMaxCount);
		for(INT i = 0; i < m_iMaxCount; ++i)
		{
			m_vecConn.push_back(NULL);
		}
	}	

	VOID NFConnectionManager::SendPostAll( CHAR* pPackte, INT Len )		// 실제 Send 처리
	{
		for(INT i = 0; i < m_iMaxCount; ++i)
		{
			if(!m_vecConn[i])
				continue;

			m_vecConn[i]->SendPost(pPackte, Len);
		}
	}

	VOID NFConnectionManager::SendPostAll( NaveNet::NFPacket& Packet)
	{
		Packet.EncryptPacket();

		INT Len = Packet.m_Header.Size;
		CHAR* pPacket = (CHAR*)&Packet;

		SendPostAll(pPacket, Len);
	}


	VOID NFConnectionManager::Draw()
	{
		Nave::NFSyncLock Sync(&m_Lock);

		INT Count = 0;
		for(INT i = 0; i < m_iMaxCount; ++i)
		{
			if(m_vecConn[i])
				Count++;
		}

		LOG_ERROR((L"Leave 되지 않은 Count : %d", Count));
	}

	// Connect, Disconnect 를 관리하는 함수.
	VOID NFConnectionManager::Join(NFConnection* pUser)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		if(!pUser)
			return;

		INT iIndex = pUser->GetIndex();
		if(m_vecConn[iIndex] == NULL)
		{
			m_vecConn[iIndex] = pUser;
			++m_iCount;
		}
		else
		{
			LOG_ERROR((L"[%04d] 중복 Join호출", iIndex));
		}
	}

	VOID NFConnectionManager::Levae(INT iIndex)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		if(m_vecConn[iIndex])
		{
			--m_iCount;
			m_vecConn[iIndex] = NULL;
		}
		else
		{
			LOG_ERROR((L"[%04d] 중복 Leave호출 Socket : %d", iIndex, WSAGetLastError()));
		}
	}

}