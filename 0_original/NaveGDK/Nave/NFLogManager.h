/** 
 *  @file		NFLogManager.h
 *  @brief		LogManager Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <time.h>
#include <list>

#include "NFSync.h"
#include "NFThread.h"

namespace Nave { 

	class NFLogManager;

	/** 
	 *  @class        NFLogBuffer
	 *  @brief        �αװ� ����Ǵ� �α� ���� �̰� ���Ϸ� ����ȴ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFLogBuffer
	{
	public:
		/// Write, Free ���۸� ����Ʈ�� �����ϰ� �Ǵµ� typedef�� ���⼭ �ߴ�.
		typedef std::list<NFLogBuffer*>		List;	

		enum 
		{ 
			MAX_LOG_BUFFER = 65536	/// �α׹����� ũ��
		};

		/// ���� ������ ������
		unsigned long		m_dwUsage;						
		/// ���� ����
		char				m_Buffer[MAX_LOG_BUFFER];		

		/// NFLogBuffer ������
		NFLogBuffer() : m_dwUsage(0) 
		{ 
		}

		/// �ʱ�ȭ �Լ�
		VOID Initialize() 
		{ 
			m_dwUsage = 0; 
		}
	};

	/** 
	 *  @class        NFLogThread
	 *  @brief        �α׸� �����ϴ� �α� �������̴�. �α׸Ŵ������� ����.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFLogThread : public NFThread
	{
	public:
		/// NFLogThread ������
		NFLogThread();
		/// NFLogThread �Ҹ���
		~NFLogThread();
		
		/**
		 * @brief		�α׸� �ʱ�ȭ�Ѵ�.
		 * @param pLog		�α׸޴��� ��ü
		 * @param bCompress ���࿩��
		 */
		VOID		Initialize(NFLogManager* pLog, BOOL bCompress);

		/**
		 * @brief	�α������� ũ�⸦ �����Ѵ�. ��ũ�Ⱑ �Ǹ� �ٸ� ���Ϸ� �α׸������.
		 * @param dwSize �α������� ũ�� byte ���� ����� 10 * 1024 * 1024 �� 10�ް�
		 */
		inline VOID SetLogMaxSize(DWORD dwSize = MAX_FILE_SIZE)  { m_dwMaxFileSize = dwSize; }
		
		/// �α׸� �����Ѵ�.
		BOOL		FlushSignal() { return PulseEvent(m_hFlush); }

		/// �α� �����带 �����Ѵ�.
		virtual unsigned int Run();
		/// �α� �����带 �����մϴ�.
		virtual BOOL End();			

	private:
		enum 
		{ 
			MAX_FILE_SIZE = 100 * 1024 * 1024		/// �⺻ ���� ������ 100�ް�
		};

		/**
		* @brief	�α������� �̸��� �����մϴ�. 
		* @return  ��������
		*/
		BOOL				SetLogFileName();
		/**
		* @brief	�α׹��ۿ� �α׸� Write �մϴ�.
		* @return  ��������
		*/
		BOOL				WriteLog();
		/**
		* @brief	������ �α������� �����մϴ�.
		* @return	��������
		*/
		BOOL				Compress();

	private:
		/// �ش� �����尡 �������� ���·���
		volatile LONG		m_bEnd;					

		/// ���� �ش� �����尡 �۵������� Ȯ���ϴ� �̺�Ʈ
		HANDLE				m_hFlush;				
		/// �αװ� ����Ǵ� �����ڵ�
		HANDLE				m_hFile;
		/// ���� ����ũ��
		DWORD				m_dwTotalWritten;		

		/// �α� �޴���
		NFLogManager*		m_pLogHandle;
		/// ���� ����
		BOOL				m_bCompress;

		/// ���� �α� ����
		NFLogBuffer::List	m_WriteBufferList;

		/// �α��� �����̸�
		WCHAR				m_szLogFileName[MAX_PATH];

		/// �α������� �ִ� ũ��
		DWORD				m_dwMaxFileSize;
	};

	/** 
	 *  @class        NFLogManager
	 *  @brief        �α׸� �����Ҷ� ����ϴ� �α� �Ŵ��� Ŭ����
	 *  @remarks      �α� �Ŵ����� �� ������ �������� �α׸� �������� ����� ���� �ֱ� ������ �̱������� ������ �ʴ´�.\r\n
	 *  			  �����Ϸα״� ���̳ʸ����� �ֱ����� �����ϴ� �α״�.												\r\n
	 *  		 	  �ȿ� ���� �α״� ���� ���� �ش� �α� ����ü���� �����ؾ��Ѵ�.									\r\n
	 *  			  ������ �α��� ��� �ӵ������� ���� ó���� �޸𸮻� ���۸� ���� ���۰� �� á���� �ش� ���۸�		\r\n
	 *  			  WriteList�� ������ �ش� ���۸� ó���Ѵ�.															\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFLogManager 
	{
	public:
		/// NFLogManager ������
		NFLogManager(void);
		/// NFLogManager �Ҹ���
		~NFLogManager(void);

		/**
		 * @brief	�α� �޴����� �ʱ�ȭ �մϴ�.
		 * @param bCompress			��������
		 * @param szLogFilePrefix	�α����ϸ�
		 * @return	��������
		 */
		BOOL			Initialize(BOOL bCompress, const WCHAR* szLogFilePrefix = 0);
		/// �α׸Ŵ����� �����մϴ�.
		BOOL			Destroy();

		/// �α׹����� ����� �����Ѵ�.
		VOID			SetLogMaxSize(DWORD dwSize = 10 * 1024 * 1024);


		/**
		 * @brief	�α������� �̸�
		 * @return	�α������̸�
		 */
		const WCHAR*	GetLogFilePrefix() const { return m_szLogFilePrefix; }
	
		/**
		 * @brief	�α׸� �����մϴ�.
		 * @return	��������
		 */
		BOOL			Flush();

		/**
		 * @brief	���� ���Ǵ� �α� ���۸� ���մϴ�.
		 * @return	�α׹���
		 */
		NFLogBuffer*	GetBuffer();

		/**
		 * @brief	 �α׹��۸� �Ҵ��Ѵ�.
		 * @param usReserve �Ҵ� ������
		 * @return	�α׹����� ��ġ������
		 */
		char*			ReserveBuffer(unsigned short usReserve);

		/**
	     * @brief	�Ҵ�� �α׹��۸� �����ŵ�ϴ�.
	     * @param usRealUse ������
	     */
	    VOID		    Complete(unsigned short usRealUse) { m_lpDetailBuffer->m_dwUsage += usRealUse; }

		/**
		 * @brief	��»��·� �α׹��۸� �Ҵ��մϴ�.
		 * @param ppDetailBuffer 
		 */
		VOID			PushBuffer(NFLogBuffer** ppDetailBuffer);

		VOID			SpliceInWriteBuffer(NFLogBuffer::List& logBufferList);
		VOID			SpliceInFreeBuffer(NFLogBuffer::List& logBufferList);

	private:
		enum 
		{ 
			DEFAULT_FREE_LOG_BUFFER_NUM = 10		/// �ִ� 10������ �������۸� �����Ѵ�.
		};

		/// Sync ��ü
		NFSync							m_LogSync;

		/// �α� ���ϸ�
		WCHAR							m_szLogFilePrefix[MAX_PATH];

		/// �α� ����� ������ ����
		NFLogThread						m_LogSaveThread;

		/// ����ִ� ����
		NFLogBuffer::List				m_FreeList;		
		/// ���Ͽ� ������ ����
		NFLogBuffer::List				m_WriteList;	

		/// ���� ���Ǵ� ����
	    NFLogBuffer*					m_lpDetailBuffer;
	};

}