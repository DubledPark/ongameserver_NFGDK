/** 
 *  @file  		NFPacketPool.h
 *  @brief 		Packet Pool 관리객체, 이곳에서 Buffer를 할당하거나 해제한다.
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

namespace NaveServer {

	//////////////////////////////////////////////////////////////////////////////////////////////
	// NFPacketPool Class
	//////////////////////////////////////////////////////////////////////////////////////////////
	class NFPacketPool  
	{
	//---------------------- Member Functions ----------------------//
	public:
		NFPacketPool();												// Contructor 
		virtual ~NFPacketPool();										// Destructor 

		BOOL Create(INT nMaxUse			= DEF_MAXUSER,				// 최대 유저
					CHAR* lpszFileName	= NULL,						// 로그 파일 이름
					INT nMaxPacketSize	= DEF_MAXPACKETSIZE,
					INT nAcptBuffCnt    = 1,
					INT nRecvBuffCnt    = 8,
					INT nSendBuffCnt    = 16);						// Create Packet Pool

		VOID Release();

		// Operations
		BOOL AllocAcptPacket(LPOVERLAPPEDPLUS &newolp);				// Accept 패킷 생성 
		BOOL AllocRecvPacket(LPOVERLAPPEDPLUS &newolp);				// Receive 패킷 생성 
		BOOL AllocSendPacket(LPOVERLAPPEDPLUS &newolp);				// Send Buffer 생성 
		BOOL AllocProcBuffer(PCHAR &newbuf);						// Send Buffer 생성

		BOOL FreeAcptPacket(LPOVERLAPPEDPLUS olp);					// Accept 패킷 해제 
		BOOL FreeRecvPacket(LPOVERLAPPEDPLUS olp);					// Receive 패킷 해제 
		BOOL FreeSendPacket(LPOVERLAPPEDPLUS olp);					// Send Buffer 해제
		BOOL FreeProcBuffer(PCHAR buffer);							// Send Buffer 해제

	private:
		BOOL				LogOpen(CHAR *lpszFileName);			// 로그 파일 열기 
		BOOL	__cdecl		LogWrite(CHAR *lpszFmt, ...);			// 로그 파일 만들기 
		VOID				LogClose();								// 로그 파일 닫기 

	//---------------------- Member Variables ----------------------//
	private:
		FILE*				m_hLogFile;								// 로그 파일을 만들기 위한 파일 핸들 객체 

		NFMemPool			<OVERLAPPEDPLUS>* m_pAcptPool;			// Accept Packet
		NFMemPool			<OVERLAPPEDPLUS>* m_pRecvPool;			// Receive Packet
		NFMemPool			<OVERLAPPEDPLUS>* m_pProcPool;			// Process Packet

		INT					m_nMaxPacketSize;						// 패킷 사이즈 정한것
	};

}