/** 
 *  @file  		NFServerCtrl.h
 *  @brief 		IOCP Base ��Ʈ�ѷ� Ŭ����
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

#include "NFConnection.h"
#include <vector>

namespace NaveServer {

//	extern INT iMaxRecvPacket;

	typedef struct LISTENER
	{
		SOCKET	s;		// Listen Socket
		INT		nPort;	// ��Ʈ
		CHAR	cBindQue;	// ���ε� ť

		VOID Init()
		{
			s = INVALID_SOCKET;		// Listen Socket
			nPort = 0;	// ��Ʈ
			cBindQue = 0;
		}

		VOID Release()
		{
			// Ŭ���� ���� ���� ť�� ����Ÿ�� ������ ���� �����ϴ� �ɼ�
			struct linger li = {1, 0};	// Default: SO_DONTLINGER
			shutdown(s, SD_BOTH);
			setsockopt(s, SOL_SOCKET, SO_LINGER, (CHAR *)&li, sizeof(li));
			closesocket(s );

			Init();
		}
	}*LPLISTENER;

	class NFServerCtrl
	{
	public:
		NFServerCtrl();
		virtual ~NFServerCtrl();

	public:
		SOCKET						CreateListenSocket(INT nServerPort, CHAR cBindQue = 32);		// Listen Socket ���� 
		HANDLE						ConnectIOCPSocket(SOCKET sckListener);		// Listen Socket �� iocp ���Ͽ� ����

		inline HANDLE				GetIOCP() { return m_hIOCP; };

		inline BOOL					IsRun() { return m_bServerRun; };
		inline BOOL					IsPause() { return m_bPause; };

		static unsigned __stdcall	WINAPI	Thread_MainEx(LPVOID lpVoid);		// IOCP ���� ���� Thread
		static unsigned __stdcall	WINAPI	Process_MainEx(LPVOID lpVoid);		// Process ó�� Thread
		static unsigned __stdcall	WINAPI	Packet_MainEx(LPVOID lpVoid);		// Packet ó�� Thread

		virtual VOID				Update();									// Process ó�� �Լ�

		virtual VOID				ShowServerInfo();							// ���۽� ȭ�鿡 ���� ���� ǥ��

		virtual BOOL				CreateSession(SOCKET sckListener) { return TRUE; }		// Client Session �� �����Ѵ�.
		virtual BOOL				ReleaseSession() { return TRUE; }	// Client Session �� �����Ѵ�.

	protected:

		HANDLE						CreateIOCP();								// IOCP �ڵ� ����
		BOOL						InitSocket();								// ���� ���̺귯�� Ȱ��
		INT							GetNumberOfProcess();						// CPU���� ���Ѵ� => ������ ������ �̿� 
		INT							GetNumberOfThread() { return m_nMaxThreadNum; };	// ������ �������� ������ ���Ѵ�.

		BOOL						Start(INT nPort, INT nMaxConn, INT nMaxThreadNum = 0);	// ���� ����
		VOID						Pause(BOOL bPause);
		BOOL						Stop();										// ���� ���� 


	protected:
		INT							m_nMaxThreadNum;							// �ִ� ������ �� ���� 
		HANDLE*						m_pWorkThread;								// IOCP Work ������ �ڵ� 
		HANDLE						m_hIOCP;									// IOCP �ڵ� 

		std::vector<LISTENER>		m_vecListener;								// ������ ����Ʈ �������� �����ʸ� ������ ���� �ִ�.

		HANDLE						m_hProcThread;								// ���μ����� ���� ������
		HANDLE						m_hPacketThread;							// PacketUpdate�� ���� ������

		INT							m_iPort;
		INT							m_iMaxConn;

		BOOL						m_bPause;
		BOOL						m_bServerRun;								// ���� ���� ���� ����Ǹ� TRUE����.

		NFUpdateManager*			m_pUpdateManager;
	};

}