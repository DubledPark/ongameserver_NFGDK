#include "NaveServer.h"
#include "NFMemPool.h"
#include "NFPacketPool.h"
#include <Nave/NFLog.h>

namespace NaveServer {

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 생성자					 					//
	// [2]PARAMETER :												//
	// [3]RETURN :													//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	NFPacketPool::NFPacketPool()
	{
		m_hLogFile = NULL;											// 파일 핸들 초기화 
		m_pAcptPool = NULL;
		m_pRecvPool = NULL;
		m_pProcPool = NULL;											// 변수 초기화 

		m_nMaxPacketSize = 0;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 소멸자  - 메모리 할당 해제 					//
	// [2]PARAMETER :												//
	// [3]RETURN :													//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	NFPacketPool::~NFPacketPool()
	{
		Release();
	}

	VOID NFPacketPool::Release()
	{
		LogClose();													// 로그 파일 핸들 닫기 

		m_nMaxPacketSize = 0;

		if(m_pAcptPool)
			delete m_pAcptPool;

		if(m_pRecvPool)
			delete m_pRecvPool;

		if(m_pProcPool)	
			delete m_pProcPool;

		m_pAcptPool = NULL;
		m_pRecvPool = NULL;
		m_pProcPool = NULL;											// 변수 초기화 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 초기화 함수									//
	// [2]PARAMETER : lpszFileName - 파일 이름, 없으면 NULL			//
	// [3]RETURN :	TRUE - 정상처리 FALSE - 실패 					//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::Create(INT nMaxUser,CHAR* lpszFileName,INT nMaxPacketSize,INT nAcptBuffCnt,INT nRecvBuffCnt,INT nSendBuffCnt)
	{
		// 포인터(이름)가 존재 하면..
		if(lpszFileName) LogOpen( lpszFileName );					// 파일 오픈 

		m_nMaxPacketSize = nMaxPacketSize;

		// 각 패킷열 메모리 할당 
		// Winsock 자체에서 쓰는 메모리 땜에 64를 더한다 
		// winsock에서 사용하는 OVERLAPPED 때문일수도.. 
		m_pAcptPool = new NFMemPool <OVERLAPPEDPLUS> (nMaxUser*nAcptBuffCnt, m_nMaxPacketSize+64);
		m_pRecvPool = new NFMemPool <OVERLAPPEDPLUS> (nMaxUser*nRecvBuffCnt, m_nMaxPacketSize+64);
		m_pProcPool = new NFMemPool <OVERLAPPEDPLUS> (nMaxUser*nSendBuffCnt, m_nMaxPacketSize+64);

		// 파일 쓰기 
		LogWrite("acpt=%08X, recv=%08X, proc=%08X", m_pAcptPool, m_pRecvPool, m_pProcPool);

		// 하나라도 메모리 할당이 안되었다면 
		if( !m_pAcptPool || !m_pRecvPool || !m_pProcPool)
			return FALSE;											// FALSE  

		m_pAcptPool->SetLogTitle(L"ACPT");
		m_pRecvPool->SetLogTitle(L"RECV");
		m_pProcPool->SetLogTitle(L"SEND");

		return TRUE;												// TRUE  
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 파일 핸들 열기								//
	// [2]PARAMETER : lpszFileName - 파일 이름						//
	// [3]RETURN :	TRUE - 정상처리 FALSE - 실패 					//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::LogOpen( CHAR *lpszFileName )
	{
		// 이름을 정하지 않았다면 
		if( m_hLogFile || lpszFileName == NULL ) return FALSE;		// FALSE 리턴 
		
		m_hLogFile = fopen( lpszFileName, "ab" );					// 처리 파일 오픈 

		// 에러 검사 
		if( !m_hLogFile ) return FALSE;

		return TRUE;												// TRUE 리턴 - 정상 처리 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 파일에 연결하여 쓰기						//
	// [2]PARAMETER : lpszFileName - 파일 이름 정하기 				//
	// [3]RETURN :	TRUE - 정상처리 FALSE - 실패 					//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL __cdecl NFPacketPool::LogWrite( CHAR *lpszFmt, ... )
	{
		if(NULL == m_hLogFile) return FALSE;

		va_list		argptr;
		CHAR		szOutStr[1024];
		va_start(argptr, lpszFmt);
		vsprintf(szOutStr, lpszFmt, argptr);
		va_end(argptr);
		
		INT nBytesWritten = fprintf( m_hLogFile, "%s\r\n", szOutStr );// LOG 내용 
		fflush( m_hLogFile );

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 오픈된 파일 닫기 							//
	// [2]PARAMETER : void											//
	// [3]RETURN :	void											//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	void NFPacketPool::LogClose()
	{
		// 파일 핸들이 존재 한다면,
		if(m_hLogFile) fclose( m_hLogFile );						// 파일 핸들 닫기 
		m_hLogFile = NULL;											// 파일 핸들 초기화 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Accept Packet 할당 							//
	// [2]PARAMETER : newolp - 참조 변수 							//
	// [3]RETURN :	TRUE - 정상처리 								//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::AllocAcptPacket( LPOVERLAPPEDPLUS &newolp )
	{
		LPOVERLAPPEDPLUS olp = NULL;								// 구조체 객체 생성 
		BOOL bRet = FALSE;

		if(!m_pAcptPool)
			return bRet;												// 반환 

		olp = m_pAcptPool->Alloc();									// Accept Pool에설 할당 받음 

		LogWrite( "AlocAcptPacket(%08X)", (DWORD)olp );				// logging

		// 할당이 제대로 되었다면 
		if(olp)
		{
			// 할당 메모리 처리 부분 
			newolp = olp;											// 변수 세팅 
			newolp->wbuf.buf = (CHAR*)(olp+1);						// 버퍼 세팅 
			bRet = TRUE;											// 반환값 - TRUE로 바꿈 
		}

		return bRet;												// 반환 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Receive Packet 할당 						//
	// [2]PARAMETER : newolp - 참조 변수 							//
	// [3]RETURN :	TRUE - 정상처리 								//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::AllocRecvPacket( LPOVERLAPPEDPLUS &newolp )
	{
		LPOVERLAPPEDPLUS olp = NULL;								// 구조체 객체 생성 
		BOOL bRet = FALSE;

		if(!m_pRecvPool)
			return bRet;												// 반환 

		olp = m_pRecvPool->Alloc();									// Receive Pool에설 할당 받음

		LogWrite( "AlocRecvPacket(%08X)", (DWORD)olp );				// logging

		// 할당이 제대로 되었다면 
		if(olp)
		{
			// 할당 메모리 처리 부분
			newolp = olp;											// 변수 세팅 
			newolp->wbuf.buf = (CHAR*)(olp+1);						// 버퍼 세팅 
			bRet   = TRUE;											// 반환값 - TRUE로 바꿈
		}
		else
			LOG_ERROR((L"NFPacketPool::AllocRecvPacket Pool Alloc Faild.."));

		return bRet;												// 반환
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Process Packet 할당 						//
	// [2]PARAMETER : newolp - 참조 변수 							//
	// [3]RETURN :	TRUE - 정상처리 								//
	// [4]DATE : 2000년 10월 7일									//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::AllocSendPacket(LPOVERLAPPEDPLUS &newolp)
	{
		LPOVERLAPPEDPLUS olp = NULL;								// 구조체 객체 생성 
		BOOL bRet = FALSE;

		if(!m_pProcPool)
			return bRet;												// 반환 

		olp = m_pProcPool->Alloc();									// Accept Pool에설 할당 받음 

		LogWrite( "AlocAcptPacket(%08X)", (DWORD)olp );				// logging

		// 할당이 제대로 되었다면 
		if(olp)
		{
			// 할당 메모리 처리 부분 
			newolp = olp;											// 변수 세팅 
			newolp->wbuf.buf = (CHAR*)(olp+1);						// 버퍼 세팅 
			bRet   = TRUE;											// 반환값 - TRUE로 바꿈 
		}
		return bRet;												// 반환 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Process Buffer 할당 						//
	// [2]PARAMETER : newolp - 참조 변수 							//
	// [3]RETURN :	TRUE - 정상처리 								//
	// [4]DATE : 2000년 10월 7일									//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::AllocProcBuffer(PCHAR &newbuf)
	{
		LPOVERLAPPEDPLUS olp = NULL;								// 구조체 객체 생성 
		BOOL bRet = FALSE;

		if(!m_pProcPool)
			return bRet;												// 반환 

		olp = m_pProcPool->Alloc();									// Accept Pool에설 할당 받음 

		LogWrite( "AllocProcBuffer(%08X)", (DWORD)olp );				// logging

		// 할당이 제대로 되었다면 
		if(olp)
		{
			// 할당 메모리 처리 부분 
			newbuf = (CHAR*)(olp+1);								// 버퍼 세팅 
			bRet   = TRUE;											// 반환값 - TRUE로 바꿈 
		}
		return bRet;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Accept 할당 패킷 해제  						//
	// [2]PARAMETER : olp - 해제할 패킷 							//
	// [3]RETURN :	TRUE - 정상 처리 								//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::FreeAcptPacket( LPOVERLAPPEDPLUS olp )
	{
		LogWrite( "FreeAcptPacket(%08X)", (DWORD)olp );				// Logging

		if(!m_pAcptPool)
			return FALSE;												// 반환 

		return(m_pAcptPool->Free(olp));								// 패킷 해제 및 결과 반환 
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Receive 할당 패킷 해제  					//
	// [2]PARAMETER : olp - 해제할 패킷 							//
	// [3]RETURN :	TRUE - 정상 처리 								//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::FreeRecvPacket( LPOVERLAPPEDPLUS olp )
	{
		LogWrite( "FreeRecvPacket(%08X)", (DWORD)olp );				// Logging

		if(!m_pRecvPool)
			return FALSE;												// 반환 

		return( m_pRecvPool->Free(olp) );							// 패킷 해제 및 결과 반환						
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Process 할당 패킷 해제  					//
	// [2]PARAMETER : olp - 해제할 패킷 							//
	// [3]RETURN :	TRUE - 정상 처리 								//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::FreeSendPacket(LPOVERLAPPEDPLUS olp)
	{
		LogWrite( "FreeProcPacket(%08X)", (DWORD)olp );				// Logging

		if(!m_pProcPool)
			return FALSE;												// 반환 

		return(m_pProcPool->Free(olp));								// 패킷 해제 및 결과 반환
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Process 할당 버퍼  해제  					//
	// [2]PARAMETER : olp - 해제할 패킷 							//
	// [3]RETURN :	TRUE - 정상 처리 								//
	// [4]DATE : 2000년 9월 4일										//
	//////////////////////////////////////////////////////////////////
	BOOL NFPacketPool::FreeProcBuffer(PCHAR buffer)
	{
		LPOVERLAPPEDPLUS olp;
		olp = ((LPOVERLAPPEDPLUS)buffer) - 1;
		LogWrite( "FreeProcBuffer(%08X)", (DWORD)olp );				// Logging

		if(!m_pProcPool)
			return FALSE;												// 반환 

		return(m_pProcPool->Free(olp));								// 패킷 해제 및 결과 반환
	}

}