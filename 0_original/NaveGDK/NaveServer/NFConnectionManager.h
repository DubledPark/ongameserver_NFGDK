/** 
 *  @file  		NFConnectionManager.h
 *  @brief 		Connection�� �����ϴ� ������ü
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

#include <Nave/NFSync.h>
#include <vector>

namespace NaveServer { 

	/** 
	 *  @class        NFConnectionManager
	 *  @brief        Ư���� Ŀ�ؼ� ����Ʈ�� �����ϱ� ���� �޴��� ��ü ���ÿ� �������� ����� �ִ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-08-28
	 */
	class NFConnectionManager
	{
	public:
		NFConnectionManager(VOID);
		~NFConnectionManager(VOID);

	public:
		/**
		 * @brief	�޴��� �ʱ�ȭ
		 * @param iMaxCount ���� ������ �ִ� Ŀ�ؼ� ����
		 */
		VOID Init(INT iMaxCount);

		/**
		 * @brief	������ ��Ͻ�Ų��.
		 * @param pUser ������ ���ᰴü ������
		 */
		VOID Join(NFConnection* pUser);

		/**
		 * @brief	�ش� �ε����� ��ü�� �����Ѵ�.
		 * @param iIndex ������ �ε��� 
		 */
		VOID Levae(INT iIndex);	
		
		/**
		 * @brief	���� ��ϵ� ���ᰴü ���� ���
		 * @return  Ŀ�ؼ� ����
		 */
		INT	GetCount() { return m_iCount; }

		/**
		 * @brief	�����ϴ� ���ᰴü �ִ� ����
		 * @return  �ִ� ���ᰴü ����
		 */
		INT GetMaxCount() { return m_iMaxCount; }

		/**
		 * @brief	��ϵ� ��� ���ᰴü�� ��Ŷ�� �����Ѵ�.
		 * @param pPackte ���� ��Ŷ ����
		 * @param Len ���� ��Ŷ ������
		 */
		virtual VOID		SendPostAll( CHAR* pPackte, INT Len );

		/**
		 * @brief	��ϵ� ��� ���ᰴü�� ��Ŷ�� �����Ѵ�.
		 * @param Packet ���� ��Ŷ ����ü
		 */
		virtual VOID		SendPostAll( NaveNet::NFPacket&	Packet);

		VOID Draw();

	private:
		/// ���ᰴü ���� ����
		std::vector<NFConnection*>						m_vecConn;
		typedef std::vector<NFConnection*>::iterator	iterConn;

		/// ���� ��ϵ� ���ᰴü�� ����
		INT												m_iCount;
		/// ������ ������ �ִ� ���ᰴü�� ����
		INT												m_iMaxCount;

		/// Sync ��ü
		Nave::NFSync									m_Lock;
	};

}