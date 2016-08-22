/** 
 *  @file  		NFUpdateManager.h
 *  @brief 		한개의 완성된 패킷을 받았을때 해당패킷을 업데이트 하는 원형큐
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
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
			// 최대 1024 개의 패킷을 저장할 수 있다.
			MaxQueCount = 1024,
		};

		// 원형큐 대용.
		INT						m_Head;
		INT						m_Tail;
		NFConnection*			m_Conn[MaxQueCount];
		NaveNet::NFPacket		m_Packet[MaxQueCount];

		/// Sync 객체
		Nave::NFSync								m_Lock;
	};

}