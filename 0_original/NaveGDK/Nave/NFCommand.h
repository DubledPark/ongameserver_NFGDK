/** 
 *  @file		NFCommand.h
 *  @brief		어플리케이션에서 Command처리를 하기 위한 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include "NFStringUtil.h"

namespace Nave {

	/**
	 * @brief		새로운 커맨드를 등록합니다.
	 * @param cmd		Command Type
	 * @param command	Command 객체 클래스
	 * @param msg		Command 출력 메시지
	 * @return			성공여부
	 * @warning		NFCommandFactory에 종속된 Define 명령어입니다.
	 */
	#define ADD_COMMAND(cmd, command, msg) AddCommand(cmd, new command, msg)

	/** 
	 *  @class        NFCommand
	 *  @brief        어플리케이션에서 커맨드를 작성할때 사용할 커멘드 객체
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	class NFCommand
	{
	public:
		/// NFCommand 생성자
		NFCommand()
		{
		}

		/// NFCommand 소멸자
		virtual ~NFCommand()
		{
		}

	public:

		/**
		 * @brief		커맨드를 처리합니다.
		 * @param param		파라메터
		 * @return		상공여부
		 */
		virtual BOOL DoProcess(WCHAR* param) = 0;    
	};

	/** 
	 *  @class        NFCommandFactory
	 *  @brief		  커맨드를 사용할수 있도록 설정합니다. (상속을 받아서사용)
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	class NFCommandFactory
	{
	public:
		/// NFCommandFactory 생성자
		NFCommandFactory()
		{
		}

		/// NFCommandFactory 소멸자
		~NFCommandFactory()
		{
			ReleaseCommand();
		}

	public:

		/**
	     * @brief		새로운 커맨드를 등록합니다.
	     * @param commandName		커맨드 이름
	     * @param command			커맨드 클래스
	     * @param message			커맨드 메시지
	     * @return		성공여부
	     */
	    BOOL AddCommand(const WCHAR* commandName, NFCommand* command, const WCHAR* message)
		{
			if(0 == commandName, 0 == command)
				return FALSE;    

			m_CMDVector.push_back(COMMAND(commandName, command, message));
			return TRUE;
		}

		/// 모든 커맨드를 삭제합니다.
		VOID ReleaseCommand()
		{
			int iSize = m_CMDVector.size();
			for(int i = 0; i < iSize; ++i)
			{
				delete m_CMDVector[i].pCommand;
			}

			m_CMDVector.clear();
		}

		/// 커맨드를 초기화합니다.
		virtual VOID	InitializeCommand() {};

		/// 커맨드를 입력루프를 시작합니다.
		virtual VOID	StartCommand() {}; 
		/// 커맨드 입력루프를 종료합니다.
		virtual VOID	EndCommand() {};

		/// 화면에 커맨드리스트를 출력합니다.
		VOID ShowCommand()
		{
			LOG_IMPORTANT((L"------------------- Commands -------------------"));
			int iSize = m_CMDVector.size();
			for(int i = 0; i < iSize; ++i)
			{
				NAVE_IMPORTANT((_T("%s : %s"), m_CMDVector[i].szCommand, m_CMDVector[i].szMessage));
			}
			LOG_IMPORTANT((L"----------------- Commands End -----------------"));
		}

		/**
		 * @brief	커맨드를 처리합니다
		 * @param command command	문자열
		 */
		VOID DoCommand(WCHAR* command)
		{
			WCHAR Buff[DEF_BUFSIZE];
			ZeroMemory(Buff, DEF_BUFSIZE);
			wcscpy(Buff, Command);

			WCHAR* cmd;
			cmd = wcstok(command, L" ");
			if(cmd)
			{
				Nave::uint32 uiHash = Nave::Hash(cmd);
				WCHAR* NextCmd = &Buff[wcslen(cmd)+1];

				int iSize = m_CMDVector.size();
				for(int i = 0; i < iSize; ++i)
				{
					if(m_CMDVector[i].uiHash != uiHash)
						continue;

					m_CMDVector[i].pCommand->DoProcess(NextCmd);
					return;
				}
			}
		}

	private:
		/// 커맨드 구조체
		struct COMMAND
		{
			Nave::uint32	uiHash;			/// 커맨드의 해쉬코드
			WCHAR			szCommand[32];	/// 커맨드 이름
			WCHAR			szMessage[128];	/// 커맨드 메시지
			NFCommand*		pCommand;		/// 커맨드가 사용하는 커맨드 클래스
	        
			/**
			 * @brief	커맨드 객체 
			 * @param commandName 커맨드 이름
			 * @param command	  커맨드 클래스
			 * @param message	  출력 메시지
			 */
			COMMAND(const WCHAR* commandName, NFCommand* command, const WCHAR* message)
			{
				uiHash = Nave::Hash(commandName);

				wcscpy(szCommand, commandName);

				if(szMessage)
					wcscpy(szMessage, message);
				else
					wcscpy(szMessage, L"No Help Message");

				pCommand = command;
			}
		};

		/// 커맨드 백터 디파인
		typedef std::vector<COMMAND>	CMDVector;

		/// 커맨드리스트 
		CMDVector						m_CMDVector;
	};

}