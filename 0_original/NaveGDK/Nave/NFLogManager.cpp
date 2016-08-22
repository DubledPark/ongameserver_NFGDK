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
		// ���⼭ m_bEnd�� TRUE�� �ƴϸ� �����尡 �۵��ϰ� �ִ�.
		for(;TRUE != InterlockedCompareExchange(&m_bEnd, TRUE, TRUE);)
		{
			// ���� �̺�Ʈ�� ������̸� 1�ʰ� ����� �ٽ� Ȯ��.
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
		// �α׿��� ���۸� �о�´�.
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
				LOG_ERROR((L"�α� ���� ��Ͽ� �����߽��ϴ�. ErrorNum : %d, FileHandle:0x%p, ���� ũ��:%d",
					GetLastError(), m_hFile, pLogBuffer->m_dwUsage));
			}

			m_dwTotalWritten += dwWritten;
		}

		m_pLogHandle->SpliceInFreeBuffer(m_WriteBufferList);

		// ������ ��� ������´�.
		if(m_dwTotalWritten > m_dwMaxFileSize)
		{
			if(INVALID_HANDLE_VALUE != m_hFile) { CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE; }	// ���� �ݰ�...

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
			LOG_ERROR((L"���� �α׸� ������ �� �����ϴ�."));
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

		// ���� �˰��� �߰�.
		SDK::CGZip gzip;

		if (!gzip.Open(szCompressedLogFileName, SDK::CGZip::ArchiveModeWrite))
		{
			LOG_ERROR((L"���� ������ ������ �����߽��ϴ�."));
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
				LOG_ERROR((L"������ ���� ���� ���Ͽ� ����� �� �����ϴ�."));
				break;
			}
		}

		gzip.Close();

		CloseHandle(hFile);

		int nError = GetLastError();
		if(0 != nError && ERROR_HANDLE_EOF != nError)
		{
			LOG_ERROR((L"%d ������ ���� �α� ���� ������ �����߽��ϴ�.", nError));
			return FALSE;
		}
		
		// �α����� ����
		if(FALSE == DeleteFileW(m_szLogFileName))
		{
			LOG_ERROR((L"%d ������ ���� �����ϰ� �� �α׸� �������� ���߽��ϴ�.", nError));
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

		// ���α׷��̸��� ������ ���������� �����Ѵ�.
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
				LOG_ERROR((L"�� �α׸� ���� ���� �̸��� ������ �� �����ϴ�."));
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

		// ó���� �������� ������ŭ �����.
		for(int nCount = 0; nCount < DEFAULT_FREE_LOG_BUFFER_NUM; ++nCount)
		{
			NFLogBuffer* pDetailBuffer = new NFLogBuffer;
			if(0 == pDetailBuffer)
			{
				LOG_ERROR((L"�� �α� ���۸� �Ҵ��ϴ� �� �����߽��ϴ�"));
				Destroy();
				return FALSE;
			}

			m_FreeList.push_back(pDetailBuffer);
		};

		// ���۰� ������ �켱 1�� ������.
		if(m_lpDetailBuffer == 0)
		{
			m_lpDetailBuffer = GetBuffer();
		}

		m_LogSaveThread.Initialize(this, bCompress);

		if(INVALID_HANDLE_VALUE == NFThreadManager::Run(&m_LogSaveThread))
		{
			LOG_ERROR((L"Flush�����带 �����ϴ� �� �����߽��ϴ�"));
			return FALSE;
		}

		return TRUE;
	}

	BOOL NFLogManager::Destroy()
	{
		Flush();
		NFThreadManager::Stop(&m_LogSaveThread, INFINITE);

		// ��ũ�Ǵ�.
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
		// ��ũ�Ǵ�.
		NFSyncLock CL(&m_LogSync);

		NFLogBuffer*	pLogBuffer = 0;
		if(m_FreeList.empty())
		{
			// ���� ���۰� ������ �ǽð����� �����ع�����. ��¿������.
			pLogBuffer = new NFLogBuffer;
		}
		else
		{
			pLogBuffer = m_FreeList.front();
			m_FreeList.pop_front();
		}

		if(0 == pLogBuffer)
		{
			LOG_ERROR((L"�� �α� ���۸� �Ҵ��� �� �����ϴ�."));
			return 0;
		}

		pLogBuffer->Initialize();
		return pLogBuffer;
	};

	BOOL NFLogManager::Flush()
	{
		// ���۸� �ִ´�.
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
			Flush();	// ���۰� ��á���� �����ض�.
			m_lpDetailBuffer = GetBuffer();		
		}

		if(0 == m_lpDetailBuffer)
		{
			LOG_ERROR((L"�α� ���۰� 0�Դϴ�."));
			return 0;
		}

		// ���ø�Ʈ�� �����ؼ� �ش翵���� Ȯ���Ѵ�.
		char* pPoint = &m_lpDetailBuffer->m_Buffer[m_lpDetailBuffer->m_dwUsage];
		Complete(usReserve);
		return pPoint;
	}

	VOID NFLogManager::SpliceInWriteBuffer(NFLogBuffer::List& logBufferList)
	{
		NFSyncLock CL(&m_LogSync);

		if(m_WriteList.empty())
			return;

		// WriteBuffer �� �����´�.
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
			// ����� 0�̴� �ٽ� Free�� ����.
			m_FreeList.push_back(*ppDetailBuffer);
		}
		else
		{
			m_WriteList.push_back(*ppDetailBuffer);
		}

		// �̺κ� �빮�� ���������ͷ� ������ ������.
		*ppDetailBuffer = 0;
	}

}