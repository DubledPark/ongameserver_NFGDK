/** 
 *  @file		NFCommand.h
 *  @brief		���ø����̼ǿ��� Commandó���� �ϱ� ���� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include "NFStringUtil.h"

namespace Nave {

	/**
	 * @brief		���ο� Ŀ�ǵ带 ����մϴ�.
	 * @param cmd		Command Type
	 * @param command	Command ��ü Ŭ����
	 * @param msg		Command ��� �޽���
	 * @return			��������
	 * @warning		NFCommandFactory�� ���ӵ� Define ��ɾ��Դϴ�.
	 */
	#define ADD_COMMAND(cmd, command, msg) AddCommand(cmd, new command, msg)

	/** 
	 *  @class        NFCommand
	 *  @brief        ���ø����̼ǿ��� Ŀ�ǵ带 �ۼ��Ҷ� ����� Ŀ��� ��ü
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	class NFCommand
	{
	public:
		/// NFCommand ������
		NFCommand()
		{
		}

		/// NFCommand �Ҹ���
		virtual ~NFCommand()
		{
		}

	public:

		/**
		 * @brief		Ŀ�ǵ带 ó���մϴ�.
		 * @param param		�Ķ����
		 * @return		�������
		 */
		virtual BOOL DoProcess(WCHAR* param) = 0;    
	};

	/** 
	 *  @class        NFCommandFactory
	 *  @brief		  Ŀ�ǵ带 ����Ҽ� �ֵ��� �����մϴ�. (����� �޾Ƽ����)
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	class NFCommandFactory
	{
	public:
		/// NFCommandFactory ������
		NFCommandFactory()
		{
		}

		/// NFCommandFactory �Ҹ���
		~NFCommandFactory()
		{
			ReleaseCommand();
		}

	public:

		/**
	     * @brief		���ο� Ŀ�ǵ带 ����մϴ�.
	     * @param commandName		Ŀ�ǵ� �̸�
	     * @param command			Ŀ�ǵ� Ŭ����
	     * @param message			Ŀ�ǵ� �޽���
	     * @return		��������
	     */
	    BOOL AddCommand(const WCHAR* commandName, NFCommand* command, const WCHAR* message)
		{
			if(0 == commandName, 0 == command)
				return FALSE;    

			m_CMDVector.push_back(COMMAND(commandName, command, message));
			return TRUE;
		}

		/// ��� Ŀ�ǵ带 �����մϴ�.
		VOID ReleaseCommand()
		{
			int iSize = m_CMDVector.size();
			for(int i = 0; i < iSize; ++i)
			{
				delete m_CMDVector[i].pCommand;
			}

			m_CMDVector.clear();
		}

		/// Ŀ�ǵ带 �ʱ�ȭ�մϴ�.
		virtual VOID	InitializeCommand() {};

		/// Ŀ�ǵ带 �Է·����� �����մϴ�.
		virtual VOID	StartCommand() {}; 
		/// Ŀ�ǵ� �Է·����� �����մϴ�.
		virtual VOID	EndCommand() {};

		/// ȭ�鿡 Ŀ�ǵ帮��Ʈ�� ����մϴ�.
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
		 * @brief	Ŀ�ǵ带 ó���մϴ�
		 * @param command command	���ڿ�
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
		/// Ŀ�ǵ� ����ü
		struct COMMAND
		{
			Nave::uint32	uiHash;			/// Ŀ�ǵ��� �ؽ��ڵ�
			WCHAR			szCommand[32];	/// Ŀ�ǵ� �̸�
			WCHAR			szMessage[128];	/// Ŀ�ǵ� �޽���
			NFCommand*		pCommand;		/// Ŀ�ǵ尡 ����ϴ� Ŀ�ǵ� Ŭ����
	        
			/**
			 * @brief	Ŀ�ǵ� ��ü 
			 * @param commandName Ŀ�ǵ� �̸�
			 * @param command	  Ŀ�ǵ� Ŭ����
			 * @param message	  ��� �޽���
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

		/// Ŀ�ǵ� ���� ������
		typedef std::vector<COMMAND>	CMDVector;

		/// Ŀ�ǵ帮��Ʈ 
		CMDVector						m_CMDVector;
	};

}