/** 
 *  @file		NFLog.h
 *  @brief		Log ��� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string>

#include "NFLogManager.h"

namespace Nave { 
		
	/** 
	 *  @class        NFLog
	 *  @brief        Log ��� Ŭ����
	 *  @remarks      NFLog::SetLogHandel(hList);	// Ư�� ������� �α׸� ����Ѵ�.		\r\n
	 *                NFLog::EnableLogPrint(TRUE);	// �α׸� ȭ�鿡 ����Ѵ�.				\r\n
	 *                NFLog::EnableLogOutput(TRUE);	// �α׸� ���Ϸ� �����Ѵ�				\r\n
	 *                NFLog::SetLogDetail(TRUE);	// �� �α׷� �����մϴ�.				\r\n
	 *                NFLog::SetLogLimit(NFLog::Proxy::Info) // �α� �����͸� �����մϴ�.	\r\n
	 *                // �α� ���															\r\n
	 *                LOG_IMPORTANT((L"Test Start"));										\r\n
	 *                // �α׸� �������մϴ�.												\r\n
	 *                NFLog::CloseLog();													\r\n
	 *  @warning	  NFLog::CloseLog()�� ȣ������ ������� ���Ϸ� �αװ� ������� 
	 *				  �ʽ��ϴ�.
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFLog
	{
	public:
		/// �� �α��� Index Ÿ��
		enum Groups {
			Info = 1 << 0,			
			Warning = 1 << 1,		
			Error = 1 << 2,
			Exception = 1 << 3,
			Important = 1 << 4,
		};

		/// �α� ��¿� ����ü
		struct Proxy 
		{
			/// �αװ� ȣ��� �Լ�
			const WCHAR* func;
			/// �αװ� ȣ��� ����
			const WCHAR* file;
			/// �αװ� ȣ��� ���ι�ȣ
			int			linenum;
			/// �α��� �׷���̵�
			int			group;

			Proxy(int pGroup, const WCHAR* pFile, int pLinenum, const WCHAR* pFunc);
			void Log(const WCHAR* msg, ...);
		};

		/// Message�� ����� ������ �ڵ��� �����Ѵ�.
		static void SetLogHandel(HWND hListWnd);
		/// ȭ�鿡 �ΰ� ��������
		static void SetLogPrint(BOOL enable = TRUE);
		/// ���Ϸ� �α׸� ��������
		static void SetLogOutput(BOOL enable = TRUE, DWORD dwSize = 100 * 1024 * 1024);
		/// �α��� ��� �����͸� �����մϴ�. NFLog::Info ~ NFLog::Important
		static void SetLogLimit(int limit);
		/// �� �α׸� ����ϴ�.
		static void SetLogDetail(BOOL detail);

		/// �α׸� �����մϴ�. (�ӽú����� ����� �α׸� ���Ϸ� �����մϴ�.)
		static void CloseLog();

	private:
		/**
		 * @brief	hWnd�� String�� ����Ѵ�.
		 * @param hWnd		������ �ڵ�
		 * @param String	�޽���
		 * @param Len		�޽�������
		 */
		static void AddLogMsg( HWND hWnd, WCHAR* String, int Len );

		/**
		 * @brief		�޽����� ����Ѵ�. �ܼ��϶� printf�� ���������϶� Msg������� ���
		 * @param *msg	��� �޽���
		 * @param ...	����
		 */
		static void LogPrintf( WCHAR* msg, ... );

		/**
		 * @brief		�޽����� ����Ѵ�. �ܼ��϶� printf�� ���������϶� Msg������� ���
		 * @param group	�׷��ε���
		 * @param *msg	�޽���
		 */
		static void LogPrintf( int group, WCHAR* msg );

		/**
		 * @brief		�޽����� ����Ѵ�. �ܼ��϶� printf�� ���������϶� Msg������� ���
		 * @param group		�׷��ε���
		 * @param pFile		ȣ�����ϸ�
		 * @param pLinenum	ȣ�����
		 * @param pFunc		ȣ���Լ�
		 * @param *msg		�޽���
		 */
		static void LogPrintf( int group, const WCHAR* pFile, int pLinenum, const WCHAR* pFunc, WCHAR* msg );

		/// �α� ���ڿ��� �߰��մϴ�. 
		static void OutputLog(const WCHAR* log);

	private:
		/// Message�� ����� ������ �ڵ�
		static HWND		s_hLogHandel;
		/// �α������ Ȱ��ȭ �մϴ�.
		static BOOL		s_bEnableLogPrint;
		/// �α׸� ���Ϸ� �����մϴ�.
		static BOOL		s_bSaveLogFile;
		/// �α� Limit
		static int		s_iLogLimit;
		/// �α� Detail 
		static BOOL		s_bLogDetail;

		/// �α׸� ������ �����ϴ� �Ŵ���
		static NFLogManager*	s_pLogManager;
	};
}

/// Info Log
#define LOG_INFO(LOGMESSAGE) {Nave::NFLog::Proxy(Nave::NFLog::Info, __WFILE__, __LINE__,__WFUNCSIG__).Log LOGMESSAGE;} 
/// Warning Log
#define LOG_WARNING(LOGMESSAGE) {Nave::NFLog::Proxy(Nave::NFLog::Warning,__WFILE__, __LINE__,__WFUNCSIG__).Log LOGMESSAGE;} 
/// Error Log
#define LOG_ERROR(LOGMESSAGE) {Nave::NFLog::Proxy(Nave::NFLog::Error,__WFILE__, __LINE__,__WFUNCSIG__).Log LOGMESSAGE;} 
/// Exeption Log
#define LOG_EXCEPTION(LOGMESSAGE) {Nave::NFLog::Proxy(Nave::NFLog::Exception,__WFILE__, __LINE__,__WFUNCSIG__).Log LOGMESSAGE;}
/// Important Log
#define LOG_IMPORTANT(LOGMESSAGE) {Nave::NFLog::Proxy(Nave::NFLog::Important,__WFILE__, __LINE__,__WFUNCSIG__).Log LOGMESSAGE;} 
