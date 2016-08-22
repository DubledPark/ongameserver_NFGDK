/** 
 *  @file  		NFConnection.h
 *  @brief 		하나의 Connection을 관리하는 객체이다.
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

#include "NFMemPool.h"
#include "NFPacketPool.h"
#include <NaveNet/NFIOBuffer.h>
#include <Nave/NFSync.h>

namespace NaveServer {


	/** 
	 *  @class        NFConnection
	 *  @brief        하나의 소켓객체를 나타낸다 1개의 클라이언트를 나타낸다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-08-28
	 */
	class NFConnection  
	{
	public:
		NFConnection();
		virtual ~NFConnection();

	public:
		// 이 객체 생성 
		/**
		 * @brief	멤버 변수 세팅 및 객체 초기화
		 * @param dwIndex		객체 고유 ID
		 * @param hIOCP			IOCP Handle
		 * @param listener		Listen Socket
		 * @param pPacketPool	패킷 Pool 포인터
		 * @param nMaxBuf		최대 버퍼크기
		 * @return 성공여부
		 */
		virtual BOOL		Create( DWORD			dwIndex,
									HANDLE			hIOCP,
									SOCKET			listener,
									NFPacketPool*	pPacketPool,
									INT				nMaxBuf);

		/**
		 * @brief	패킷을 업데이트처리 합니다. 
		 * @param Packet 업데이트 할 패킷정보
		 */
		virtual VOID		UpdatePacket(NaveNet::NFPacket& Packet);

		/**
		 * @brief	연결 강제 종료 
		 * @param bForce TRUE면 소켓에 남은 데이터 정리
		 */
		VOID				Disconnect( BOOL bForce = FALSE );		

		/**
		 * @brief	내부 변수등을 정리합니다.
		 */
		virtual VOID		Clear() { return; };

		/**
		 * @brief	객체의 클라이언트 연결 해제 및 초기화
		 * @param bForce TRUE면 소켓에 남은 데이터 정리
		 * @return  성공여부
		 */
		virtual BOOL		Close_Open( BOOL bForce = FALSE );

		/**
		 * @brief	IOCP 처리 핸들링
		 * @param lpOverlapPlus	처리할 패킷
		 * @return 
		 */
		virtual BOOL		DoIo( LPOVERLAPPEDPLUS lpOverlapPlus );	

		/**
		 * @brief	패킷 전송
		 * @param pPackte 보낼 패킷 포인터
		 * @param Len 보낼 패킷 크기
		 * @return  성공여부
		 */
		virtual BOOL		SendPost( CHAR* pPackte, INT Len );	

		/**
		 * @brief	패킷 전송
		 * @param Packet 보낼 패킷 구조체
		 * @return  성공여부
		 */
		virtual BOOL		SendPost( NaveNet::NFPacket&	Packet);

		/**
		 * @brief	연결자의 IP얻기 
		 * @param iIP 정수형 4개 배열
		 * @return 
		 */
		BOOL				GetClientIP( INT* iIP );					 
		/**
		 * @brief	연결자의 IP얻기 
		 * @param szIP 문자형
		 * @return 
		 */
		BOOL				GetClientIP( CHAR* szIP );				
		/**
		 * @brief	연결자의 IP얻기 
		 * @param szIP 유니코드 문자형
		 * @return 
		 */
		BOOL				GetClientIP( WCHAR* szIP );				
		/**
		 * @brief	연결자의 IP얻기 
		 * @param addr sockaddr_in 형식
		 * @return 
		 */
		BOOL				GetClientIP( sockaddr_in& addr );			

		/**
		 * @brief	연결개체의 고유 인덱스를 설정합니다.
		 * @param iIndex 연결개체의 고유 인덱스
		 */
		inline VOID			SetIndex(INT iIndex) { m_dwIndex = iIndex; };

		/**
		 * @brief	연결개체의 고유 인덱스 얻기
		 * @return  인덱스값
		 */
		inline DWORD		GetIndex() { return m_dwIndex; }

		/**
		 * @brief	수신 Tick 얻기
		 * @return  수신 Tick 카운트
		 */
		LONG				GetRecvTickCnt();

		/**
		 * @brief	연결 객체의 활성화 상태 얻기
		 * @return  활성화 상태
		 */
		BOOL				IsConnect();

		/**
		 * @brief	연결 객체의 상태를 설정합니다.
		 * @param eState	연결 객체의 상태 정보 
		 */
		void				SetConnectFlag(CONNECT_EVENT eState);

		/**
		 * @brief	전달된 Overlapped객체를 이용해 연결 객체를 종료시킨후 초기화 시킵니다.
		 * @param lpOverlapPlus Overlapped 객체
		 * @param bForce   TRUE면 소켓에 남은 데이터 정리
		 */
		void				SetClose_Open(LPOVERLAPPEDPLUS lpOverlapPlus, BOOL bForce=FALSE );

		//------------------------ 멤 버 함 수 -------------------------//
	protected:

		/**
		 * @brief	객체 초기화, 리슨 소켓 결합					
		 * @return  성공여부
		 */
		virtual BOOL		Open();

		/**
		 * @brief	Socket과 IOCP 바인딩 작업
		 * @param lpOverlapPlus 바인딩 시킬 Overlapped 구조체
		 * @return 
		 */
		BOOL				BindIOCP( LPOVERLAPPEDPLUS lpOverlapPlus );

		/**
		 * @brief	Accept패킷 할당
		 * @return  할당된 Overlapped 구조체 
		 */
		LPOVERLAPPEDPLUS	PrepareAcptPacket();

		/**
		 * @brief	IOCP를 이용한 Recv Overlapped 셋팅
		 * @param buflen 받을 크기
		 * @return  할당된 Overlapped 구조체 
		 */
		LPOVERLAPPEDPLUS	PrepareRecvPacket(UINT buflen);

		/**
		 * @brief	IOCP를 이용한 Send Overlapped 셋팅
		 * @param *psrcbuf 보낼 데이터 포인터
		 * @param srclen   보낼 크기
		 * @return  할당된 Overlapped 구조체 
		 */
		LPOVERLAPPEDPLUS	PrepareSendPacket(CHAR *psrcbuf, UINT srclen);

		/**
		 * @brief	Accept 패킷 해제  							
		 * @param lpOverlapPlus 해제할 Overlapped구조체
		 * @return  성공여부
		 */
		BOOL				ReleaseAcptPacket(LPOVERLAPPEDPLUS lpOverlapPlus);

		/**
		 * @brief	Receive 패킷 해제  							
		 * @param lpOverlapPlus 해제할 Overlapped구조체
		 * @return  성공여부
		 */
		BOOL				ReleaseRecvPacket(LPOVERLAPPEDPLUS lpOverlapPlus);

		/**
		 * @brief	Send 패킷 해제
		 * @param lpOverlapPlus 해제할 Overlapped구조체
		 * @return  성공여부
		 */
		BOOL				ReleaseSendPacket(LPOVERLAPPEDPLUS lpOverlapPlus);

		/**
		 * @brief	실제 Receive 처리
		 * @param buflen  보낼 버퍼 사이즈
		 * @return  성공여부
		 */
		BOOL				RecvPost(UINT buflen=0);

		/**
		 * @brief	로그에 메시지를 출력합니다.
		 * @param MsgIndex 메시지 아이디
		 */
		virtual VOID		ShowMessage(INT MsgIndex);

		/**
		 * @brief	 연결자에서 Connect 이벤트 발생
		 * @param bConnect Connect 성공여부
		 */
		virtual VOID		OnConnect(BOOL bConnect) { };

		/// Disconnect 이벤트 발생
		virtual	VOID		OnDisconnect() { };

		// 내부 처리 전송 함수 
		/**
		 * @brief	메세지 저장 및 Receive 신호 발생, Socket정리
		 * @param lpOverlapPlus Overlapped 구조체
		 * @return  Recv 처리 성공 여부 
		 */
		virtual BOOL		DispatchPacket( LPOVERLAPPEDPLUS lpOverlapPlus );

		/**
		 * @brief	실제 패킷을 처리하는 부분이다
		 * @param Packet 처리할 패킷 구조체
		 */
		virtual VOID		DispatchPacket( NaveNet::NFPacket& Packet ) 
		{
			// 상속받아서 패킷을 처리할때는 아래와 같이 세션 변수를 등록해서 사용한다.
			//Nave::Sync::CSSync Live(&m_Sync);
		};

	protected:
		/// 최대 버퍼 사이즈 
		INT					m_nMaxBuf;							

		/// 최대 패킷 사이즈
		INT					m_nMaxPacketSize;						

		/// 클라이언트 연결 소켓 
		SOCKET				m_Socket;								
		
		/// Listener 소켓 
		SOCKET				m_sckListener;							

		/// 서버 정보를 가지고 있는 객체 
		sockaddr_in			m_Local;	

		/// 클라이언트 정보를 가지고 있는 객체 
		sockaddr_in			m_Peer;									
		
		/// 이 클라이언트 연결 객체의 번호 
		INT					m_dwIndex;								

		/// 보낸 바이트 검사 변수 
		INT					m_nBytesSent;							

		/// IOCP 핸들 	
		HANDLE				m_hIOCP;								

		//이 클래스는 IOCP에서 패킷을 주고 받을때 Socket에 직접 연결되는 버퍼를 관리한다.
		/// 패킷풀 포인터 
		NFPacketPool*		m_pPacketPool;							

		/// 이 객체가 클라이언트와 연결 검사
		BOOL				m_bIsConnect;							
		
		/// 수신 Tick Count 
		LONG				m_uRecvTickCnt;							

		/// 커넥션 상태 변수
		CONNECT_EVENT		m_eConnectFlag;							

		/// 소켓 버퍼 초기화 상태 변수
		BOOL				m_bForce;								

		/// Sync 객체
		Nave::NFSync		m_Lock;

		// checksum 처리 ///////////////////////////////////////////// 
		// CPacketIOBuffer은 Recv받은 패킷 정보(이땐 패킷풀의 버퍼에 있다)를 저장하는 IOBuffer 이다.
		// IOBuffer을 사용하는 이유는 Recv 받은 패킷이 완전한 패킷이 아닐때 처리하기 위한 하나의
		// 방편이다.
		
		/// Recv의 IOBuffer
		NaveNet::NFPacketIOBuffer		m_RecvIO;								
		
		// CPacket는 하나의 패킷을 관리하는 클래스로 IOBuffer 에서 패킷을 꺼내 저장할때 사용된다.
		// Packet에는 패킷 버퍼가 배열로 정의되어 있다. (memcpy를 사용하기 때문에 오버로드가 발생한다.)
		/// Recv를 하기위해 사용하는 임시 패킷 변수
		NaveNet::NFPacket				m_RecvPacket;
	};

}