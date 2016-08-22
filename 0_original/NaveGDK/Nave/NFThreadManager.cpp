#include "Nave.h"
#include "NFThread.h"
#include "NFThreadManager.h"

namespace Nave { 

	HANDLE NFThreadManager::Run(NFThread* lpThread) 
	{
		unsigned int nThreadID = 0;
		HANDLE hThread = reinterpret_cast<HANDLE>(_beginthreadex(0, 
			0, NFThread::ThreadFunc, lpThread, 0, &nThreadID));

		lpThread->SetHandle(hThread);
		return hThread;
	}

	BOOL NFThreadManager::Stop(NFThread* lpThread, unsigned long dwTimeout)
	{
		if(0 == lpThread)
		{
			return FALSE;
		}

		HANDLE hThread = lpThread->GetHandle();
		if(INVALID_HANDLE_VALUE == hThread)
		{
			return FALSE;
		}

		lpThread->SetHandle(INVALID_HANDLE_VALUE);
		lpThread->End();
		WaitForSingleObject(hThread, dwTimeout);
		return (TRUE == CloseHandle(hThread));
	}

	NFThreadManager::NFThreadManager() : m_nThreadNum(0), m_bUnRegStarted(FALSE)
	{
		for(int i = 0; i < MAX_THREAD_NUM; ++i)
		{
			m_lpThreads[i] = NULL;
			m_hThreads[i] = NULL;
		}
	}

	NFThreadManager::~NFThreadManager()
	{
		UnRegister();
	}

	BOOL NFThreadManager::Register(NFThread* llpThread)
	{		
		unsigned int nThreadID = 0;
		unsigned int nThreadIndex = 0;

		if(0 == llpThread)	
		{		
			return FALSE;
		}

		//	Lock
		{
			Nave::NFSyncLock Sync(&m_ThreadLock);

			if(MAX_THREAD_NUM <= m_nThreadNum || TRUE == m_bUnRegStarted)
			{
				return FALSE;	
			}

			nThreadIndex = m_nThreadNum;
			++m_nThreadNum;
		}

		m_lpThreads[nThreadIndex] = llpThread;

		m_hThreads[nThreadIndex] = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, 
			NFThread::ThreadFunc, llpThread, 0, &nThreadID));

		return (0 != m_hThreads[nThreadIndex]);
	}

	BOOL NFThreadManager::UnRegister()
	{	
		{			
			Nave::NFSyncLock Sync(&m_ThreadLock);

			if(0 == m_nThreadNum)
			{
				return TRUE;
			}

			// 조인이 시작되면, 더이상의 스레드 생성 및 등록을 금지함.
			m_bUnRegStarted = TRUE;
		}

		// 스레드 전부 종료 & 대기.
		for(int i = 0; i < (int)m_nThreadNum; ++i)
		{
			if(m_lpThreads[i])
			{
				m_lpThreads[i]->End();
			}
		}

		WaitForMultipleObjects(m_nThreadNum, m_hThreads, TRUE, INFINITE);

		// 스레드 소멸.
		for(int i = 0; i < MAX_THREAD_NUM; ++i)
		{
			_DELETE(m_lpThreads[i]);

			if(m_hThreads[i])
			{
				CloseHandle(m_hThreads[i]);
				m_hThreads[i] = NULL;
			}
		}

		{
			// 스레드 개수 및 조인 여부 리셋.
			Nave::NFSyncLock Sync(&m_ThreadLock);

			m_nThreadNum = 0;
			m_bUnRegStarted = FALSE;
		}

		return TRUE;
	}

}