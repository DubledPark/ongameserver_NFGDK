#include "NaveServer.h"
#include "NFMemPool.h"
#include "NFPacketPool.h"
#include <Nave/NFLog.h>

namespace NaveServer {

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : ������					 					//
	// [2]PARAMETER :												//
	// [3]RETURN :													//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	NFPacketPool::NFPacketPool()
	{
		m_hLogFile = NULL;											// ���� �ڵ� �ʱ�ȭ 
		m_pAcptPool = NULL;
		m_pRecvPool = NULL;
		m_pProcPool = NULL;											// ���� �ʱ�ȭ 

		m_nMaxPacketSize = 0;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : �Ҹ���  - �޸� �Ҵ� ���� 					//
	// [2]PARAMETER :												//
	// [3]RETURN :													//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	NFPacketPool::~NFPacketPool()
	{
		Release();
	}

	VOID NFPacketPool::Release()
	{
		LogClose();													// �α� ���� �ڵ� �ݱ� 

		m_nMaxPacketSize = 0;

		if(m_pAcptPool)
			delete m_pAcptPool;

		if(m_pRecvPool)
			delete m_pRecvPool;

		if(m_pProcPool)	
			delete m_pProcPool;

		m_pAcptPool = NULL;
		m_pRecvPool = NULL;
		m_pProcPool = NULL;											// ���� �ʱ�ȭ 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : �ʱ�ȭ �Լ�									//
	// [2]PARAMETER : lpszFileName - ���� �̸�, ������ NULL			//
	// [3]RETURN :	TRUE - ����ó�� FALSE - ���� 					//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::Create(INT nMaxUser,CHAR* lpszFileName,INT nMaxPacketSize,INT nAcptBuffCnt,INT nRecvBuffCnt,INT nSendBuffCnt)
	{
		// ������(�̸�)�� ���� �ϸ�..
		if(lpszFileName) LogOpen( lpszFileName );					// ���� ���� 

		m_nMaxPacketSize = nMaxPacketSize;

		// �� ��Ŷ�� �޸� �Ҵ� 
		// Winsock ��ü���� ���� �޸� ���� 64�� ���Ѵ� 
		// winsock���� ����ϴ� OVERLAPPED �����ϼ���.. 
		m_pAcptPool = new NFMemPool <OVERLAPPEDPLUS> (nMaxUser*nAcptBuffCnt, m_nMaxPacketSize+64);
		m_pRecvPool = new NFMemPool <OVERLAPPEDPLUS> (nMaxUser*nRecvBuffCnt, m_nMaxPacketSize+64);
		m_pProcPool = new NFMemPool <OVERLAPPEDPLUS> (nMaxUser*nSendBuffCnt, m_nMaxPacketSize+64);

		// ���� ���� 
		LogWrite("acpt=%08X, recv=%08X, proc=%08X", m_pAcptPool, m_pRecvPool, m_pProcPool);

		// �ϳ��� �޸� �Ҵ��� �ȵǾ��ٸ� 
		if( !m_pAcptPool || !m_pRecvPool || !m_pProcPool)
			return FALSE;											// FALSE  

		m_pAcptPool->SetLogTitle(L"ACPT");
		m_pRecvPool->SetLogTitle(L"RECV");
		m_pProcPool->SetLogTitle(L"SEND");

		return TRUE;												// TRUE  
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : ���� �ڵ� ����								//
	// [2]PARAMETER : lpszFileName - ���� �̸�						//
	// [3]RETURN :	TRUE - ����ó�� FALSE - ���� 					//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::LogOpen( CHAR *lpszFileName )
	{
		// �̸��� ������ �ʾҴٸ� 
		if( m_hLogFile || lpszFileName == NULL ) return FALSE;		// FALSE ���� 
		
		m_hLogFile = fopen( lpszFileName, "ab" );					// ó�� ���� ���� 

		// ���� �˻� 
		if( !m_hLogFile ) return FALSE;

		return TRUE;												// TRUE ���� - ���� ó�� 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : ���Ͽ� �����Ͽ� ����						//
	// [2]PARAMETER : lpszFileName - ���� �̸� ���ϱ� 				//
	// [3]RETURN :	TRUE - ����ó�� FALSE - ���� 					//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL __cdecl NFPacketPool::LogWrite( CHAR *lpszFmt, ... )
	{
		if(NULL == m_hLogFile) return FALSE;

		va_list		argptr;
		CHAR		szOutStr[1024];
		va_start(argptr, lpszFmt);
		vsprintf(szOutStr, lpszFmt, argptr);
		va_end(argptr);
		
		INT nBytesWritten = fprintf( m_hLogFile, "%s\r\n", szOutStr );// LOG ���� 
		fflush( m_hLogFile );

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : ���µ� ���� �ݱ� 							//
	// [2]PARAMETER : void											//
	// [3]RETURN :	void											//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	void NFPacketPool::LogClose()
	{
		// ���� �ڵ��� ���� �Ѵٸ�,
		if(m_hLogFile) fclose( m_hLogFile );						// ���� �ڵ� �ݱ� 
		m_hLogFile = NULL;											// ���� �ڵ� �ʱ�ȭ 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Accept Packet �Ҵ� 							//
	// [2]PARAMETER : newolp - ���� ���� 							//
	// [3]RETURN :	TRUE - ����ó�� 								//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::AllocAcptPacket( LPOVERLAPPEDPLUS &newolp )
	{
		LPOVERLAPPEDPLUS olp = NULL;								// ����ü ��ü ���� 
		BOOL bRet = FALSE;

		if(!m_pAcptPool)
			return bRet;												// ��ȯ 

		olp = m_pAcptPool->Alloc();									// Accept Pool���� �Ҵ� ���� 

		LogWrite( "AlocAcptPacket(%08X)", (DWORD)olp );				// logging

		// �Ҵ��� ����� �Ǿ��ٸ� 
		if(olp)
		{
			// �Ҵ� �޸� ó�� �κ� 
			newolp = olp;											// ���� ���� 
			newolp->wbuf.buf = (CHAR*)(olp+1);						// ���� ���� 
			bRet = TRUE;											// ��ȯ�� - TRUE�� �ٲ� 
		}

		return bRet;												// ��ȯ 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Receive Packet �Ҵ� 						//
	// [2]PARAMETER : newolp - ���� ���� 							//
	// [3]RETURN :	TRUE - ����ó�� 								//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::AllocRecvPacket( LPOVERLAPPEDPLUS &newolp )
	{
		LPOVERLAPPEDPLUS olp = NULL;								// ����ü ��ü ���� 
		BOOL bRet = FALSE;

		if(!m_pRecvPool)
			return bRet;												// ��ȯ 

		olp = m_pRecvPool->Alloc();									// Receive Pool���� �Ҵ� ����

		LogWrite( "AlocRecvPacket(%08X)", (DWORD)olp );				// logging

		// �Ҵ��� ����� �Ǿ��ٸ� 
		if(olp)
		{
			// �Ҵ� �޸� ó�� �κ�
			newolp = olp;											// ���� ���� 
			newolp->wbuf.buf = (CHAR*)(olp+1);						// ���� ���� 
			bRet   = TRUE;											// ��ȯ�� - TRUE�� �ٲ�
		}
		else
			LOG_ERROR((L"NFPacketPool::AllocRecvPacket Pool Alloc Faild.."));

		return bRet;												// ��ȯ
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Process Packet �Ҵ� 						//
	// [2]PARAMETER : newolp - ���� ���� 							//
	// [3]RETURN :	TRUE - ����ó�� 								//
	// [4]DATE : 2000�� 10�� 7��									//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::AllocSendPacket(LPOVERLAPPEDPLUS &newolp)
	{
		LPOVERLAPPEDPLUS olp = NULL;								// ����ü ��ü ���� 
		BOOL bRet = FALSE;

		if(!m_pProcPool)
			return bRet;												// ��ȯ 

		olp = m_pProcPool->Alloc();									// Accept Pool���� �Ҵ� ���� 

		LogWrite( "AlocAcptPacket(%08X)", (DWORD)olp );				// logging

		// �Ҵ��� ����� �Ǿ��ٸ� 
		if(olp)
		{
			// �Ҵ� �޸� ó�� �κ� 
			newolp = olp;											// ���� ���� 
			newolp->wbuf.buf = (CHAR*)(olp+1);						// ���� ���� 
			bRet   = TRUE;											// ��ȯ�� - TRUE�� �ٲ� 
		}
		return bRet;												// ��ȯ 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Process Buffer �Ҵ� 						//
	// [2]PARAMETER : newolp - ���� ���� 							//
	// [3]RETURN :	TRUE - ����ó�� 								//
	// [4]DATE : 2000�� 10�� 7��									//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::AllocProcBuffer(PCHAR &newbuf)
	{
		LPOVERLAPPEDPLUS olp = NULL;								// ����ü ��ü ���� 
		BOOL bRet = FALSE;

		if(!m_pProcPool)
			return bRet;												// ��ȯ 

		olp = m_pProcPool->Alloc();									// Accept Pool���� �Ҵ� ���� 

		LogWrite( "AllocProcBuffer(%08X)", (DWORD)olp );				// logging

		// �Ҵ��� ����� �Ǿ��ٸ� 
		if(olp)
		{
			// �Ҵ� �޸� ó�� �κ� 
			newbuf = (CHAR*)(olp+1);								// ���� ���� 
			bRet   = TRUE;											// ��ȯ�� - TRUE�� �ٲ� 
		}
		return bRet;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Accept �Ҵ� ��Ŷ ����  						//
	// [2]PARAMETER : olp - ������ ��Ŷ 							//
	// [3]RETURN :	TRUE - ���� ó�� 								//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::FreeAcptPacket( LPOVERLAPPEDPLUS olp )
	{
		LogWrite( "FreeAcptPacket(%08X)", (DWORD)olp );				// Logging

		if(!m_pAcptPool)
			return FALSE;												// ��ȯ 

		return(m_pAcptPool->Free(olp));								// ��Ŷ ���� �� ��� ��ȯ 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Receive �Ҵ� ��Ŷ ����  					//
	// [2]PARAMETER : olp - ������ ��Ŷ 							//
	// [3]RETURN :	TRUE - ���� ó�� 								//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::FreeRecvPacket( LPOVERLAPPEDPLUS olp )
	{
		LogWrite( "FreeRecvPacket(%08X)", (DWORD)olp );				// Logging

		if(!m_pRecvPool)
			return FALSE;												// ��ȯ 

		return( m_pRecvPool->Free(olp) );							// ��Ŷ ���� �� ��� ��ȯ						
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Process �Ҵ� ��Ŷ ����  					//
	// [2]PARAMETER : olp - ������ ��Ŷ 							//
	// [3]RETURN :	TRUE - ���� ó�� 								//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::FreeSendPacket(LPOVERLAPPEDPLUS olp)
	{
		LogWrite( "FreeProcPacket(%08X)", (DWORD)olp );				// Logging

		if(!m_pProcPool)
			return FALSE;												// ��ȯ 

		return(m_pProcPool->Free(olp));								// ��Ŷ ���� �� ��� ��ȯ
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Process �Ҵ� ����  ����  					//
	// [2]PARAMETER : olp - ������ ��Ŷ 							//
	// [3]RETURN :	TRUE - ���� ó�� 								//
	// [4]DATE : 2000�� 9�� 4��										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::FreeProcBuffer(PCHAR buffer)
	{
		LPOVERLAPPEDPLUS olp;
		olp = ((LPOVERLAPPEDPLUS)buffer) - 1;
		LogWrite( "FreeProcBuffer(%08X)", (DWORD)olp );				// Logging

		if(!m_pProcPool)
			return FALSE;												// ��ȯ 

		return(m_pProcPool->Free(olp));								// ��Ŷ ���� �� ��� ��ȯ
	}

}