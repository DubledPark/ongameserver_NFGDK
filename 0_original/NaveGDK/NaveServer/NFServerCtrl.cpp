#include "NaveServer.h"
#include "NFServerCtrl.h"
#include <Nave/NFException.h>
#include <Nave/NFLog.h>
#include <Nave/NFSync.h>

namespace NaveServer {

//	INT				iMaxRecvPacket = 0;
//	INT				iRecvPacket = 0;								// �� ��ü�� Ŭ���̾�Ʈ�� ���� �˻�
//	DWORD			uRecvTickCnt = 0;								// ���� Tick Count 

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	NFServerCtrl::NFServerCtrl()
	{
		m_bServerRun	= FALSE;							// ���� ��������
		m_bPause		= FALSE;

		m_nMaxThreadNum	= (GetNumberOfProcess() * 2);								// CPU�� * 2���� Thread �� ���� 

		m_iPort			= 0;
		m_iMaxConn		= 50;

		m_pWorkThread		= NULL;								// ���� ������ �ڵ� 
		m_hProcThread	= 0;
		m_hPacketThread  = 0;
		m_hIOCP			= NULL;								// IOCP �ڵ� 
		m_pUpdateManager = new NFUpdateManager();
	}

	NFServerCtrl::~NFServerCtrl()
	{
		m_bServerRun = FALSE;
		_DELETE(m_pUpdateManager);
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : IOCP �ڵ� ����								//
	// [2]PARAMETER : void											//
	// [3]RETURN : HANDLE = ������ IOCP								//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	HANDLE NFServerCtrl::CreateIOCP()
	{
		return CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : Listen ���� ���� 							//
	// [2]PARAMETER : void											//
	// [3]RETURN : SOCKET = ������ LISTEN SOCKET					//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	SOCKET NFServerCtrl::CreateListenSocket(INT nServerPort, CHAR cBindQue)
	{
		SOCKET Socket = NULL;								// a Socket Variable for using Listener

		// ���� ���� ���� ����ü ���� 
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
	// [1]DESCRIPTION : Listen Socket �� iocp ���Ͽ� ����			//
	// [2]PARAMETER : SOCKET sckListener							//
	// [3]RETURN : HANDLE = ����� IOCP								//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	HANDLE NFServerCtrl::ConnectIOCPSocket(SOCKET sckListener)
	{
		return CreateIoCompletionPort((HANDLE)sckListener,m_hIOCP,0,0);
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : SOCKET���� ���̺귯�� Ȱ��ȭ 				//
	// [2]PARAMETER : void											//
	// [3]RETURN : void							 					//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	BOOL NFServerCtrl::InitSocket()
	{
		WSADATA wsaData;											// Initialzing Variables 
		return (SOCKET_ERROR != WSAStartup(0x202,&wsaData));		// Start Up
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : �ý����� CPU ���� ���� 						//
	// [2]PARAMETER : void											//
	// [3]RETURN : INT = ���� �ý����� CPU �� ��ȯ 					//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	INT NFServerCtrl::GetNumberOfProcess()
	{
		SYSTEM_INFO si;												// a System Info Structure Object
		GetSystemInfo( &si );										// Get the System Information
		return (INT)si.dwNumberOfProcessors;						// return the number of processors
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : ���� ���� 			 						//
	// [2]PARAMETER : void											//
	// [3]RETURN : BOOL - �ǹ� ����				 					//
	// [4]DATE : 2003�� 10�� 24��									//
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

		// [7] ���� ���̺귯�� ���� 
		WSACleanup();

		// [8] Show the Result of Close Processing
		LOG_IMPORTANT((L"shutdown sequence finished.."));

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : ���� ����			 						//
	// [2]PARAMETER : void											//
	// [3]RETURN : BOOL - �ǹ� ����				 					//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	BOOL NFServerCtrl::Start(INT nPort, INT nMaxConn, INT nMaxThreadNum)
	{
		m_iPort = nPort;
		m_iMaxConn = nMaxConn;

		// ���� ���Ǵ� ������� Work�� ������ CPU*2��
		// ���� ��Ŷ�̳� �̺�Ʈ���� ó���ϴ� Update�� ������ 1���� ����Ѵ�.
		// ���� ���ͳ� ��Ÿ �ٸ� ������ ���¸� ������Ʈ �ϱ����ؼ��� 
		// ���� �۾��� �����带 ������ ������ �ȴ�.
		/*
			�����ϴ°�
			IOCP�� ������ CPU������ŭ (IOCP�� ���� Accept, Recv, Send�� �ϱ� ���� ������)
			EventUpdate�� ������ 1��  (��Ŷ�� ������Ʈ �ϱ� ���� ť)
			������ ����(���͹ױ�Ÿ����)�� ������ ?�� 
		*/

		// 0�̾ƴϸ� �� ���� ���
		if(nMaxThreadNum != 0)
			m_nMaxThreadNum = nMaxThreadNum;
		else
			m_nMaxThreadNum	= (GetNumberOfProcess() * 2);			// CPU�� * 2���� Thread �� ���� 

		// [00] ���� �� ��ü ���� 
		INT				nCnt = 0;									// ���� ����  
		UINT			nDummy;										// ������ �� ó�� 
		SOCKET			skListener;

		// [01] initialize socket library
		if(!InitSocket()) goto Error_Procss;

		// [02] Create IOCP 
		if((m_hIOCP = CreateIOCP()) == NULL) goto Error_Procss;

		// [03] Create Listener Socket
		if((skListener = CreateListenSocket(m_iPort)) == INVALID_SOCKET) goto Error_Procss;

		// [04] Connect listener socket to IOCP
		if(ConnectIOCPSocket(skListener) == NULL) goto Error_Procss;

		// [05] ȭ�鿡 ���� ���� ǥ�� 
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
		// [8] Process Thread �����ϱ�
		m_bServerRun = TRUE;
		if((m_hProcThread = (HANDLE)_beginthreadex(0,0,Process_MainEx,this,0,&nDummy)) == NULL) goto Error_Procss;

		// [9] Process Thread �����ϱ�
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
	// [1]DESCRIPTION : IOCPó���� ����ϴ� ���� ������ 			//
	// [2]PARAMETER : lpVoid - IOCP Handle							//
	// [3]RETURN : BOOL - �ǹ� ����				 					//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	unsigned NFServerCtrl::Thread_MainEx(LPVOID lpVoid)
	{
		DWORD			dwIoSize;									// �Ϸ�ó�� ������ ���� 
		ULONG			lCompletionKey;								// ���� ���� Ż�� ���� �� 
		BOOL			bSuccess;									// ��ŷ ó�� ���� Ȯ��  
		HANDLE			hIOCP = (HANDLE)lpVoid;					// IOCP �ڵ� ���� 
		
		LPOVERLAPPED	lpOverlapped;								// ��ø Ȯ�� ������ 

		////////////
		// ���� ���� 
		while( TRUE )
		{
			// IOCP ó���� ��ٸ��� BLOCKING MODE
			bSuccess = GetQueuedCompletionStatus(hIOCP,										// IOCP Handle
												&dwIoSize,									// ó�� ������ 
												&lCompletionKey,							// �Ϸ� Ű 
												(LPOVERLAPPED*) &lpOverlapped,				// ��ø Ȯ�� 
												INFINITE);									// Waiting Time 

			LPOVERLAPPEDPLUS lpOverlapPlus = (LPOVERLAPPEDPLUS)lpOverlapped;

			if(bSuccess)
			{
				// ���� ��ȣ�� ���Դٸ�, ���� Ż�� 
				if( lCompletionKey == IOCP_SHUTDOWN ) break;

				if( NULL != lpOverlapPlus )
				{
					///////////////////////////////////////////////
					// ó���� ���������� �̷�����ٸ� ��������...
					lpOverlapPlus->dwBytes = dwIoSize;				// ó�� ����Ÿ Size
					// ó�� ���� Cast ��ȯ 
					// �ӵ��� ���� ���������ӿ��� try~catch�� ����.
					// �˼� ���� ������ �˻��ϱ� ���ؼ� Exception::EnableUnhandledExceptioinFilter(true)�� ����Ѵ�
/*
					// Recv ī��Ʈ ó��
					if(lpOverlapPlus->nConnState == ClientIoRead)
					{
						InterlockedExchange((LONG*)&iRecvPacket,iRecvPacket+1);
					}
*/
					NFConnection* lpClientConn = (NFConnection*) lpOverlapPlus->pClientConn;
					lpClientConn->DoIo(lpOverlapPlus);				// IOCP ó�� �ڵ鸵 
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
					// ó�� ���� Cast ��ȯ
					NFConnection* lpClientConn = (NFConnection*) lpOverlapPlus->pClientConn;
					// ������ Clear���ָ� �ȵȴ�. (����Ÿ�� ����� �ʱ�ȭ ���� �������� �ִ�)
//					lpClientConn->Clear();
//					LOG_ERROR(("[%04d] IOCP OverlapPlus Error, Close_Open()ȣ��. SOCKET_ERROR, %d", lpClientConn->GetIndex(), WSAGetLastError()));
					lpClientConn->SetClose_Open(lpOverlapPlus, TRUE);	// ���� ����
				}
			}
		}

		return 0;
	}

	//////////////////////////////////////////////////////////////////
	// [1]DESCRIPTION : IOCPó���� ����ϴ� ���� ������ 			//
	// [2]PARAMETER : lpVoid - IOCP Handle							//
	// [3]RETURN : BOOL - �ǹ� ����				 					//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	unsigned NFServerCtrl::Process_MainEx(LPVOID lpVoid)
	{
		NFServerCtrl*	pCtrl = (NFServerCtrl*)lpVoid;					// IOCP �ڵ� ���� 

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
	// [1]DESCRIPTION : UpateQueó���� ����ϴ� ���� ������ 		//
	// [2]PARAMETER : lpVoid - IOCP Handle							//
	// [3]RETURN : BOOL - �ǹ� ����				 					//
	// [4]DATE : 2003�� 10�� 24��									//
	//////////////////////////////////////////////////////////////////
	unsigned NFServerCtrl::Packet_MainEx(LPVOID lpVoid)
	{
		NFServerCtrl*	pCtrl = (NFServerCtrl*)lpVoid;					// IOCP �ڵ� ���� 
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