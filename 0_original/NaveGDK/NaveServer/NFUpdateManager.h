/** 
 *  @file  		NFUpdateManager.h
 *  @brief 		�Ѱ��� �ϼ��� ��Ŷ�� �޾����� �ش���Ŷ�� ������Ʈ �ϴ� ����ť
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

#include <Nave/NFSingleton.h>
#include <Nave/NFSync.h>
#include <NaveNet/NFPacket.h>
#include "NFConnection.h"

namespace NaveServer {

	class NFUpdateManager : public Nave::NFSingleton<NFUpdateManager>
	{
	public:
		NFUpdateManager(VOID);
		~NFUpdateManager(VOID);

	public:
		VOID Add(NFConnection* pUser, NaveNet::NFPacket* pPacket);
		VOID Update();

		VOID Draw();

	private:
		enum {
			// �ִ� 1024 ���� ��Ŷ�� ������ �� �ִ�.
			MaxQueCount = 1024,
		};

		// ����ť ���.
		INT						m_Head;
		INT						m_Tail;
		NFConnection*			m_Conn[MaxQueCount];
		NaveNet::NFPacket		m_Packet[MaxQueCount];

		/// Sync ��ü
		Nave::NFSync								m_Lock;
	};

}