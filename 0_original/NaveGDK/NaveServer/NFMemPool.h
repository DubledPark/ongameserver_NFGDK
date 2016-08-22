/** 
 *  @file  		NFMemPool.h
 *  @brief 		Packet Memory ������ü
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

#include <Nave/NFSync.h>
#include <Nave/NFLog.h>

namespace NaveServer {

#pragma pack(push, struct_packing_before)
#pragma pack(1)													

	/** 
	 *  @class		NFMemPool   
	 *  @brief      IOCP���� ����� ��Ŷ ���۸� ������ �޸�Pool Ŭ����  
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-08-28
	 */
	template <class TYPE>
	class NFMemPool
	{
	public:
		/// �Ҵ� ���� ������ ���� ������ �Է� 
		NFMemPool(INT nTotalCnt, INT nAllocBufferSize=0);			
		/// �Ҵ� ���� ó�� 
		~NFMemPool();												

		/**
		 * @brief	�α������� �̸��� �����մϴ�.
		 * @param strLogTitle �α��� �̸�
		 */
		VOID SetLogTitle(WCHAR* strLogTitle)
		{
			wcscpy(m_strLogTitle, strLogTitle);
		}

		/// ���� �Ҵ� ���� �� 
		INT GetCount();												
		/// ��� �ִ� ��Ŷ �� 
		INT GetEmpty();												
		/// ���ۿ� ���� �ε��� ��� 
		INT GetIndex( TYPE *ptr );									
		
		/// �Ҵ�� ��Ŷ�� �� ��Ŷ ��ȯ
		TYPE* Alloc();												
		/// �Ҵ� ���� �޸𸮸� ���� 
		BOOL Free( TYPE *ptr );										

	private:
		/// INDEX Structure  
		struct INDEX
		{
			/// ���� Index �����͸� ������ 
			INDEX* pNext;											
			/// ���� �ε��� ��ȣ 
			INT nIndex;												

			/// Structure constructure
			INDEX(){
				pNext = NULL;									
				nIndex = 0;
			}
		}; 

	private:
		/// ���� �Ҵ� ���� �� 											
		INT		m_nCount;											
		/// ������� �Ҵ� ��Ŷ �� 
		INT		m_nTotalCnt;										
		/// �� ��Ŷ�� �Ҵ� ���� ���� ������ 
		INT		m_nAllocBufferSize;									
		/// �� ��Ŷ�� ��ü ������ 
		INT		m_nPerPacketSize;									

		/// �α����� �̸�
		WCHAR	m_strLogTitle[32];									
		/// ��ü �Ҵ� ������ 
		PVOID	m_pPakets;											
		/// ���� ����ִ� ó�� �ε��� ������ 
		INDEX*	m_pFreeRoom;										

		Nave::NFSync	m_Sync;
	};
#pragma pack( pop, struct_packing_before )


	template <class TYPE>
	NFMemPool<TYPE>::NFMemPool( INT nTotalCnt, INT nAllocBufferSize )
	{
		assert( nTotalCnt > 0 );

		m_nCount	= 0;
		m_pFreeRoom = NULL;
		m_pPakets	= NULL;

		m_nTotalCnt			= nTotalCnt;							// MAXUSER * 2 
		m_nAllocBufferSize	= nAllocBufferSize;						// 1024 + 64
		m_nPerPacketSize	= sizeof(INDEX) + sizeof(TYPE) + m_nAllocBufferSize;

		m_pPakets = VirtualAlloc( NULL, 
								m_nTotalCnt * m_nPerPacketSize,
								MEM_RESERVE | MEM_COMMIT,         // reserve and commit
								PAGE_READWRITE );

			////////////////////////////////////////////////////////// 
			//				< �Ҵ� ����Ÿ ���� >					// 
			//													    // 
			//	-----------------------------------------------     // 
			//  |    INDEX   |   TYPE  |    m_nExBlockSize     |    // 
			//  -----------------------------------------------     // 
			////////////////////////////////////////////////////////// 	

		assert( m_pPakets );
		INDEX* pIndex = (INDEX*) m_pPakets;
		assert( pIndex );

		//////////////////////////////////////////
		// init linear linked list for buffer pool

		// �Ҵ� ����Ÿ ���� �� ������ ������ ������ ��� 
		pIndex = (INDEX*) ((DWORD)pIndex + (m_nTotalCnt - 1) * m_nPerPacketSize);

		// �Ҵ� ����Ÿ ������ ������ ���� Linked List�� �����Ͽ� �ö���� 
		for( INT i = m_nTotalCnt-1; i >= 0; i-- )
		{
			pIndex->pNext = m_pFreeRoom;								// �� �������� pNext = NULL�̴� 
			pIndex->nIndex = i;											// INDEX �۾� 

			m_pFreeRoom = pIndex;										// ���� ������ ���� ���� 

	#ifdef _DEBUG
			// �� �� ���� ExBlockSize�� �κп� ����Ÿ ���� �۾� 
			if( m_nAllocBufferSize )
				memset((PVOID)( (DWORD)pIndex+sizeof(INDEX)+sizeof(TYPE)),
								(i%10+'0'), 
								m_nAllocBufferSize );
	#endif
			// ���� ����Ʈ�� C8List ����Ʈ �̵� 
			pIndex = (INDEX*)((DWORD)pIndex - m_nPerPacketSize);
		}
	}

	template <class TYPE>
	NFMemPool<TYPE>::~NFMemPool()
	{
		if(NULL != m_pPakets) VirtualFree( m_pPakets, 0, MEM_RELEASE );
	}

	template <class TYPE>
	TYPE* NFMemPool<TYPE>::Alloc()
	{
		// ���� �ʱ�ȭ 
		INDEX* pIndex = NULL;							
		TYPE* pType = NULL;

		Nave::NFSyncLock CL(&m_Sync);

		pIndex = m_pFreeRoom;
		if( pIndex != NULL )
		{
			// ����� �̵� �ε��� �����͸� ���� �ε����� �̵� 
			m_pFreeRoom = m_pFreeRoom->pNext;						
			m_nCount++;												// ī��Ʈ ���� 
			// ������ �ε��� �����Ϳ��� ���ø� Ÿ�� ��Ƽ ������ ��ȯ 
			pType = (TYPE*)(pIndex+1);								
			
			///////////////////////////////////////////
			// ġ������ ���� �߻� 
			// assert( m_nCount > 0 );									// make sure we don't overflow
			if(m_nCount > m_nTotalCnt)
			{
				LOG_ERROR((L"[%s] MemPool Alloc Overflow Count(%d) >= %d", m_strLogTitle, m_nCount, m_nTotalCnt));
				return NULL;
			}
		}
		else
			LOG_ERROR((L"[%s] MemPool Alloc m_pFreeRoom = NULL, Count(%d)", m_strLogTitle, m_nCount));

		return pType;												// ���ø� Ÿ�� ��ȯ 
	}

	template <class TYPE>
	BOOL NFMemPool<TYPE>::Free( TYPE *ptr )
	{
		Nave::NFSyncLock CL(&m_Sync);

		BOOL bRet = FALSE;

		INDEX* pIndex = ((INDEX*)(ptr))-1;							// ��� ��Ŷ�� �ε��� ������ ��� 

		// ������� ��Ŷ�� �ִٸ� 
		if( m_nCount > 0 )
		{
			// ����� �ε��� �����͸� ������ ���� ��ƾ 
			pIndex->pNext = m_pFreeRoom;							
			m_pFreeRoom = pIndex;
			m_nCount--;												// ��� ��Ŷ�� 1 ���� 
			bRet = TRUE;											// ���� ó�� �˸� 
		}
		else
			LOG_ERROR((L"[%s] MemPool Free Faild Count(%d) <= 0", m_strLogTitle, m_nCount));

		return bRet;												// ���� ó�� ���� ��ȯ 
	}

	template <class TYPE>
	INT NFMemPool<TYPE>::GetCount()
	{
		Nave::NFSyncLock CL(&m_Sync);

		INT nRet = m_nCount;										// ���� ���� 
		return nRet;												// ��ȯ 
	}

	template <class TYPE>
	INT NFMemPool<TYPE>::GetEmpty()
	{
		Nave::NFSyncLock CL(&m_Sync);

		INT nRet = m_nTotalCnt - m_nCount;							// ���� 
		return nRet;												// ��ȯ 
	}

	template <class TYPE>
	INT NFMemPool<TYPE>::GetIndex( TYPE *ptr )
	{
		Nave::NFSyncLock CL(&m_Sync);

		INT nRet = 0;												// ���� �ʱ�ȭ 
		INDEX* pIndex = ((INDEX*)(ptr))-1;							// �ε��� ������ ��� 
		nRet = pIndex->nIndex;										// ���� ���� 
		return nRet;												// ��ȯ 
	}

}