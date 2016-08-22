/** 
 *  @file  		NFConnection.h
 *  @brief 		�ϳ��� Connection�� �����ϴ� ��ü�̴�.
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
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
	 *  @brief        �ϳ��� ���ϰ�ü�� ��Ÿ���� 1���� Ŭ���̾�Ʈ�� ��Ÿ����.
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
		// �� ��ü ���� 
		/**
		 * @brief	��� ���� ���� �� ��ü �ʱ�ȭ
		 * @param dwIndex		��ü ���� ID
		 * @param hIOCP			IOCP Handle
		 * @param listener		Listen Socket
		 * @param pPacketPool	��Ŷ Pool ������
		 * @param nMaxBuf		�ִ� ����ũ��
		 * @return ��������
		 */
		virtual BOOL		Create( DWORD			dwIndex,
									HANDLE			hIOCP,
									SOCKET			listener,
									NFPacketPool*	pPacketPool,
									INT				nMaxBuf);

		/**
		 * @brief	��Ŷ�� ������Ʈó�� �մϴ�. 
		 * @param Packet ������Ʈ �� ��Ŷ����
		 */
		virtual VOID		UpdatePacket(NaveNet::NFPacket& Packet);

		/**
		 * @brief	���� ���� ���� 
		 * @param bForce TRUE�� ���Ͽ� ���� ������ ����
		 */
		VOID				Disconnect( BOOL bForce = FALSE );		

		/**
		 * @brief	���� �������� �����մϴ�.
		 */
		virtual VOID		Clear() { return; };

		/**
		 * @brief	��ü�� Ŭ���̾�Ʈ ���� ���� �� �ʱ�ȭ
		 * @param bForce TRUE�� ���Ͽ� ���� ������ ����
		 * @return  ��������
		 */
		virtual BOOL		Close_Open( BOOL bForce = FALSE );

		/**
		 * @brief	IOCP ó�� �ڵ鸵
		 * @param lpOverlapPlus	ó���� ��Ŷ
		 * @return 
		 */
		virtual BOOL		DoIo( LPOVERLAPPEDPLUS lpOverlapPlus );	

		/**
		 * @brief	��Ŷ ����
		 * @param pPackte ���� ��Ŷ ������
		 * @param Len ���� ��Ŷ ũ��
		 * @return  ��������
		 */
		virtual BOOL		SendPost( CHAR* pPackte, INT Len );	

		/**
		 * @brief	��Ŷ ����
		 * @param Packet ���� ��Ŷ ����ü
		 * @return  ��������
		 */
		virtual BOOL		SendPost( NaveNet::NFPacket&	Packet);

		/**
		 * @brief	�������� IP��� 
		 * @param iIP ������ 4�� �迭
		 * @return 
		 */
		BOOL				GetClientIP( INT* iIP );					 
		/**
		 * @brief	�������� IP��� 
		 * @param szIP ������
		 * @return 
		 */
		BOOL				GetClientIP( CHAR* szIP );				
		/**
		 * @brief	�������� IP��� 
		 * @param szIP �����ڵ� ������
		 * @return 
		 */
		BOOL				GetClientIP( WCHAR* szIP );				
		/**
		 * @brief	�������� IP��� 
		 * @param addr sockaddr_in ����
		 * @return 
		 */
		BOOL				GetClientIP( sockaddr_in& addr );			

		/**
		 * @brief	���ᰳü�� ���� �ε����� �����մϴ�.
		 * @param iIndex ���ᰳü�� ���� �ε���
		 */
		inline VOID			SetIndex(INT iIndex) { m_dwIndex = iIndex; };

		/**
		 * @brief	���ᰳü�� ���� �ε��� ���
		 * @return  �ε�����
		 */
		inline DWORD		GetIndex() { return m_dwIndex; }

		/**
		 * @brief	���� Tick ���
		 * @return  ���� Tick ī��Ʈ
		 */
		LONG				GetRecvTickCnt();

		/**
		 * @brief	���� ��ü�� Ȱ��ȭ ���� ���
		 * @return  Ȱ��ȭ ����
		 */
		BOOL				IsConnect();

		/**
		 * @brief	���� ��ü�� ���¸� �����մϴ�.
		 * @param eState	���� ��ü�� ���� ���� 
		 */
		void				SetConnectFlag(CONNECT_EVENT eState);

		/**
		 * @brief	���޵� Overlapped��ü�� �̿��� ���� ��ü�� �����Ų�� �ʱ�ȭ ��ŵ�ϴ�.
		 * @param lpOverlapPlus Overlapped ��ü
		 * @param bForce   TRUE�� ���Ͽ� ���� ������ ����
		 */
		void				SetClose_Open(LPOVERLAPPEDPLUS lpOverlapPlus, BOOL bForce=FALSE );

		//------------------------ �� �� �� �� -------------------------//
	protected:

		/**
		 * @brief	��ü �ʱ�ȭ, ���� ���� ����					
		 * @return  ��������
		 */
		virtual BOOL		Open();

		/**
		 * @brief	Socket�� IOCP ���ε� �۾�
		 * @param lpOverlapPlus ���ε� ��ų Overlapped ����ü
		 * @return 
		 */
		BOOL				BindIOCP( LPOVERLAPPEDPLUS lpOverlapPlus );

		/**
		 * @brief	Accept��Ŷ �Ҵ�
		 * @return  �Ҵ�� Overlapped ����ü 
		 */
		LPOVERLAPPEDPLUS	PrepareAcptPacket();

		/**
		 * @brief	IOCP�� �̿��� Recv Overlapped ����
		 * @param buflen ���� ũ��
		 * @return  �Ҵ�� Overlapped ����ü 
		 */
		LPOVERLAPPEDPLUS	PrepareRecvPacket(UINT buflen);

		/**
		 * @brief	IOCP�� �̿��� Send Overlapped ����
		 * @param *psrcbuf ���� ������ ������
		 * @param srclen   ���� ũ��
		 * @return  �Ҵ�� Overlapped ����ü 
		 */
		LPOVERLAPPEDPLUS	PrepareSendPacket(CHAR *psrcbuf, UINT srclen);

		/**
		 * @brief	Accept ��Ŷ ����  							
		 * @param lpOverlapPlus ������ Overlapped����ü
		 * @return  ��������
		 */
		BOOL				ReleaseAcptPacket(LPOVERLAPPEDPLUS lpOverlapPlus);

		/**
		 * @brief	Receive ��Ŷ ����  							
		 * @param lpOverlapPlus ������ Overlapped����ü
		 * @return  ��������
		 */
		BOOL				ReleaseRecvPacket(LPOVERLAPPEDPLUS lpOverlapPlus);

		/**
		 * @brief	Send ��Ŷ ����
		 * @param lpOverlapPlus ������ Overlapped����ü
		 * @return  ��������
		 */
		BOOL				ReleaseSendPacket(LPOVERLAPPEDPLUS lpOverlapPlus);

		/**
		 * @brief	���� Receive ó��
		 * @param buflen  ���� ���� ������
		 * @return  ��������
		 */
		BOOL				RecvPost(UINT buflen=0);

		/**
		 * @brief	�α׿� �޽����� ����մϴ�.
		 * @param MsgIndex �޽��� ���̵�
		 */
		virtual VOID		ShowMessage(INT MsgIndex);

		/**
		 * @brief	 �����ڿ��� Connect �̺�Ʈ �߻�
		 * @param bConnect Connect ��������
		 */
		virtual VOID		OnConnect(BOOL bConnect) { };

		/// Disconnect �̺�Ʈ �߻�
		virtual	VOID		OnDisconnect() { };

		// ���� ó�� ���� �Լ� 
		/**
		 * @brief	�޼��� ���� �� Receive ��ȣ �߻�, Socket����
		 * @param lpOverlapPlus Overlapped ����ü
		 * @return  Recv ó�� ���� ���� 
		 */
		virtual BOOL		DispatchPacket( LPOVERLAPPEDPLUS lpOverlapPlus );

		/**
		 * @brief	���� ��Ŷ�� ó���ϴ� �κ��̴�
		 * @param Packet ó���� ��Ŷ ����ü
		 */
		virtual VOID		DispatchPacket( NaveNet::NFPacket& Packet ) 
		{
			// ��ӹ޾Ƽ� ��Ŷ�� ó���Ҷ��� �Ʒ��� ���� ���� ������ ����ؼ� ����Ѵ�.
			//Nave::Sync::CSSync Live(&m_Sync);
		};

	protected:
		/// �ִ� ���� ������ 
		INT					m_nMaxBuf;							

		/// �ִ� ��Ŷ ������
		INT					m_nMaxPacketSize;						

		/// Ŭ���̾�Ʈ ���� ���� 
		SOCKET				m_Socket;								
		
		/// Listener ���� 
		SOCKET				m_sckListener;							

		/// ���� ������ ������ �ִ� ��ü 
		sockaddr_in			m_Local;	

		/// Ŭ���̾�Ʈ ������ ������ �ִ� ��ü 
		sockaddr_in			m_Peer;									
		
		/// �� Ŭ���̾�Ʈ ���� ��ü�� ��ȣ 
		INT					m_dwIndex;								

		/// ���� ����Ʈ �˻� ���� 
		INT					m_nBytesSent;							

		/// IOCP �ڵ� 	
		HANDLE				m_hIOCP;								

		//�� Ŭ������ IOCP���� ��Ŷ�� �ְ� ������ Socket�� ���� ����Ǵ� ���۸� �����Ѵ�.
		/// ��ŶǮ ������ 
		NFPacketPool*		m_pPacketPool;							

		/// �� ��ü�� Ŭ���̾�Ʈ�� ���� �˻�
		BOOL				m_bIsConnect;							
		
		/// ���� Tick Count 
		LONG				m_uRecvTickCnt;							

		/// Ŀ�ؼ� ���� ����
		CONNECT_EVENT		m_eConnectFlag;							

		/// ���� ���� �ʱ�ȭ ���� ����
		BOOL				m_bForce;								

		/// Sync ��ü
		Nave::NFSync		m_Lock;

		// checksum ó�� ///////////////////////////////////////////// 
		// CPacketIOBuffer�� Recv���� ��Ŷ ����(�̶� ��ŶǮ�� ���ۿ� �ִ�)�� �����ϴ� IOBuffer �̴�.
		// IOBuffer�� ����ϴ� ������ Recv ���� ��Ŷ�� ������ ��Ŷ�� �ƴҶ� ó���ϱ� ���� �ϳ���
		// �����̴�.
		
		/// Recv�� IOBuffer
		NaveNet::NFPacketIOBuffer		m_RecvIO;								
		
		// CPacket�� �ϳ��� ��Ŷ�� �����ϴ� Ŭ������ IOBuffer ���� ��Ŷ�� ���� �����Ҷ� ���ȴ�.
		// Packet���� ��Ŷ ���۰� �迭�� ���ǵǾ� �ִ�. (memcpy�� ����ϱ� ������ �����ε尡 �߻��Ѵ�.)
		/// Recv�� �ϱ����� ����ϴ� �ӽ� ��Ŷ ����
		NaveNet::NFPacket				m_RecvPacket;
	};

}