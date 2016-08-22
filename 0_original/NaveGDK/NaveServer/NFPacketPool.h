/** 
 *  @file  		NFPacketPool.h
 *  @brief 		Packet Pool ������ü, �̰����� Buffer�� �Ҵ��ϰų� �����Ѵ�.
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
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

		BOOL Create(INT nMaxUse			= DEF_MAXUSER,				// �ִ� ����
					CHAR* lpszFileName	= NULL,						// �α� ���� �̸�
					INT nMaxPacketSize	= DEF_MAXPACKETSIZE,
					INT nAcptBuffCnt    = 1,
					INT nRecvBuffCnt    = 8,
					INT nSendBuffCnt    = 16);						// Create Packet Pool

		VOID Release();

		// Operations
		BOOL AllocAcptPacket(LPOVERLAPPEDPLUS &newolp);				// Accept ��Ŷ ���� 
		BOOL AllocRecvPacket(LPOVERLAPPEDPLUS &newolp);				// Receive ��Ŷ ���� 
		BOOL AllocSendPacket(LPOVERLAPPEDPLUS &newolp);				// Send Buffer ���� 
		BOOL AllocProcBuffer(PCHAR &newbuf);						// Send Buffer ����

		BOOL FreeAcptPacket(LPOVERLAPPEDPLUS olp);					// Accept ��Ŷ ���� 
		BOOL FreeRecvPacket(LPOVERLAPPEDPLUS olp);					// Receive ��Ŷ ���� 
		BOOL FreeSendPacket(LPOVERLAPPEDPLUS olp);					// Send Buffer ����
		BOOL FreeProcBuffer(PCHAR buffer);							// Send Buffer ����

	private:
		BOOL				LogOpen(CHAR *lpszFileName);			// �α� ���� ���� 
		BOOL	__cdecl		LogWrite(CHAR *lpszFmt, ...);			// �α� ���� ����� 
		VOID				LogClose();								// �α� ���� �ݱ� 

	//---------------------- Member Variables ----------------------//
	private:
		FILE*				m_hLogFile;								// �α� ������ ����� ���� ���� �ڵ� ��ü 

		NFMemPool			<OVERLAPPEDPLUS>* m_pAcptPool;			// Accept Packet
		NFMemPool			<OVERLAPPEDPLUS>* m_pRecvPool;			// Receive Packet
		NFMemPool			<OVERLAPPEDPLUS>* m_pProcPool;			// Process Packet

		INT					m_nMaxPacketSize;						// ��Ŷ ������ ���Ѱ�
	};

}