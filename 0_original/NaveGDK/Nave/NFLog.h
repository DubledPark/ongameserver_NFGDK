/** 
 *  @file		NFLog.h
 *  @brief		Log 출력 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
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
	 *  @brief        Log 출력 클래스
	 *  @remarks      NFLog::SetLogHandel(hList);	// 특정 윈도우로 로그를 출력한다.		\r\n
	 *                NFLog::EnableLogPrint(TRUE);	// 로그를 화면에 출력한다.				\r\n
	 *                NFLog::EnableLogOutput(TRUE);	// 로그를 파일로 저장한다				\r\n
	 *                NFLog::SetLogDetail(TRUE);	// 상세 로그로 설정합니다.				\r\n
	 *                NFLog::SetLogLimit(NFLog::Proxy::Info) // 로그 리미터를 정의합니다.	\r\n
	 *                // 로그 사용															\r\n
	 *                LOG_IMPORTANT((L"Test Start"));										\r\n
	 *                // 로그를 마무리합니다.												\r\n
	 *                NFLog::CloseLog();													\r\n
	 *  @warning	  NFLog::CloseLog()를 호출하지 않을경우 파일로 로그가 저장되지 
	 *				  않습니다.
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFLog
	{
	public:
		/// 각 로그의 Index 타입
		enum Groups {
			Info = 1 << 0,			
			Warning = 1 << 1,		
			Error = 1 << 2,
			Exception = 1 << 3,
			Important = 1 << 4,
		};

		/// 로그 출력용 구조체
		struct Proxy 
		{
			/// 로그가 호출된 함수
			const WCHAR* func;
			/// 로그가 호출된 파일
			const WCHAR* file;
			/// 로그가 호출된 라인번호
			int			linenum;
			/// 로그의 그룹아이디
			int			group;

			Proxy(int pGroup, const WCHAR* pFile, int pLinenum, const WCHAR* pFunc);
			void Log(const WCHAR* msg, ...);
		};

		/// Message를 출력할 윈도우 핸들을 셋팅한다.
		static void SetLogHandel(HWND hListWnd);
		/// 화면에 로고를 찍을꺼냐
		static void SetLogPrint(BOOL enable = TRUE);
		/// 파일로 로그를 찍을꺼냐
		static void SetLogOutput(BOOL enable = TRUE, DWORD dwSize = 100 * 1024 * 1024);
		/// 로그의 사용 리미터를 정의합니다. NFLog::Info ~ NFLog::Important
		static void SetLogLimit(int limit);
		/// 상세 로그를 남깁니다.
		static void SetLogDetail(BOOL detail);

		/// 로그를 종료합니다. (임시변수에 저장된 로그를 파일로 저장합니다.)
		static void CloseLog();

	private:
		/**
		 * @brief	hWnd에 String을 출력한다.
		 * @param hWnd		윈도우 핸들
		 * @param String	메시지
		 * @param Len		메시지길이
		 */
		static void AddLogMsg( HWND hWnd, WCHAR* String, int Len );

		/**
		 * @brief		메시지를 출력한다. 콘솔일때 printf로 윈도우모드일때 Msg윈도우로 출력
		 * @param *msg	출력 메시지
		 * @param ...	인자
		 */
		static void LogPrintf( WCHAR* msg, ... );

		/**
		 * @brief		메시지를 출력한다. 콘솔일때 printf로 윈도우모드일때 Msg윈도우로 출력
		 * @param group	그룹인덱스
		 * @param *msg	메시지
		 */
		static void LogPrintf( int group, WCHAR* msg );

		/**
		 * @brief		메시지를 출력한다. 콘솔일때 printf로 윈도우모드일때 Msg윈도우로 출력
		 * @param group		그룹인덱스
		 * @param pFile		호출파일명
		 * @param pLinenum	호출라인
		 * @param pFunc		호출함수
		 * @param *msg		메시지
		 */
		static void LogPrintf( int group, const WCHAR* pFile, int pLinenum, const WCHAR* pFunc, WCHAR* msg );

		/// 로그 문자열을 추가합니다. 
		static void OutputLog(const WCHAR* log);

	private:
		/// Message를 출력할 윈도우 핸들
		static HWND		s_hLogHandel;
		/// 로그출력을 활성화 합니다.
		static BOOL		s_bEnableLogPrint;
		/// 로그를 파일로 저장합니다.
		static BOOL		s_bSaveLogFile;
		/// 로그 Limit
		static int		s_iLogLimit;
		/// 로그 Detail 
		static BOOL		s_bLogDetail;

		/// 로그를 파일을 관리하는 매니져
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
