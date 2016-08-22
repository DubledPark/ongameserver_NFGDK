/** 
 *  @file  		NFMemPool.h
 *  @brief 		Packet Memory 관리객체
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
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
	 *  @brief      IOCP에서 사용할 패킷 버퍼를 관리할 메모리Pool 클래스  
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
		/// 할당 받을 갯수와 버퍼 사이즈 입력 
		NFMemPool(INT nTotalCnt, INT nAllocBufferSize=0);			
		/// 할당 해제 처리 
		~NFMemPool();												

		/**
		 * @brief	로그파일의 이름을 결정합니다.
		 * @param strLogTitle 로그의 이름
		 */
		VOID SetLogTitle(WCHAR* strLogTitle)
		{
			wcscpy(m_strLogTitle, strLogTitle);
		}

		/// 현재 할당 받은 수 
		INT GetCount();												
		/// 비어 있는 패킷 수 
		INT GetEmpty();												
		/// 버퍼에 따른 인덱스 얻기 
		INT GetIndex( TYPE *ptr );									
		
		/// 할당된 패킷중 빈 패킷 반환
		TYPE* Alloc();												
		/// 할당 받은 메모리를 해제 
		BOOL Free( TYPE *ptr );										

	private:
		/// INDEX Structure  
		struct INDEX
		{
			/// 다음 Index 포인터를 가진다 
			INDEX* pNext;											
			/// 현재 인덱스 번호 
			INT nIndex;												

			/// Structure constructure
			INDEX(){
				pNext = NULL;									
				nIndex = 0;
			}
		}; 

	private:
		/// 현재 할당 받은 수 											
		INT		m_nCount;											
		/// 만들어진 할당 패킷 수 
		INT		m_nTotalCnt;										
		/// 한 패킷당 할당 받은 버퍼 사이즈 
		INT		m_nAllocBufferSize;									
		/// 한 패킷당 전체 사이즈 
		INT		m_nPerPacketSize;									

		/// 로그파일 이름
		WCHAR	m_strLogTitle[32];									
		/// 전체 할당 포인터 
		PVOID	m_pPakets;											
		/// 현재 비어있는 처리 인덱스 포인터 
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
			//				< 할당 데이타 블럭도 >					// 
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

		// 할당 데이타 구조 맨 마지막 단위의 포인터 얻기 
		pIndex = (INDEX*) ((DWORD)pIndex + (m_nTotalCnt - 1) * m_nPerPacketSize);

		// 할당 데이타 구조의 마지막 부터 Linked List를 구성하여 올라오기 
		for( INT i = m_nTotalCnt-1; i >= 0; i-- )
		{
			pIndex->pNext = m_pFreeRoom;								// 맨 마지막의 pNext = NULL이다 
			pIndex->nIndex = i;											// INDEX 작업 

			m_pFreeRoom = pIndex;										// 다음 연결을 위한 세팅 

	#ifdef _DEBUG
			// 각 블럭 마다 ExBlockSize의 부분에 데이타 세팅 작업 
			if( m_nAllocBufferSize )
				memset((PVOID)( (DWORD)pIndex+sizeof(INDEX)+sizeof(TYPE)),
								(i%10+'0'), 
								m_nAllocBufferSize );
	#endif
			// 다음 리스트로 C8List 포인트 이동 
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
		// 변수 초기화 
		INDEX* pIndex = NULL;							
		TYPE* pType = NULL;

		Nave::NFSyncLock CL(&m_Sync);

		pIndex = m_pFreeRoom;
		if( pIndex != NULL )
		{
			// 빈공간 이동 인덱스 포인터를 다음 인덱스로 이동 
			m_pFreeRoom = m_pFreeRoom->pNext;						
			m_nCount++;												// 카운트 증가 
			// 지정한 인덱스 포인터에서 템플릿 타입 위티 포인터 반환 
			pType = (TYPE*)(pIndex+1);								
			
			///////////////////////////////////////////
			// 치명적인 에러 발생 
			// assert( m_nCount > 0 );									// make sure we don't overflow
			if(m_nCount > m_nTotalCnt)
			{
				LOG_ERROR((L"[%s] MemPool Alloc Overflow Count(%d) >= %d", m_strLogTitle, m_nCount, m_nTotalCnt));
				return NULL;
			}
		}
		else
			LOG_ERROR((L"[%s] MemPool Alloc m_pFreeRoom = NULL, Count(%d)", m_strLogTitle, m_nCount));

		return pType;												// 템플릿 타입 반환 
	}

	template <class TYPE>
	BOOL NFMemPool<TYPE>::Free( TYPE *ptr )
	{
		Nave::NFSyncLock CL(&m_Sync);

		BOOL bRet = FALSE;

		INDEX* pIndex = ((INDEX*)(ptr))-1;							// 사용 패킷의 인덱스 포인터 얻기 

		// 사용중인 패킷이 있다면 
		if( m_nCount > 0 )
		{
			// 빈공간 인덱스 포인터를 앞으로 당기는 루틴 
			pIndex->pNext = m_pFreeRoom;							
			m_pFreeRoom = pIndex;
			m_nCount--;												// 사용 패킷수 1 감소 
			bRet = TRUE;											// 정상 처리 알림 
		}
		else
			LOG_ERROR((L"[%s] MemPool Free Faild Count(%d) <= 0", m_strLogTitle, m_nCount));

		return bRet;												// 정상 처리 유무 반환 
	}

	template <class TYPE>
	INT NFMemPool<TYPE>::GetCount()
	{
		Nave::NFSyncLock CL(&m_Sync);

		INT nRet = m_nCount;										// 변수 설정 
		return nRet;												// 반환 
	}

	template <class TYPE>
	INT NFMemPool<TYPE>::GetEmpty()
	{
		Nave::NFSyncLock CL(&m_Sync);

		INT nRet = m_nTotalCnt - m_nCount;							// 설정 
		return nRet;												// 반환 
	}

	template <class TYPE>
	INT NFMemPool<TYPE>::GetIndex( TYPE *ptr )
	{
		Nave::NFSyncLock CL(&m_Sync);

		INT nRet = 0;												// 변수 초기화 
		INDEX* pIndex = ((INDEX*)(ptr))-1;							// 인데스 포인터 얻기 
		nRet = pIndex->nIndex;										// 변수 설정 
		return nRet;												// 반환 
	}

}