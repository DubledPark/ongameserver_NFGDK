/** 
 *  @file  		UIFramework.h
 *  @brief 		Server�� ȭ���������̽��� �����ϴ� UIFramework ��ü
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

#include "UICmdEdit.h"
#include "UICmdMsgView.h"
#include <vector>

namespace NaveServer {

	/// ���ο� Ŀ�ǵ带 �߰��մϴ�.
	/// ADD_COMMAND(_T("help"), CCmdHelp, L"ȭ�鿡 Ŀ��帮��Ʈ�� �����ݴϴ�.");
	#define ADD_COMMAND(cmd, object, msg) AddCommand(cmd, new object, msg)

	/** 
	 *  @class        UICommand
	 *  @brief        ���ø����̼ǿ��� Ŀ�ǵ带 �ۼ��Ҷ� ����� Ŀ��� ��ü
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-05-09
	 */
	class UICommand
	{
	public:
		virtual BOOL DoProcess(WCHAR* lpParam) = 0;    
	};

	/** 
	 *  @class        UICommandFactory
	 *  @brief        Ŀ�ǵ带 �����ϴ� Factory ��ü Server�� Server�� UI��ü�� ��ӹ޴´�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-05-09
	 */
	class UICommandFactory
	{
	public:
	    BOOL AddCommand(const WCHAR* szCommand, UICommand* lpCommand, const WCHAR* szMessage)
		{
			if(0 == szCommand, 0 == lpCommand)
				return FALSE;    

			m_CMDVector.push_back(COMMAND(szCommand, lpCommand, szMessage));
			return TRUE;
		}

		VOID ReleaseCommand()
		{
			int iSize = m_CMDVector.size();
			for(int i = 0; i < iSize; ++i)
			{
				delete m_CMDVector[i].m_lpCMD;
			}

			m_CMDVector.clear();
		}

		virtual VOID	InitializeCommand() {};

		virtual VOID	StartCommand() {}; 							// UIConsol���� Command �Է�
		virtual VOID	EndCommand() {};

		VOID ShowCommand()
		{
			LOG_IMPORTANT( (L"------------------- Commands -------------------") );
			int iSize = m_CMDVector.size();
			for(int i = 0; i < iSize; ++i)
			{
				LOG_IMPORTANT((L"%s : %s", m_CMDVector[i].m_szCMD, m_CMDVector[i].m_szMSG));
			}
			LOG_IMPORTANT( (L"----------------- Commands End -----------------") );
		}

		VOID DoCommand(WCHAR* Command)
		{
			WCHAR Buff[DEF_BUFSIZE];
			ZeroMemory(Buff, DEF_BUFSIZE);
			wcscpy(Buff, Command);

			WCHAR* cmd;
			cmd = wcstok(Command, L" ");
			if(cmd)
			{
				Nave::uint32 uiHash = Nave::Hash(cmd);
				WCHAR* NextCmd = &Buff[wcslen(cmd)+1];

				int iSize = m_CMDVector.size();
				for(int i = 0; i < iSize; ++i)
				{
					if(m_CMDVector[i].m_uiHash != uiHash)
						continue;

					m_CMDVector[i].m_lpCMD->DoProcess(NextCmd);
					return;
				}
			}
		}

	public:
		UICommandFactory()
		{
		}
		~UICommandFactory()
		{
			ReleaseCommand();
		}

	private:
		struct COMMAND
		{
			Nave::uint32	m_uiHash;
			WCHAR			m_szCMD[32];
			WCHAR			m_szMSG[128];
			UICommand*		m_lpCMD;

			COMMAND(const WCHAR* szCommand, UICommand* lpCMD, const WCHAR* szMessage)
			{
				m_uiHash = Nave::Hash(szCommand);

				wcscpy(m_szCMD, szCommand);

				if(szMessage)
					wcscpy(m_szMSG, szMessage);
				else
					wcscpy(m_szMSG, L"No Help Message");

				m_lpCMD = lpCMD;
			}
		};

		typedef std::vector<COMMAND>	CMDVector;
		CMDVector						m_CMDVector;
	};

	/** 
	 *  @class        UIConsol
	 *  @brief		  �ܼ� UI
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-05-09
	 */
	class UIConsol : public UICommandFactory
	{
		BOOL			m_bCommandExit;
	public:
		// ����� ������Ʈ�� �ʱ�ȭ �մϴ�. (�����켳���� ����� �Ŀ� ȣ��)
		virtual VOID	InitObject() {};
		// ����� ������Ʈ�� �����մϴ�. (���η����� ������(EndProcessȣ����) ȣ��)
		virtual VOID	ReleaseObject() {};

		BOOL			Init();

		virtual VOID	InitializeCommand() {};
		virtual VOID	StartCommand();							// UIConsol���� Command �Է�
		virtual VOID	EndCommand();

	public:
		UIConsol(VOID);
		~UIConsol(VOID);
	};

	/** 
	 *  @class        UIWindow
	 *  @brief        ������ UI
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-05-09
	 */
	class UIWindow : public UICommandFactory
	{
	public:
		HWND			GetWnd() { return m_hMainWnd; }
		BOOL			IsExit() { return m_bCommandExit; }
		// ����� ������Ʈ�� �ʱ�ȭ �մϴ�. (�����켳���� ����� �Ŀ� ȣ��)
		virtual VOID	InitObject() {};
		// ����� ������Ʈ�� �����մϴ�. (���η����� ������(EndProcessȣ����) ȣ��)
		virtual VOID	ReleaseObject() {};

		virtual VOID	ShowWindow(int nCmdShow);

		// Windows MsgProc
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		BOOL			Init(HINSTANCE hInstance, int nCmdShow, int iWidth, int iHeight, WCHAR* AppName, WCHAR* Icon);

		virtual VOID	InitializeCommand() {};
		virtual VOID	StartCommand();							// UIConsol���� Command �Է�
		virtual VOID	EndCommand();

	public:
		UIWindow(VOID);
		~UIWindow(VOID);

	private:
		HWND			m_hMainWnd;
		BOOL			m_bCommandExit;
		HFONT			m_hFont;

		VOID			ResizeWindows( VOID );

		// WM_EDIT_RETURN
		virtual VOID	OnEditReturn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		// WM_COMMAND
		virtual VOID	OnCommand(HWND hWnd, INT nID, INT iEvent, LPARAM lParam) { return; }
	};

}