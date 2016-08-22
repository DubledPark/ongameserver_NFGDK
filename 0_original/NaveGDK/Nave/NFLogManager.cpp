#include "Nave.h"
#include "NFLog.h"
#include "NFLogManager.h"

#include <GZip/gzip.h>
#include "NFThreadManager.h"

namespace Nave { 

	NFLogThread::NFLogThread() : m_pLogHandle(NULL), m_bCompress(TRUE),
								m_hFlush(CreateEvent(0, TRUE, FALSE, 0)),
								m_hFile(INVALID_HANDLE_VALUE), m_dwTotalWritten(0),
								m_dwMaxFileSize(MAX_FILE_SIZE)
	{
		InterlockedExchange(&m_bEnd, FALSE);
	}

	NFLogThread::~NFLogThread()
	{
		if(INVALID_HANDLE_VALUE != m_hFile)
		{ 
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}

		if(0 != m_hFlush) 
		{
			CloseHandle(m_hFlush); 
			m_hFlush = 0;
		}	
	}

	VOID NFLogThread::Initialize(NFLogManager* pLog, BOOL bCompress)
	{
		m_bCompress = bCompress;
		m_pLogHandle = pLog;
	}

	unsigned int NFLogThread::Run()
	{
		// 여기서 m_bEnd가 TRUE가 아니면 스레드가 작동하고 있다.
		for(;TRUE != InterlockedCompareExchange(&m_bEnd, TRUE, TRUE);)
		{
			// 현재 이벤트가 사용중이면 1초간 대기후 다시 확인.
			if(WAIT_TIMEOUT == WaitForSingleObject(m_hFlush, 1000))
			{
				continue;
			}
			
			WriteLog();
		};

		WriteLog();
		return 0;
	}

	BOOL NFLogThread::End()
	{
		InterlockedExchange(&m_bEnd, TRUE);
		SetEvent(m_hFlush);

		return TRUE;
	}

	BOOL NFLogThread::WriteLog()
	{
		// 로그에서 버퍼를 읽어온다.
		if(!m_pLogHandle)
			return FALSE;

		m_pLogHandle->SpliceInWriteBuffer(m_WriteBufferList);

		if(m_WriteBufferList.empty())
			return TRUE;

		if(INVALID_HANDLE_VALUE == m_hFile)
		{
			SetLogFileName();
			m_hFile = CreateFileW(m_szLogFileName, GENERIC_WRITE,
				FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			if(INVALID_HANDLE_VALUE == m_hFile)
			{
				return FALSE;
			}
		}

		unsigned long dwWritten = 0;

		for(NFLogBuffer::List::iterator itr = m_WriteBufferList.begin();
			itr != m_WriteBufferList.end(); ++itr)
		{
			NFLogBuffer* pLogBuffer = *itr;
			if(FALSE == WriteFile(m_hFile, pLogBuffer->m_Buffer, pLogBuffer->m_dwUsage, &dwWritten, 0))
			{
				LOG_ERROR((L"로그 파일 기록에 실패했습니다. ErrorNum : %d, FileHandle:0x%p, 버퍼 크기:%d",
					GetLastError(), m_hFile, pLogBuffer->m_dwUsage));
			}

			m_dwTotalWritten += dwWritten;
		}

		m_pLogHandle->SpliceInFreeBuffer(m_WriteBufferList);

		// 파일은 계속 열어놓는다.
		if(m_dwTotalWritten > m_dwMaxFileSize)
		{
			if(INVALID_HANDLE_VALUE != m_hFile) { CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE; }	// 파일 닫고...

			if(m_bCompress)
				Compress();

			m_dwTotalWritten = 0;
		}

		return TRUE;
	}

	BOOL NFLogThread::Compress()
	{
		HANDLE hFile = CreateFileW(m_szLogFileName, GENERIC_READ, 
			FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if(INVALID_HANDLE_VALUE == hFile)
		{
			LOG_ERROR((L"게임 로그를 압축할 수 없습니다."));
			return FALSE;
		}

		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		unsigned long dwSpinCount = 0;
		WCHAR szCompressedLogFileName[MAX_PATH*2];
		
		WCHAR drive[_MAX_DRIVE];
		WCHAR dir[_MAX_DIR];
		WCHAR fname[_MAX_FNAME];
		WCHAR ext[_MAX_EXT];

		_wsplitpath(m_szLogFileName, drive, dir, fname, ext);	

		while (TRUE) 
		{		
			_snwprintf(szCompressedLogFileName, MAX_PATH*2, L"%s\\%s-%04d%02d%02d-%02d%02d%02d-%04d.log.gz",
				dir, fname, sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, dwSpinCount);

			if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(szCompressedLogFileName))
			{
				break;
			}

			++dwSpinCount;
		}

		// 압축 알고리즘 추가.
		SDK::CGZip gzip;

		if (!gzip.Open(szCompressedLogFileName, SDK::CGZip::ArchiveModeWrite))
		{
			LOG_ERROR((L"압축 파일의 생성에 실패했습니다."));
			CloseHandle(hFile);
			return FALSE;
		}

		unsigned long	dwRead = 0;
		char	szBuffer[4096];

		while(FALSE != ReadFile(hFile, szBuffer, 4096, &dwRead, 0))
		{
			if(0 == dwRead)
			{
				break;
			}

			if(!gzip.WriteBuffer(szBuffer, dwRead))
			{
				LOG_ERROR((L"에러로 인해 압축 파일에 기록할 수 없습니다."));
				break;
			}
		}

		gzip.Close();

		CloseHandle(hFile);

		int nError = GetLastError();
		if(0 != nError && ERROR_HANDLE_EOF != nError)
		{
			LOG_ERROR((L"%d 에러로 인해 로그 파일 압축이 실패했습니다.", nError));
			return FALSE;
		}
		
		// 로그파일 삭제
		if(FALSE == DeleteFileW(m_szLogFileName))
		{
			LOG_ERROR((L"%d 에러로 인해 압축하고 난 로그를 삭제하지 못했습니다.", nError));
			return FALSE;
		}	

		return TRUE;
	}

	BOOL NFLogThread::SetLogFileName()
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		unsigned long dwSpinCount = 0;
		WCHAR szProgramName[MAX_PATH];
		WCHAR szLogFilePrefix[MAX_PATH];

		// 프로그램이름과 동일한 하위폴더를 생성한다.
		Nave::GetProgramName(szProgramName, MAX_PATH);

		if(m_pLogHandle)
			_snwprintf(szLogFilePrefix, MAX_PATH - 1, L"%s", m_pLogHandle->GetLogFilePrefix());
		else
			wcsncpy(szLogFilePrefix, szProgramName, MAX_PATH);

		if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(szProgramName))
		{
			if (!CreateDirectoryW(szProgramName, 0))
			{			
				return FALSE;
			}
		}

		while (TRUE) 
		{
			int LogLen = _snwprintf(m_szLogFileName, MAX_PATH, 
				L"%s\\%s-%04d%02d%02d-%02d%02d%02d-%04d.log", 
				szProgramName, szLogFilePrefix, sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, dwSpinCount);

			if(LogLen <= 0)
			{
				LOG_ERROR((L"상세 로그를 위한 파일 이름을 생성할 수 없습니다."));
				return FALSE;
			}

			if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(m_szLogFileName))
			{
				break;
			}

			++dwSpinCount;
		}

		return TRUE;
	}

	NFLogManager::NFLogManager(void) : m_lpDetailBuffer(0)
	{
	}

	NFLogManager::~NFLogManager(void)
	{
		Destroy();
	}

	BOOL NFLogManager::Initialize(BOOL bCompress, const WCHAR* szLogFilePrefix)
	{
		WCHAR strName[MAX_PATH];
		Nave::GetProgramName(strName, MAX_PATH);
		wcsncpy(m_szLogFilePrefix, strName, MAX_PATH);

		if(szLogFilePrefix != 0)
		{
			wcscat(m_szLogFilePrefix, szLogFilePrefix);
		}

		// 처음에 프리버퍼 개수만큼 만든다.
		for(int nCount = 0; nCount < DEFAULT_FREE_LOG_BUFFER_NUM; ++nCount)
		{
			NFLogBuffer* pDetailBuffer = new NFLogBuffer;
			if(0 == pDetailBuffer)
			{
				LOG_ERROR((L"상세 로그 버퍼를 할당하는 데 실패했습니다"));
				Destroy();
				return FALSE;
			}

			m_FreeList.push_back(pDetailBuffer);
		};

		// 버퍼가 없으니 우선 1개 가져옴.
		if(m_lpDetailBuffer == 0)
		{
			m_lpDetailBuffer = GetBuffer();
		}

		m_LogSaveThread.Initialize(this, bCompress);

		if(INVALID_HANDLE_VALUE == NFThreadManager::Run(&m_LogSaveThread))
		{
			LOG_ERROR((L"Flush스레드를 생성하는 데 실패했습니다"));
			return FALSE;
		}

		return TRUE;
	}

	BOOL NFLogManager::Destroy()
	{
		Flush();
		NFThreadManager::Stop(&m_LogSaveThread, INFINITE);

		// 싱크건다.
		NFSyncLock CL(&m_LogSync);

		NFLogBuffer* pDetailBuffer;
		for(NFLogBuffer::List::iterator itr = m_FreeList.begin(); itr != m_FreeList.end(); ++itr)
		{
			pDetailBuffer = (*itr);
			_DELETE(pDetailBuffer);
		}
		m_FreeList.clear();

		for(NFLogBuffer::List::iterator itr = m_WriteList.begin(); itr != m_WriteList.end(); ++itr)
		{
			pDetailBuffer = (*itr);
			_DELETE(pDetailBuffer);
		}
		m_WriteList.clear();

		return TRUE;
	}

	VOID NFLogManager::SetLogMaxSize(DWORD dwSize)
	{
		m_LogSaveThread.SetLogMaxSize(dwSize);
	}

	NFLogBuffer* NFLogManager::GetBuffer()
	{
		// 싱크건다.
		NFSyncLock CL(&m_LogSync);

		NFLogBuffer*	pLogBuffer = 0;
		if(m_FreeList.empty())
		{
			// 남는 버퍼가 없으면 실시간으로 생성해버린다. 어쩔수없다.
			pLogBuffer = new NFLogBuffer;
		}
		else
		{
			pLogBuffer = m_FreeList.front();
			m_FreeList.pop_front();
		}

		if(0 == pLogBuffer)
		{
			LOG_ERROR((L"상세 로그 버퍼를 할당할 수 없습니다."));
			return 0;
		}

		pLogBuffer->Initialize();
		return pLogBuffer;
	};

	BOOL NFLogManager::Flush()
	{
		// 버퍼를 넣는다.
		PushBuffer(&m_lpDetailBuffer);
		return m_LogSaveThread.FlushSignal();
	}

	char* NFLogManager::ReserveBuffer(unsigned short usReserve)
	{
		NFSyncLock CL(&m_LogSync);

		if(0 == m_lpDetailBuffer)
		{
			m_lpDetailBuffer = GetBuffer();
		}
		
		if(NFLogBuffer::MAX_LOG_BUFFER < m_lpDetailBuffer->m_dwUsage + usReserve)
		{
			Flush();	// 버퍼가 꽉찼으니 저장해라.
			m_lpDetailBuffer = GetBuffer();		
		}

		if(0 == m_lpDetailBuffer)
		{
			LOG_ERROR((L"로그 버퍼가 0입니다."));
			return 0;
		}

		// 컴플리트를 먼저해서 해당영역을 확보한다.
		char* pPoint = &m_lpDetailBuffer->m_Buffer[m_lpDetailBuffer->m_dwUsage];
		Complete(usReserve);
		return pPoint;
	}

	VOID NFLogManager::SpliceInWriteBuffer(NFLogBuffer::List& logBufferList)
	{
		NFSyncLock CL(&m_LogSync);

		if(m_WriteList.empty())
			return;

		// WriteBuffer 를 가져온다.
		logBufferList.splice(logBufferList.end(), m_WriteList);
	}

	VOID NFLogManager::SpliceInFreeBuffer(NFLogBuffer::List& logBufferList)
	{
		NFSyncLock CL(&m_LogSync);
		
		if(logBufferList.empty())
			return;

		m_FreeList.splice(m_FreeList.end(), logBufferList);
	}

	VOID NFLogManager::PushBuffer(NFLogBuffer** ppDetailBuffer)
	{
		if(0 == *ppDetailBuffer) { return; }

		NFSyncLock CL(&m_LogSync);

		if(0 == (*ppDetailBuffer)->m_dwUsage)
		{
			// 사이즈가 0이니 다시 Free에 넣음.
			m_FreeList.push_back(*ppDetailBuffer);
		}
		else
		{
			m_WriteList.push_back(*ppDetailBuffer);
		}

		// 이부분 대문에 이중포인터로 변수를 받은것.
		*ppDetailBuffer = 0;
	}

}