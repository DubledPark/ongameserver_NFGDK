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

	VOID NFConnectionManager::SendPostAll( CHAR* pPackte, INT Len )		// ���� Send ó��
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

		LOG_ERROR((L"Leave ���� ���� Count : %d", Count));
	}

	// Connect, Disconnect �� �����ϴ� �Լ�.
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
			LOG_ERROR((L"[%04d] �ߺ� Joinȣ��", iIndex));
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
			LOG_ERROR((L"[%04d] �ߺ� Leaveȣ�� Socket : %d", iIndex, WSAGetLastError()));
		}
	}

}