#include "NaveServer.h"
#include "NFServerCtrl.h"
#include <Nave/NFException.h>
#include <Nave/NFLog.h>
#include <Nave/NFSync.h>

namespace NaveServer {

//	INT				iMaxRecvPacket = 0;
//	INT				iRecvPacket = 0;								// 이 객체가 클라이언트와 연결 검사
//	DWORD			uRecvTickCnt = 0;								// 수신 Tick Count 

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	NFServerCtrl::NFServerCtrl()
	{
		m_bServerRun	= FALSE;							// 서버 구동상태
		m_bPause		= FALSE;

		m_nMaxThreadNum	= (GetNumberOfProcess() * 2);								// CPU수 * 2개로 Thread 수 결정 

		m_iPort			= 0;
		m_iMaxConn		= 50;

		m_pWorkThread		= NULL;								// 메인 스레드 핸들 
		m_hProcThread	= 0;
		m_hPacketThread  = 0;
		m_hIOCP			= NULL;								// IOCP 핸들 
		m_pUpdateManager = new NFUpdateManager();
	}

	NFServerCtrl::~NFServerCtrl()
	{
		m_bServerRun = FALSE;
		_DELETE(m_pUpdateManager);
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : IOCP 핸들 생성								//
	// [2]PARAMETER : void											//
	// [3]RETURN : HANDLE = 생성된 IOCP								//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	HANDLE NFServerCtrl::CreateIOCP()
	{
		return CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Listen 소켓 생성 							//
	// [2]PARAMETER : void											//
	// [3]RETURN : SOCKET = 생성된 LISTEN SOCKET					//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	SOCKET NFServerCtrl::CreateListenSocket(INT nServerPort, CHAR cBindQue)
	{
		SOCKET Socket = NULL;								// a Socket Variable for using Listener

		// 소켓 상태 설정 구조체 선언 
		SOCKADDR_IN		addr;
						addr.sin_family			= AF_INET;
						addr.sin_addr.s_addr	= INADDR_ANY;
						addr.sin_port			= htons( (short)nServerPort );

		// [1] Create Listen Socket
		Socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED );

		if(Socket == NULL)
			return Socket;

		// [2] bind listen socket
		if(bind(Socket,(SOCKADDR *)&addr,sizeof(addr)) != 0)
			return Socket;

		// [3] listening for an concurrent incoming connections limited in 5
		listen(Socket, cBindQue);

		LISTENER stLin;
		stLin.Init();

		stLin.s = Socket;
		stLin.nPort = nServerPort;
		stLin.cBindQue = cBindQue;

		m_vecListener.push_back(stLin);

		return Socket;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Listen Socket 을 iocp 소켓에 연결			//
	// [2]PARAMETER : SOCKET sckListener							//
	// [3]RETURN : HANDLE = 연결된 IOCP								//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	HANDLE NFServerCtrl::ConnectIOCPSocket(SOCKET sckListener)
	{
		return CreateIoCompletionPort((HANDLE)sckListener,m_hIOCP,0,0);
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : SOCKET과련 라이브러리 활성화 				//
	// [2]PARAMETER : void											//
	// [3]RETURN : void							 					//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	BOOL NFServerCtrl::InitSocket()
	{
		WSADATA wsaData;											// Initialzing Variables 
		return (SOCKET_ERROR != WSAStartup(0x202,&wsaData));		// Start Up
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 시스템의 CPU 수를 구함 						//
	// [2]PARAMETER : void											//
	// [3]RETURN : INT = 현재 시스템의 CPU 수 반환 					//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	INT NFServerCtrl::GetNumberOfProcess()
	{
		SYSTEM_INFO si;												// a System Info Structure Object
		GetSystemInfo( &si );										// Get the System Information
		return (INT)si.dwNumberOfProcessors;						// return the number of processors
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 서버 정지 			 						//
	// [2]PARAMETER : void											//
	// [3]RETURN : BOOL - 의미 없음				 					//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	BOOL NFServerCtrl::Stop()
	{
		//////////////////////////////////////////////////////////////////////
		//						Server Closing Process						//
		//////////////////////////////////////////////////////////////////////
		INT nCnt;
		// [01] Queue Suicide Packets into each IOCP Main Thread
		for( nCnt = 0; nCnt < m_nMaxThreadNum; nCnt++ )
		{
			if(PostQueuedCompletionStatus(m_hIOCP,0,IOCP_SHUTDOWN,0) ==NULL) 
				return FALSE;
		}

		// [02] Wait for thread terminations
		nCnt = WaitForMultipleObjects( m_nMaxThreadNum, m_pWorkThread, TRUE, 15000 );

		switch ( nCnt )
		{
			case WAIT_TIMEOUT:
				LOG_ERROR((L"Not All WorkThreads Died in Time."));
				break;
			case WAIT_FAILED:
				LOG_ERROR((L"WAIT_FAILED, WaitForMultipleObjects()."));
				break;
			default:
				break;
		}

		// [03] Close Thread Handles
		if( m_pWorkThread )
		{
			for( nCnt = 0; nCnt < m_nMaxThreadNum; nCnt++ ) 
				CloseHandle( m_pWorkThread[nCnt] );

			_DELETE_ARRAY(m_pWorkThread);
		}

		// [04] Process Thread Stop and Close
		m_bServerRun = FALSE;
		CloseHandle(m_hProcThread);

		CloseHandle(m_hPacketThread);

		if(ReleaseSession() == TRUE)
		{
			LOG_IMPORTANT((L"shutdown session.."));
		}

		// [05] Close Listener Socket
		int iListener = m_vecListener.size();
		for(int i = 0; i < iListener; ++i)
		{
			m_vecListener[i].Release();
		}

		// [6] Close IOCP Handle
		if(m_hIOCP) CloseHandle(m_hIOCP);

		// [7] 소켓 라이브러리 종료 
		WSACleanup();

		// [8] Show the Result of Close Processing
		LOG_IMPORTANT((L"shutdown sequence finished.."));

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : 서버 구동			 						//
	// [2]PARAMETER : void											//
	// [3]RETURN : BOOL - 의미 없음				 					//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	BOOL NFServerCtrl::Start(INT nPort, INT nMaxConn, INT nMaxThreadNum)
	{
		m_iPort = nPort;
		m_iMaxConn = nMaxConn;

		// 현재 사용되는 스레드는 Work용 스레드 CPU*2개
		// 받은 패킷이나 이벤트등을 처리하는 Update용 스레드 1개를 사용한다.
		// 만약 몬스터나 기타 다른 게임의 상태를 업데이트 하기위해서는 
		// 직접 작업용 스레드를 생성해 물리면 된다.
		/*
			생각하는것
			IOCP용 스레드 CPU개수만큼 (IOCP를 통해 Accept, Recv, Send를 하기 위한 스레드)
			EventUpdate용 스레드 1개  (패킷을 업데이트 하기 위한 큐)
			게임의 상태(몬스터및기타상태)용 스레드 ?개 
		*/

		// 0이아니면 그 개수 사용
		if(nMaxThreadNum != 0)
			m_nMaxThreadNum = nMaxThreadNum;
		else
			m_nMaxThreadNum	= (GetNumberOfProcess() * 2);			// CPU수 * 2개로 Thread 수 결정 

		// [00] 변수 및 객체 선언 
		INT				nCnt = 0;									// 루프 변수  
		UINT			nDummy;										// 쓰레기 값 처리 
		SOCKET			skListener;

		// [01] initialize socket library
		if(!InitSocket()) goto Error_Procss;

		// [02] Create IOCP 
		if((m_hIOCP = CreateIOCP()) == NULL) goto Error_Procss;

		// [03] Create Listener Socket
		if((skListener = CreateListenSocket(m_iPort)) == INVALID_SOCKET) goto Error_Procss;

		// [04] Connect listener socket to IOCP
		if(ConnectIOCPSocket(skListener) == NULL) goto Error_Procss;

		// [05] 화면에 서버 정보 표시 
		ShowServerInfo();

		// [06] Create Session..
		if(CreateSession(skListener) == FALSE)	goto Error_Procss;

		// [07] Update Session..
		if((m_pWorkThread = new HANDLE[m_nMaxThreadNum]) == NULL) goto Error_Procss;	// Create thread Control Handle

		for(nCnt = 0; nCnt < m_nMaxThreadNum; nCnt++)								// Run Thread 
		{
			if((m_pWorkThread[nCnt] = (HANDLE)_beginthreadex(0,0,Thread_MainEx,m_hIOCP,0,&nDummy)) == NULL) goto Error_Procss;
		}

		//--------------> Server Initializing has been done <---------------//
		// [8] Process Thread 생성하기
		m_bServerRun = TRUE;
		if((m_hProcThread = (HANDLE)_beginthreadex(0,0,Process_MainEx,this,0,&nDummy)) == NULL) goto Error_Procss;

		// [9] Process Thread 생성하기
		if((m_hPacketThread = (HANDLE)_beginthreadex(0,0,Packet_MainEx,this,0,&nDummy)) == NULL) goto Error_Procss;

		return TRUE;

	Error_Procss:
		Stop();
		m_bServerRun = FALSE;
		return FALSE;
	}

	VOID NFServerCtrl::ShowServerInfo()
	{
		WCHAR	szDate[32],
				szTime[32];

		_tzset();
		_wstrdate( szDate );
		_wstrtime( szTime );
		LOG_IMPORTANT((L"------------------------------------------------"));
		LOG_IMPORTANT((L" %s initialized at %s, %s", L"Server", szDate, szTime) );
		LOG_IMPORTANT((L"------------------------------------------------"));

		//////////////////////////////////////////////////////////////////////
		//							Server start							//
		//////////////////////////////////////////////////////////////////////
		LOG_IMPORTANT((L"------------------------------------------------"));
		LOG_IMPORTANT((L"|                 SERVER START                 |"));
		LOG_IMPORTANT((L"------------------------------------------------"));

		WCHAR Ip[32];
		GetLocalIP(Ip);
		LOG_IMPORTANT((L"IP(%s) Port(%d)", Ip, m_iPort ));
	}

	VOID NFServerCtrl::Pause(BOOL bPause)
	{
		m_bPause = bPause;
	}

	VOID NFServerCtrl::Update()
	{
/*		if(timeGetTime() - uRecvTickCnt > 1000)
		{
			InterlockedExchange((LONG*)&iMaxRecvPacket,iRecvPacket);
			InterlockedExchange((LONG*)&iRecvPacket,0);
			InterlockedExchange((LONG*)&uRecvTickCnt,timeGetTime());
		}
*/
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : IOCP처리를 담당하는 메인 스레드 			//
	// [2]PARAMETER : lpVoid - IOCP Handle							//
	// [3]RETURN : BOOL - 의미 없음				 					//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	unsigned NFServerCtrl::Thread_MainEx(LPVOID lpVoid)
	{
		DWORD			dwIoSize;									// 완료처리 사이즈 얻음 
		ULONG			lCompletionKey;								// 무한 루프 탈출 용이 됨 
		BOOL			bSuccess;									// 블럭킹 처리 에러 확인  
		HANDLE			hIOCP = (HANDLE)lpVoid;					// IOCP 핸들 얻음 
		
		LPOVERLAPPED	lpOverlapped;								// 중첩 확장 포인터 

		////////////
		// 무한 루프 
		while( TRUE )
		{
			// IOCP 처리를 기다리는 BLOCKING MODE
			bSuccess = GetQueuedCompletionStatus(hIOCP,										// IOCP Handle
												&dwIoSize,									// 처리 사이즈 
												&lCompletionKey,							// 완료 키 
												(LPOVERLAPPED*) &lpOverlapped,				// 중첩 확장 
												INFINITE);									// Waiting Time 

			LPOVERLAPPEDPLUS lpOverlapPlus = (LPOVERLAPPEDPLUS)lpOverlapped;

			if(bSuccess)
			{
				// 종료 신호가 들어왔다면, 루프 탈출 
				if( lCompletionKey == IOCP_SHUTDOWN ) break;

				if( NULL != lpOverlapPlus )
				{
					///////////////////////////////////////////////
					// 처리가 정상적으로 이루어진다면 이쪽으로...
					lpOverlapPlus->dwBytes = dwIoSize;				// 처리 데이타 Size
					// 처리 변수 Cast 변환 
					// 속도를 위해 메인프레임에선 try~catch를 뺀다.
					// 알수 없는 오류를 검사하기 위해선 Exception::EnableUnhandledExceptioinFilter(true)를 사용한다
/*
					// Recv 카운트 처리
					if(lpOverlapPlus->nConnState == ClientIoRead)
					{
						InterlockedExchange((LONG*)&iRecvPacket,iRecvPacket+1);
					}
*/
					NFConnection* lpClientConn = (NFConnection*) lpOverlapPlus->pClientConn;
					lpClientConn->DoIo(lpOverlapPlus);				// IOCP 처리 핸들링 
				}
			}
			else
			{
				if(!lpOverlapPlus)
				{
					LOG_ERROR((L"Critical Error on GetQueuedCompletionStatus()."));
				}
				else
				{
					// 처리 변수 Cast 변환
					NFConnection* lpClientConn = (NFConnection*) lpOverlapPlus->pClientConn;
					// 강제로 Clear해주면 안된다. (데이타가 제대로 초기화 되지 않을수도 있다)
//					lpClientConn->Clear();
//					LOG_ERROR(("[%04d] IOCP OverlapPlus Error, Close_Open()호출. SOCKET_ERROR, %d", lpClientConn->GetIndex(), WSAGetLastError()));
					lpClientConn->SetClose_Open(lpOverlapPlus, TRUE);	// 연결 해제
				}
			}
		}

		return 0;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : IOCP처리를 담당하는 메인 스레드 			//
	// [2]PARAMETER : lpVoid - IOCP Handle							//
	// [3]RETURN : BOOL - 의미 없음				 					//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	unsigned NFServerCtrl::Process_MainEx(LPVOID lpVoid)
	{
		NFServerCtrl*	pCtrl = (NFServerCtrl*)lpVoid;					// IOCP 핸들 얻음 

		assert(pCtrl);

		while( pCtrl->IsRun())
		{
			if(pCtrl->IsPause())
			{
				Sleep(1);
				continue;
			}

			pCtrl->Update();
			Sleep(1);
		}

		return 0;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : UpateQue처리를 담당하는 메인 스레드 		//
	// [2]PARAMETER : lpVoid - IOCP Handle							//
	// [3]RETURN : BOOL - 의미 없음				 					//
	// [4]DATE : 2003년 10월 24일									//
	//////////////////////////////////////////////////////////////////
	unsigned NFServerCtrl::Packet_MainEx(LPVOID lpVoid)
	{
		NFServerCtrl*	pCtrl = (NFServerCtrl*)lpVoid;					// IOCP 핸들 얻음 
		NFUpdateManager* pUpdateManager = NFUpdateManager::GetInstancePtr();

		assert(pCtrl);
		assert(pUpdateManager);

		while( pCtrl->IsRun())
		{
			pUpdateManager->Update();
			Sleep(1);
		}

		return 0;
	}
	

}