#include "NaveServer.h"
#include "NFConnection.h"
#include <Nave/NFLog.h>


namespace NaveServer {

	NFConnection::NFConnection() : m_nMaxBuf(0), m_nMaxPacketSize(0), m_sckListener(NULL), 
									m_Socket(NULL), m_eConnectFlag(CONNECT_NONE), m_bIsConnect(FALSE), 
									m_bForce(FALSE), m_hIOCP(NULL), m_pPacketPool(NULL), m_uRecvTickCnt(0)
	{
		m_RecvPacket.Init();
		m_RecvIO.NewIOBuf(0);	// 0���� �ϸ� �⺻������ 1024*64��ŭ ������.
	}

	NFConnection::~NFConnection()
	{
		m_nMaxBuf = m_nMaxPacketSize = 0;
		m_sckListener	= NULL;

		Disconnect();

		m_eConnectFlag	= CONNECT_NONE;
		m_bIsConnect	= FALSE;
		m_bForce		= FALSE;

		m_hIOCP			= NULL;
		m_pPacketPool	= NULL;
		m_uRecvTickCnt	= 0;

		m_RecvIO.DeleteIOBuf();
	}

	BOOL NFConnection::Create(	DWORD			dwIndex,
								HANDLE			hIOCP,
								SOCKET			listener,
								NFPacketPool*	pPacketPool,
								INT				nMaxBuf)
	{	
		// ��ü�� ��� ���� ���� 
		m_dwIndex			= dwIndex;
		m_hIOCP				= hIOCP;
		m_sckListener		= listener;
		m_pPacketPool		= pPacketPool;

		m_nMaxBuf			= m_nMaxPacketSize	= nMaxBuf;

		// ��ü OPEN
		if(!Open())
		{
			// �����ϸ� ��� �ؾ� �ϴ°�? �� ���ؼ� Ŭ������ ���� Ŭ������ �ȴ�.
			Close_Open(FALSE);
			return FALSE;
		}

		return TRUE;
	}

	BOOL NFConnection::Open()
	{
		// ���� �� �־�� ��.
		assert(m_nMaxBuf);
		assert(m_nMaxPacketSize);
		assert(m_pPacketPool);
		assert(m_sckListener);

		// ��Ŷ Pool�� ����� ���� �Ǿ����� �˻� 
//		if( !m_pPacketPool ) return FALSE;

		// Listener Socket ���� �˻� 
//		if( m_sckListener == NULL ) return FALSE;

		m_RecvIO.InitIOBuf();	// ��Ŷ���۸� �ʱ�ȭ�Ѵ�.
		
 		// create socket for send/recv
		m_Socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP,NULL,0,WSA_FLAG_OVERLAPPED );

		// ���� ������ ����� ���� �Ǿ����� �˻� 
		if(m_Socket == NULL) return FALSE;

		// Accpet�� ����������ü�� ��Ŷ���۸� �غ��Ѵ�.
		LPOVERLAPPEDPLUS newolp = PrepareAcptPacket();

		if(NULL == newolp) 
		{
			closesocket(m_Socket);
			m_Socket = NULL;
			return FALSE;
		}

		/////////////////////////////////////////////////////////////////////
		// Socket�� Listener�� ����
		// Overlapped�� ���� ������ ���߿� IOCP �̺�Ʈ �߻� ó���� ���δ� 
		BOOL bRet = AcceptEx(newolp->sckListen,						// Listen Socket
							newolp->sckClient,						// Socket
							&(newolp->wbuf.buf[0]),				// ���� ������ 
							m_nMaxBuf,								// ���� ������ 
							sizeof(sockaddr_in) + 16,				// ���� ���� - IP, Address, Name.. etc
							sizeof(sockaddr_in) + 16,				// ���� ���� - IP, Address, Name.. etc
							&newolp->dwBytes,						// ó�� ����Ʈ ũ�� 
							&newolp->overlapped);					// *�߿�*


		// ���� ó�� 
		if(!bRet && WSAGetLastError() != WSA_IO_PENDING)
		{
			ShowMessage(ACCEPT_FAILED);
			closesocket(m_Socket);
			m_Socket = NULL;
			ReleaseAcptPacket(newolp);
			return FALSE;
		}

		/////////////////////////////////
		// ������ ���� ����ȭ�� ���� ���� 
		INT zero = 0;
		INT err = 0;

		// Send Buffer�� ���� ����
		if( (err = setsockopt( m_Socket, SOL_SOCKET, SO_SNDBUF, (CHAR *)&zero, sizeof(zero))) == SOCKET_ERROR)
		{
			ShowMessage(ACCEPT_FAILED);
			closesocket(m_Socket);
			m_Socket = NULL;
			ReleaseAcptPacket(newolp);
			return FALSE;
		}

		// Receive Buffer�� ���� ���� 
		if((err = setsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, (CHAR *)&zero, sizeof(zero) )) == SOCKET_ERROR)
		{
			ShowMessage(ACCEPT_FAILED);
			closesocket(m_Socket);
			m_Socket = NULL;
			ReleaseAcptPacket(newolp);
			return FALSE;
		}

		// ���� �ʱ�ȭ 
		InterlockedExchange((LONG*)&m_bIsConnect,0);
		Clear();

		m_uRecvTickCnt = 0 ;

		return TRUE;
	}

	BOOL NFConnection::Close_Open( BOOL bForce )
	{
		// ���ϰ� ������ ���� Ȯ�� 
		// Disconnect �Լ��ȿ��� OnDisconnect(), Clear() �� ȣ��ȴ�.
		Disconnect(bForce);

		// �� ��Ŷ �ٽ� �ʱ�ȭ 
		if(!Open())
		{
			// �����ϸ� ��� �ؾ� �ϴ°�? �� ���ؼ� Ŭ������ ���� Ŭ������ �ȴ�.
			// ���� ���� ���¸� ǥ�����ְ� üũ����. �׸��� ���߿� Ÿ�̸ӿ��� �ٽ� �ʱ�ȭ �غ���.
			ShowMessage(DEAD_CONNECTION);
			Disconnect();
			return FALSE;
		}
		return TRUE;												// ���� ó�� 
	}

	VOID NFConnection::ShowMessage(INT MsgIndex)
	{
		switch(MsgIndex)
		{
		case ACCEPT_FAILED:
			LOG_ERROR((L"[%04d] AcceptEx(): SOCKET_ERROR, %d.", GetIndex(), WSAGetLastError()));
			break;
		case CLOSE_SOCKET:
			LOG_INFO((L"[%04d] ���� ����.", GetIndex()));
			break;
		case DEAD_CONNECTION:
			LOG_ERROR((L"[%04d] Dead Connection.", GetIndex()));
			break;
		case CONNECT_SUCCESS:
			LOG_INFO((L"[%04d] ���� ����.", GetIndex()));
			break;
		case CONNECT_FAILED:
			LOG_ERROR((L"[%04d] ���� ���� (Check Sum ����).", GetIndex()));
			break;
		case DISPATCH_FAILED:
			LOG_ERROR((L"[%04d] Dispatch return FALSE.", GetIndex()));
			break;
		case DOIOSWITCH_FAILED:
			LOG_ERROR((L"[%04d] DoIo(..) - Switch Default.", GetIndex()));
			break;
		case ALLOCACPT_FAILED:
			LOG_ERROR((L"[%04d] PrepareAcptPacket() acpt packet alloc failed.", GetIndex()));
			break;
		case PREPAREACPT_FAILED:
			LOG_ERROR((L"[%04d] PrepareAcptPacket().newolp == NULL.", GetIndex()));
			break;
		case PREPARERECVSIZE_FAILED:
			LOG_ERROR((L"[%04d] PrepareRecvPacket() srclen > m_nMaxPacketSize.", GetIndex()));
			break;
		case PREPARESENDSIZE_FAILED:
			LOG_ERROR((L"[%04d] PrepareSendPacket() srclen > m_nMaxPacketSize.", GetIndex()));
			break;
		case ALLOCRECV_FAILED:
			LOG_ERROR((L"[%04d] PrepareRecvPacket() recv packet alloc failed.", GetIndex()));
			break;
		case ALLOCSEND_FAILED:
			LOG_ERROR((L"[%04d] PrepareSendPacket() send packet alloc failed.", GetIndex()));
			break;
		case PREPARERECV_FAILED:
			LOG_ERROR((L"[%04d] PrepareRecvPacket().newolp == NULL.", GetIndex()));
			break;
		case PREPARESEND_FAILED:
			LOG_ERROR((L"[%04d] PrepareSendPacket().newolp == NULL.", GetIndex()));
			break;
		case RELEASEACPT_FAILED:
			LOG_ERROR((L"[%04d] ReleaseAcptPacket() free acpt packet failed.", GetIndex()));
			break;
		case RELEASERECV_FAILED:
			LOG_ERROR((L"[%04d] ReleaseRecvPacket() free recv packet failed.", GetIndex()));
			break;
		case RELEASESEND_FAILED:
			LOG_ERROR((L"[%04d] ReleaseSendPacket() free send packet failed.", GetIndex()));
			break;
		case BINDIOCP_FAILED:
			LOG_ERROR((L"[%04d] BindIOCP().lpOverlapPlus == NULL.", GetIndex()));
			break;
		case RECVPOST_FAILED:
			LOG_ERROR((L"[%04d] RecvPost() m_Socket == NULL or IsConnect() == FALSE.", GetIndex()));
			break;
		case RECVPOSTPENDING_FAILED:
			LOG_ERROR((L"[%04d] RecvPost().WSARecv() == SOCKET_ERROR, %d.", GetIndex(), WSAGetLastError()));
			break;
//		case ALLOCPROC_FAILED:
//			LOG_ERROR((L"[%04d] SendPost().AllocProcBuffer == NULL or SOCKET_ERROR, %d.", GetIndex(), WSAGetLastError()));
//			break;
		case SENDPOSTPENDING_FAILED:
			LOG_ERROR((L"[%04d] SendPost().WSASend() == SOCKET_ERROR, %d.", GetIndex(), WSAGetLastError()));
			break;
		case SENDPOST_FAILED:
			LOG_ERROR((L"[%04d] SendPost() == SOCKET_ERROR, %d.", GetIndex(), WSAGetLastError()));
			break;
		}
	}

	BOOL NFConnection::DoIo( LPOVERLAPPEDPLUS lpOverlapPlus )
	{
		// �Ҵ� ��Ŷ�� ���� Ȯ�� 
		switch(lpOverlapPlus->nConnState)
		{
		// ACCEPT���� ó��
		case ClientIoAccept:
			BindIOCP(lpOverlapPlus);							// ���� ���ϰ� IOCP ���ε� ó�� 

			InterlockedIncrement((LONG*)&m_bIsConnect);		// ���� ���� ���� ON !!!
			
			// �˻�� ������ �Ƴ� ��� ���� ���� 
			if(lpOverlapPlus->dwBytes && strncmp(lpOverlapPlus->wbuf.buf,CONNECT_CHECK_DATA,CONNECT_CHECK_SIZE) == 0)
			{
				ShowMessage(CONNECT_SUCCESS);
				SetConnectFlag(CONNECT_TRUE);
			}
			else
			{
				ShowMessage(CONNECT_FAILED);
				LOG_ERROR((L"[%04d] Check Sum : %d, %s.", GetIndex(), lpOverlapPlus->dwBytes, lpOverlapPlus->wbuf.buf));
				SetConnectFlag(CONNECT_FALSE);
				ReleaseAcptPacket( lpOverlapPlus );
				break;
			}

			// Accept �Ҵ� ��Ŷ ���� 
			if(!RecvPost())
			{
				SetClose_Open(lpOverlapPlus);
				break;
			}

			// TICK ī��Ʈ ���� 
			InterlockedExchange((LONG*)&m_uRecvTickCnt,timeGetTime());

			ReleaseAcptPacket( lpOverlapPlus );
			break;

		// RECEIVE ���� ó�� 
		case ClientIoRead:
			// TICK ī��Ʈ ���� 
			InterlockedExchange((LONG*)&m_uRecvTickCnt,timeGetTime());

			// ó�� ����Ÿ�� ���ٸ� 
			if(lpOverlapPlus->dwBytes == 0)
			{
				SetClose_Open(lpOverlapPlus, FALSE);			// ����, ��ü �ٽ� �ʱ�ȭ 
			}
			else if((INT)lpOverlapPlus->dwBytes == SOCKET_ERROR) // ������� 
			{
				SetClose_Open(lpOverlapPlus, TRUE);			// ����, ��ü �ٽ� �ʱ�ȭ 
			}
			else// �����̶�� 
			{
				// �޼��� ���� 
				if(!DispatchPacket(lpOverlapPlus))
				{
					ShowMessage(DISPATCH_FAILED);
					SetClose_Open( lpOverlapPlus, TRUE );
				}
				else
				{
					// Receive �Ҵ� ��Ŷ ���� 
					ReleaseRecvPacket( lpOverlapPlus );
				}
			}
			break;

		case ClientIoWrite:
			ReleaseSendPacket( lpOverlapPlus );
			break;

		default:
			ShowMessage(DOIOSWITCH_FAILED);
			assert(0);
			break;
		}
		return TRUE;
	}

	LPOVERLAPPEDPLUS NFConnection::PrepareAcptPacket()
	{
		LPOVERLAPPEDPLUS newolp = NULL;

		// get accept overlapped structure and packet buffer.
		if(m_pPacketPool->AllocAcptPacket(newolp) == FALSE)
		{
			ShowMessage(ALLOCACPT_FAILED);
			return NULL;
		}

		// ġ������ ���� 
		if(!newolp)
		{
			ShowMessage(PREPAREACPT_FAILED);
			return NULL;
		}

		// clear buffer
		memset(&newolp->overlapped	, NULL, sizeof(OVERLAPPED));
		memset(&newolp->wbuf.buf[0], NULL, sizeof(m_nMaxPacketSize));

		// init olp structure
		newolp->sckListen	= m_sckListener;
		newolp->sckClient	= m_Socket;
		newolp->nConnState	= ClientIoAccept;
		newolp->pClientConn = (PVOID)this;
		newolp->wbuf.len	= CONNECT_CHECK_SIZE;					//	newolp->wbuf.len	= MAXPACKETSIZE;
																	// ** WARNING ** 
																	// When you change your packet certfying correct connection,
																	// you must change the size of definition 'CONNECT_CHECK_SIZE'.
		return newolp;
	}

	LPOVERLAPPEDPLUS NFConnection::PrepareRecvPacket(UINT srclen)
	{
		// ġ������ ����
		if(srclen > (UINT)m_nMaxPacketSize)
		{
			ShowMessage(PREPARERECVSIZE_FAILED);
			return NULL;
		}

		LPOVERLAPPEDPLUS newolp = NULL;

		// get recv overlapped structure and packet buffer.
		if( FALSE == m_pPacketPool->AllocRecvPacket(newolp) )
		{
			ShowMessage(ALLOCRECV_FAILED);
			return NULL;
		}

		// ġ������ ����
		if(!newolp)
		{
			ShowMessage(PREPARERECV_FAILED);
			return NULL;
		}

		// clear buffer
		memset(&newolp->overlapped	, NULL, sizeof(OVERLAPPED));
		memset(&newolp->wbuf.buf[0], NULL, sizeof(m_nMaxPacketSize));

		// init olp structure
		newolp->sckListen	= m_sckListener;
		newolp->sckClient	= m_Socket;
		newolp->nConnState	= ClientIoRead;
		newolp->pClientConn = (PVOID) this;

		if(srclen == 0)	newolp->wbuf.len	= m_nMaxPacketSize; 
		else			newolp->wbuf.len	= srclen; 
		
		return newolp;
	}

	LPOVERLAPPEDPLUS NFConnection::PrepareSendPacket(CHAR *psrcbuf, UINT srclen)
	{
		// ġ������ ����
		if(srclen < 0 || srclen > (UINT)m_nMaxPacketSize)
		{
			ShowMessage(PREPARESENDSIZE_FAILED);
			return NULL;
		}

		LPOVERLAPPEDPLUS newolp = NULL;

		// get recv overlapped structure and packet buffer.
		if( FALSE == m_pPacketPool->AllocSendPacket(newolp) )
		{
			ShowMessage(ALLOCSEND_FAILED);
			return NULL;
		}

		// ġ������ ����
		if(!newolp)
		{
			ShowMessage(PREPARESEND_FAILED);
			return NULL;
		}

		// clear buffer
		memset(&newolp->overlapped	, NULL, sizeof(OVERLAPPED));
		memset(&newolp->wbuf.buf[0], NULL, sizeof(m_nMaxPacketSize));

		// init olp structure
		newolp->sckListen	= m_sckListener;
		newolp->sckClient	= m_Socket;
		newolp->nConnState	= ClientIoWrite;
		newolp->pClientConn = (PVOID) this;
		newolp->wbuf.len	= srclen; 
		memcpy(newolp->wbuf.buf,psrcbuf,srclen);
		
		return newolp;
	}

	BOOL NFConnection::ReleaseAcptPacket(LPOVERLAPPEDPLUS olp)
	{
		if(NULL == olp)				return FALSE;
		if(NULL == olp->wbuf.buf)	return FALSE;

		if(!m_pPacketPool->FreeAcptPacket(olp))
		{
			ShowMessage(RELEASEACPT_FAILED);
			return FALSE;
		}
		return TRUE;
	}

	BOOL NFConnection::ReleaseRecvPacket(LPOVERLAPPEDPLUS olp)
	{
		if(olp == NULL)				return FALSE;
		if(olp->wbuf.buf == NULL)	return FALSE;

		if(!m_pPacketPool->FreeRecvPacket(olp))
		{
			ShowMessage(RELEASERECV_FAILED);
			return FALSE;
		}
		return TRUE;
	}

	BOOL NFConnection::ReleaseSendPacket(LPOVERLAPPEDPLUS olp)
	{
		if(olp == NULL)				return FALSE;
		if(olp->wbuf.buf == NULL)	return FALSE;

		if(!m_pPacketPool->FreeSendPacket(olp))
		{
			ShowMessage(RELEASESEND_FAILED);
			return FALSE;
		}
		return TRUE;
	}

	BOOL NFConnection::BindIOCP(LPOVERLAPPEDPLUS lpOverlapPlus)
	{
		// ġ������ ����
		if(!lpOverlapPlus)
		{
			ShowMessage(BINDIOCP_FAILED);
			return FALSE;
		}

		INT	locallen, remotelen;

		sockaddr_in *	plocal	= 0,
					*	premote	= 0;

		GetAcceptExSockaddrs(
			&(lpOverlapPlus->wbuf.buf[0]),
			m_nMaxBuf,
			sizeof(sockaddr_in) + 16,
			sizeof(sockaddr_in) + 16,
			(sockaddr **)&plocal,									// ������ 
			&locallen,
			(sockaddr **)&premote,									// ���ô� 
			&remotelen
		);

		memcpy(&m_Local, plocal, sizeof(sockaddr_in));
		memcpy(&m_Peer, premote, sizeof(sockaddr_in));

		if(CreateIoCompletionPort((HANDLE)lpOverlapPlus->sckClient,m_hIOCP,0,0) == 0) return FALSE;

		return TRUE;
	}

	void NFConnection::SetClose_Open(LPOVERLAPPEDPLUS lpOverlapPlus, BOOL bForce)
	{
		if(m_eConnectFlag != CONNECT_NONE)
		{
			LOG_ERROR((L"NFConnection::SetClose_Open() m_eConnectFlag == %d", (int)m_eConnectFlag));
		}

		// �Ҵ� ��Ŷ ���� �˻��� ��Ŷ �Ҵ� ���� 
		if(NULL != lpOverlapPlus)
		{
			if(NULL != lpOverlapPlus->wbuf.buf && NULL != m_pPacketPool)
			{
				// ���������� �Ϸ� �Ǿ��� ����������ü�� ���۸� �������Ѵ�.
				switch( lpOverlapPlus->nConnState)
				{
				case ClientIoAccept:
					ReleaseAcptPacket( lpOverlapPlus );
					break;

				case ClientIoRead:
					ReleaseRecvPacket( lpOverlapPlus );
					break;

				case ClientIoWrite:
					ReleaseSendPacket( lpOverlapPlus );
					break;

				default:
					break;
				}
			}
		}

		m_bForce = bForce;

		InterlockedExchange((LONG*)&m_eConnectFlag,CLOSEOPEN_TRUE);

		NFUpdateManager::GetInstance().Add(this, NULL);
	}

	void NFConnection::SetConnectFlag(CONNECT_EVENT eState)
	{
		if(m_eConnectFlag != CONNECT_NONE)
		{
			LOG_ERROR((L"NFConnection::SetConnectFlag() m_eConnectFlag == %s", (int)m_eConnectFlag));
		}

		InterlockedExchange((LONG*)&m_eConnectFlag,eState);

		NFUpdateManager::GetInstance().Add(this, NULL);
	}

	BOOL NFConnection::DispatchPacket(LPOVERLAPPEDPLUS lpOverlapPlus)
	{
		// Read������� RecvPost �Ҷ� ���� 1�� �߻��ϴµ� ����
		// Read�Ҷ� RecvPost�� 1���� �ϱ� �빮�� �����尡 �ƹ��� ���Ƶ�.DispatchPacket�� 1���� �Ͼ��.

		CHAR*  psrcbuf     =	&( lpOverlapPlus->wbuf.buf[0] );
		INT     srclen     =    lpOverlapPlus->dwBytes;

		// Packet���� ��ü�� [H 2byte][P size] �����̴�.
		m_RecvIO.Append(psrcbuf, srclen);

		m_RecvIO.Lock();

		// IOCP�� ������ ��Ŷó���� ���� ��������� �� �ɷ��̴�
		// �׷��� �Ʒ��� ���� UpdateManaget�� ��Ŷ�� ������ �� �����忡�� Update�� ó���ϸ�
		// ��������� ������ ������ ������ ����ó���� ���� ��������� ������ �ʴ´�.
		// ��Ŷ�� ������ ���¿��� �ٷ� ó���ϰ� �����ϰ� UpdateManager�� Ŀ��Ʈ �÷�����
		// ������Ʈ �ϴ°ɷ� �����غ���. 
#ifdef ENABLE_UPDATEQUE
		if(m_RecvIO.GetPacket(&m_RecvPacket) == 1)
		{
			m_RecvPacket.DecryptPacket();

			if(m_RecvPacket.IsAliveChecksum())
				NFUpdateManager::GetInstance().Add(this, &m_RecvPacket);

			m_RecvPacket.Init();
		}
		else
			m_RecvIO.UnLock();
#else
		// �Ʒ��� ���� �����忡�� ��Ŷ�� ó���ϰ� �Ǹ� 
		// Dispatch�� ũ��Ƽ�� ������ ����־� ������� �����ؾ��Ѵ�.
		if(m_RecvIO.GetPacket(&m_RecvPacket) == 1)
		{
			m_RecvPacket.DecryptPacket();

			if(m_RecvPacket.IsAliveChecksum())
			{	
				DispatchPacket(m_RecvPacket);
			}

			m_RecvPacket.Init();
		}
		else
			m_RecvIO.UnLock();
#endif

		// �׸��� ���ο� recieve buffer�� �غ��Ͽ� Post�Ѵ�.
		return RecvPost();
	}

	VOID NFConnection::UpdatePacket(NaveNet::NFPacket& Packet)
	{
		if(Packet.GetCommand() != 0)
		{
			DispatchPacket(Packet);
			return;
		}

		// Ŀ�ؼ��� ���¸� ������Ʈ �ϱ����� �κ�.
		if(m_eConnectFlag == CONNECT_NONE)
			return;

		if(m_eConnectFlag == CONNECT_TRUE)
		{
			InterlockedExchange((LONG*)&m_eConnectFlag,CONNECT_NONE);
			OnConnect(TRUE);
		}
		else if(m_eConnectFlag == CONNECT_FALSE)
		{
			InterlockedExchange((LONG*)&m_eConnectFlag,CONNECT_NONE);
			OnConnect(FALSE);
			Close_Open();
		}
		else if(m_eConnectFlag == DISCONNECT_TURE)
		{
			InterlockedExchange((LONG*)&m_eConnectFlag,CONNECT_NONE);
			Disconnect(FALSE);
		}
		else if(m_eConnectFlag  == CLOSEOPEN_TRUE)
		{
			InterlockedExchange((LONG*)&m_eConnectFlag,CONNECT_NONE);
			Close_Open(m_bForce);
		}
	}

	BOOL NFConnection::SendPost( CHAR* pPackte, INT Len )
	{
		if(!m_Socket) return FALSE;
		if(!IsConnect()) return FALSE;

		if(m_Socket == NULL || IsConnect() == FALSE)
		{
			ShowMessage(SENDPOST_FAILED);
			return FALSE;
		}

		// prepare recieve buffer
		LPOVERLAPPEDPLUS newolp = PrepareSendPacket(pPackte,Len);

		// ����� �Ҵ� �޾Ҵ��� ����
		if(newolp == NULL)	return FALSE;

		INT ret = WSASend(	newolp->sckClient,
							&newolp->wbuf,
							1,
							&newolp->dwBytes,						// ���� ȣ�������� �ٷ� �޾Ҵٸ� ����� ���� ũ�Ⱑ �Ѿ������ iocp������ �ǹ̰� ����.
							newolp->dwFlags,
							&newolp->overlapped,					// Overlapped ����ü 
							NULL );
		
		// ���� ó�� 
		if(ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			ShowMessage(SENDPOSTPENDING_FAILED);
			ReleaseSendPacket(newolp);
			return FALSE;
		}
		return TRUE;	
	}

	BOOL NFConnection::SendPost( NaveNet::NFPacket& Packet )		// ���� Send ó��
	{
		// ������ ���� üũ���� �����Ѵ�.
		Packet.EncryptPacket();

		INT Len = Packet.m_Header.Size;
		CHAR* pPacket = (CHAR*)&Packet;
		return SendPost(pPacket, Len);
	}

	BOOL NFConnection::RecvPost(UINT buflen)
	{
		if(m_Socket == NULL || IsConnect() == FALSE)
		{
			ShowMessage(RECVPOST_FAILED);
			return FALSE;
		}

		// prepare recieve buffer
		LPOVERLAPPEDPLUS newolp = PrepareRecvPacket(buflen);

		// ����� �Ҵ� �޾Ҵ��� ����
		if(newolp == NULL)	return FALSE;

		INT ret = WSARecv(	newolp->sckClient,
							&newolp->wbuf,
							1,
							&newolp->dwBytes,						// ���� ȣ�������� �ٷ� �޾Ҵٸ� ����� ���� ũ�Ⱑ �Ѿ������ iocp������ �ǹ̰� ����.
							&newolp->dwFlags,
							&newolp->overlapped,					// Overlapped ����ü 
							NULL );
		
		// ���� ó�� 
		if(ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			ShowMessage(RECVPOSTPENDING_FAILED);
			ReleaseRecvPacket(newolp);
			return FALSE;
		}
		return TRUE;
	}

	BOOL NFConnection::GetClientIP(INT *iIP)
	{
		if(iIP == NULL) return FALSE;

		if(!IsConnect())
		{
			iIP[0] = 0;
			return FALSE;
		}

		iIP[0] = m_Peer.sin_addr.S_un.S_un_b.s_b1;
		iIP[1] = m_Peer.sin_addr.S_un.S_un_b.s_b2;
		iIP[2] = m_Peer.sin_addr.S_un.S_un_b.s_b3;
		iIP[3] = m_Peer.sin_addr.S_un.S_un_b.s_b4;

		return TRUE;
	}

	BOOL NFConnection::GetClientIP(CHAR *szIP)
	{
		if(szIP == NULL) return FALSE;

		if(!IsConnect())
		{
			szIP[0] = NULL;
			return FALSE;
		}
		sprintf(szIP,"%d.%d.%d.%d",(INT)m_Peer.sin_addr.S_un.S_un_b.s_b1,
									(INT)m_Peer.sin_addr.S_un.S_un_b.s_b2,
									(INT)m_Peer.sin_addr.S_un.S_un_b.s_b3,
									(INT)m_Peer.sin_addr.S_un.S_un_b.s_b4);

		return TRUE;
	}

	BOOL NFConnection::GetClientIP(WCHAR *szIP)
	{
		if(szIP == NULL) return FALSE;

		if(!IsConnect())
		{
			szIP[0] = NULL;
			return FALSE;
		}
		swprintf(szIP, L"%d.%d.%d.%d",(INT)m_Peer.sin_addr.S_un.S_un_b.s_b1,
									(INT)m_Peer.sin_addr.S_un.S_un_b.s_b2,
									(INT)m_Peer.sin_addr.S_un.S_un_b.s_b3,
									(INT)m_Peer.sin_addr.S_un.S_un_b.s_b4);

		return TRUE;
	}

	BOOL NFConnection::GetClientIP(sockaddr_in& addr)
	{
		if(!IsConnect())
		{
			memcpy(&addr,0,sizeof(sockaddr_in));
			return FALSE;
		}
		CopyMemory((CHAR*)&addr,(CHAR*)&m_Peer,sizeof(sockaddr_in));
		return TRUE;
	}

	VOID NFConnection::Disconnect(BOOL bForce)
	{
		// ���� �Ǿ��ִ��� Ȯ�� 
		if(!IsConnect()) 
			return;	

		// ���� �ʱ�ȭ 
		InterlockedExchange((LONG*)&m_eConnectFlag,CONNECT_NONE);
		InterlockedExchange((LONG*)&m_bIsConnect,FALSE);
		m_uRecvTickCnt = 0;
		m_bForce		= FALSE;

		// ���ϰ� ������ ���� Ȯ�� 
		if(m_Socket)
		{
			struct linger li = {0, 0};								// Default: SO_DONTLINGER
			shutdown(m_Socket, SD_BOTH );						

			// ���Ͽ� ó�� ���� ���� ����Ÿ�� ���� ���ۿ� �����ִٸ�,
			if(bForce) li.l_onoff = 1; // SO_LINGER, timeout = 0

			// �ܿ� ����Ÿ�� ������ ����
			setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (CHAR *)&li, sizeof(li));
			closesocket(m_Socket);									// ���� �ݱ� 
			m_Socket = NULL;										// �ʱ�ȭ 

			ShowMessage(CLOSE_SOCKET);
		}

		OnDisconnect();

		Clear();
	}

	BOOL NFConnection::IsConnect()
	{
		BOOL bRet = m_bIsConnect;
		return bRet;
	}

	LONG NFConnection::GetRecvTickCnt()
	{
		LONG lRecvTickCount = m_uRecvTickCnt;
		return lRecvTickCount;
	}
}