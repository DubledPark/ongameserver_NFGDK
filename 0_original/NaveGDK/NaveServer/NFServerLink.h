/** 
 *  @file  		NFServerLink.h
 *  @brief 		Server To Server 용 Connect 객체
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

namespace NaveServer {

	// NFServerLink는 Server가 NFServerCtrl로 Connection을 하기위해 제작된 구조체이다
	// NFServerLink을 선언후 Create로 Server에 Connection하게 한다.

	class NFServerLink : public NFConnection
	{
	public:
		NFServerLink(VOID);
		virtual ~NFServerLink(VOID);

	public:
		virtual VOID		Clear() { return; };

		virtual BOOL		Close_Open(BOOL bForce=FALSE );

		// 이 객체 생성 
		virtual BOOL		Create(	DWORD			dwIndex,
									HANDLE			hIOCP,
									NFPacketPool*	pPacketPool,
									WCHAR*			sIP,
									INT				nPort,
									INT				nMaxBuf);

		virtual BOOL		SendOpenPacket() { return TRUE; };

	protected:
		// Socket과 리스너 연결 및 스탠바이 상태 만듬  
		virtual BOOL		Open();

		// 내부 처리 전송 함수 
		virtual INT			DispatchPacket( LPOVERLAPPEDPLUS lpOverlapPlus );

		// 실제 패킷을 처리하는 부분이다.
		virtual VOID		DispatchPacket( NaveNet::NFPacket& Packet ) 
		{
			// Nave::Sync::CSSync Live(&m_Sync);
		};

	protected:
		WCHAR	m_strIP[32];
		INT		m_iPort;
	};

}