/** 
 *  @file  		NFServerLink.h
 *  @brief 		Server To Server �� Connect ��ü
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

namespace NaveServer {

	// NFServerLink�� Server�� NFServerCtrl�� Connection�� �ϱ����� ���۵� ����ü�̴�
	// NFServerLink�� ������ Create�� Server�� Connection�ϰ� �Ѵ�.

	class NFServerLink : public NFConnection
	{
	public:
		NFServerLink(VOID);
		virtual ~NFServerLink(VOID);

	public:
		virtual VOID		Clear() { return; };

		virtual BOOL		Close_Open(BOOL bForce=FALSE );

		// �� ��ü ���� 
		virtual BOOL		Create(	DWORD			dwIndex,
									HANDLE			hIOCP,
									NFPacketPool*	pPacketPool,
									WCHAR*			sIP,
									INT				nPort,
									INT				nMaxBuf);

		virtual BOOL		SendOpenPacket() { return TRUE; };

	protected:
		// Socket�� ������ ���� �� ���Ĺ��� ���� ����  
		virtual BOOL		Open();

		// ���� ó�� ���� �Լ� 
		virtual INT			DispatchPacket( LPOVERLAPPEDPLUS lpOverlapPlus );

		// ���� ��Ŷ�� ó���ϴ� �κ��̴�.
		virtual VOID		DispatchPacket( NaveNet::NFPacket& Packet ) 
		{
			// Nave::Sync::CSSync Live(&m_Sync);
		};

	protected:
		WCHAR	m_strIP[32];
		INT		m_iPort;
	};

}