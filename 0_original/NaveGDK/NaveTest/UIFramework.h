/** 
 *  @file  		UIFramework.h
 *  @brief 		UI�� ���� Ŭ����
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-11
 */
#pragma once

#include <Nave/NFLog.h>
#include <Nave/NFStringUtil.h>
#include <vector>

namespace NaveTest {

	/**
	 * @brief		���ο� Ŀ�ǵ带 �߰��մϴ�.
	 * @param cmd		Ŀ�ǵ� 
	 * @param object	Ŀ�ǵ带 ������ ��ü Ŭ������
	 * @param msg		��� �޽���
	 */
	#define ADD_COMMAND(cmd, object, msg) AddCommand(cmd, new object, msg)

	/** 
	 *  @class		  UICommand  
	 *  @brief        Ŀ�ǵ� ��ü�� �����ϴ� Ŭ����
	 *  @remarks      class CCmdHelp : public NaveTest::UICommand		\r\n
	 *                {													\r\n
	 *                public:											\r\n
     *					  virtual BOOL DoProcess(WCHAR* lpParam)		\r\n
	 *                    {												\r\n
	 *                		if(g_pMainWnd) g_pMainWnd->ShowCommand();	\r\n
	 *                        return TRUE;								\r\n
	 *                    }												\r\n
	 *                };												\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */ 
	class UICommand
	{
	public:
		/**
		 * @brief		Ŀ�ǵ带 ó���մϴ�.
		 * @param lpParam	Ŀ��Ʈ �߰�	�Ķ����
		 * @return ��������
		 */
		virtual BOOL DoProcess(WCHAR* lpParam) = 0;    
	};

	/** 
	 *  @class		  UICommandFactory  
	 *  @brief        Ŀ�ǵ� ��ü�� ����ϱ� ���� ��ü �ش� ��ü�� ��ӹ޾Ƽ� ���.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */ 
	class UICommandFactory
	{
	public:
		UICommandFactory()
		{
		}
		virtual ~UICommandFactory()
		{
			ReleaseCommand();
		}

	public:
		/**
		 * @brief		���ο� Ŀ�ǵ带 �߰��Ѵ�.
		 * @param szCommand	Ŀ�ǵ�
		 * @param lpCommand	Ŀ�ǵ� ��ü
		 * @param szMessage	Ŀ�ǵ� ����
		 * @return ��������
		 */
	    BOOL AddCommand(const WCHAR* szCommand, UICommand* lpCommand, const WCHAR* szMessage)
		{
			if(0 == szCommand, 0 == lpCommand)
				return FALSE;    

			m_CMDVector.push_back(COMMAND(szCommand, lpCommand, szMessage));
			return TRUE;
		}

		/// ��� Ŀ�ǵ带 �����մϴ�.
		VOID ReleaseCommand()
		{
			int iSize = (int)m_CMDVector.size();
			for(int i = 0; i < iSize; ++i)
			{
				delete m_CMDVector[i].m_lpCMD;
			}

			m_CMDVector.clear();
		}
		/// Ŀ�ǵ带 �߰��մϴ�.
		virtual VOID	InitializeCommand() {};
		/// Ŀ�ǵ� �����찡 ���۵˴ϴ�.
		virtual VOID	StartCommand() {}; 							// Consol���� Command �Է�
		/// Ŀ�ǵ� �����찡 ����˴ϴ�.
		virtual VOID	EndCommand() {};

		/// ȭ�鿡 ��ϵ� Ŀ�ǵ� ������ ����մϴ�.
		VOID ShowCommand()
		{
			LOG_IMPORTANT( (L"------------------- Commands -------------------") );
			int iSize = (int)m_CMDVector.size();
			for(int i = 0; i < iSize; ++i)
			{
				LOG_IMPORTANT((L"%s : %s", m_CMDVector[i].m_szCMD, m_CMDVector[i].m_szMSG));
			}
			LOG_IMPORTANT( (L"----------------- Commands End -----------------") );
		}

		/**
		 * @brief		Ŀ�ǵ带 ó���մϴ�.
		 * @param Command	Ŀ�ǵ� ���ڿ�
		 * @return ��������
		 */
		VOID DoCommand(WCHAR* Command)
		{
			WCHAR Buff[MAX_BUFSIZE];
			ZeroMemory(Buff, MAX_BUFSIZE);
			wcscpy(Buff, Command);

			WCHAR* cmd;
			cmd = wcstok(Command, L" ");
			if(cmd)
			{
				Nave::uint32 uiHash = Nave::Hash(cmd);
				WCHAR* NextCmd = &Buff[wcslen(cmd)+1];

				int iSize = (int)m_CMDVector.size();
				for(int i = 0; i < iSize; ++i)
				{
					if(m_CMDVector[i].m_uiHash != uiHash)
						continue;

					m_CMDVector[i].m_lpCMD->DoProcess(NextCmd);
					return;
				}
			}
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
	 *  @class		  UIWindow  
	 *  @brief        Command�� ����ϴ� ������ �����ӿ�ũ.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */ 
	class UIWindow : public UICommandFactory  
	{
	public:
		UIWindow(VOID);
		virtual ~UIWindow(VOID);

	public:
		/**
		 * @brief	���� ������ �ڵ��� ���ɴϴ�.
		 * @return	���������� �ڵ�
		 */
		HWND	GetWnd() { return m_hMainWnd; }
		/**
		 * @brief	���� �����찡 ����Ǿ����� Ȯ���մϴ�.
		 * @return	���� ����
		 */
		BOOL	IsExit() { return m_bCommandExit; }

		/// ����� ������Ʈ�� �ʱ�ȭ �մϴ�. (�����켳���� ����� �Ŀ� ȣ��)
		virtual VOID InitObject() {};
		/// ����� ������Ʈ�� �����մϴ�. (���η����� ������(EndProcessȣ����) ȣ��)
		virtual VOID ReleaseObject() {};

		/// �����츦 SHOW �մϴ�.
		virtual VOID	ShowWindow(int nCmdShow);

		/// Windows MsgProc
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		/**
		 * @brief		���������츦 �ʱ�ȭ�մϴ�.
		 * @param hInstance	�ν��Ͻ� �ڵ�
		 * @param nCmdShow	Ȱ��ȭ Ŀ�ǵ�
		 * @param AppName	������ �̸�
		 * @param Icon		������ �̸�
		 * @return ��������
		 */
		BOOL			Init(HINSTANCE hInstance, int nCmdShow, WCHAR* AppName, WCHAR* Icon);

		/// Ŀ�ǵ带 �߰��մϴ�.
		virtual VOID	InitializeCommand() {};
		/// Ŀ�ǵ� �����찡 ���۵˴ϴ�.
		virtual VOID	StartCommand(); 							// Consol���� Command �Է�
		/// Ŀ�ǵ� �����찡 ����˴ϴ�.
		virtual VOID	EndCommand();

	private:
		HWND			m_hMainWnd;
		BOOL			m_bCommandExit;
		HFONT			m_hFont;

		/// ������ ũ�� ������
		VOID	ResizeWindows( VOID );

		/// WM_EDIT_RETURN
		virtual VOID OnEditReturn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		/// WM_COMMAND
		virtual VOID OnCommand(HWND hWnd, INT nID, INT iEvent, LPARAM lParam) { return; }
	};

}

