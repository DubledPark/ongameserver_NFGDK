/** 
 *  @file  		UIFramework.h
 *  @brief 		UI에 관련 클래스
 *  @remarks 
 *  @author  	강동명(edith2580@gmail.com)
 *  @date  		2009-05-11
 */
#pragma once

#include <Nave/NFLog.h>
#include <Nave/NFStringUtil.h>
#include <vector>

namespace NaveTest {

	/**
	 * @brief		새로운 커맨드를 추가합니다.
	 * @param cmd		커맨드 
	 * @param object	커맨드를 관리할 객체 클래스명
	 * @param msg		출력 메시지
	 */
	#define ADD_COMMAND(cmd, object, msg) AddCommand(cmd, new object, msg)

	/** 
	 *  @class		  UICommand  
	 *  @brief        커맨드 객체를 관리하는 클래스
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
		 * @brief		커맨드를 처리합니다.
		 * @param lpParam	커멘트 추가	파라메터
		 * @return 성공여부
		 */
		virtual BOOL DoProcess(WCHAR* lpParam) = 0;    
	};

	/** 
	 *  @class		  UICommandFactory  
	 *  @brief        커맨드 객체를 사용하기 위한 객체 해당 객체를 상속받아서 사용.
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
		 * @brief		새로운 커맨드를 추가한다.
		 * @param szCommand	커맨드
		 * @param lpCommand	커맨드 객체
		 * @param szMessage	커맨드 설명
		 * @return 성공여부
		 */
	    BOOL AddCommand(const WCHAR* szCommand, UICommand* lpCommand, const WCHAR* szMessage)
		{
			if(0 == szCommand, 0 == lpCommand)
				return FALSE;    

			m_CMDVector.push_back(COMMAND(szCommand, lpCommand, szMessage));
			return TRUE;
		}

		/// 모든 커맨드를 삭제합니다.
		VOID ReleaseCommand()
		{
			int iSize = (int)m_CMDVector.size();
			for(int i = 0; i < iSize; ++i)
			{
				delete m_CMDVector[i].m_lpCMD;
			}

			m_CMDVector.clear();
		}
		/// 커맨드를 추가합니다.
		virtual VOID	InitializeCommand() {};
		/// 커맨드 윈도우가 시작됩니다.
		virtual VOID	StartCommand() {}; 							// Consol형식 Command 입력
		/// 커맨드 윈도우가 종료됩니다.
		virtual VOID	EndCommand() {};

		/// 화면에 등록된 커맨드 정보를 출력합니다.
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
		 * @brief		커맨드를 처리합니다.
		 * @param Command	커맨드 문자열
		 * @return 성공여부
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
	 *  @brief        Command를 사용하는 윈도우 프레임워크.
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
		 * @brief	메인 윈도우 핸들을 얻어옵니다.
		 * @return	메인윈도우 핸들
		 */
		HWND	GetWnd() { return m_hMainWnd; }
		/**
		 * @brief	메인 윈도우가 종료되었는지 확인합니다.
		 * @return	종료 여부
		 */
		BOOL	IsExit() { return m_bCommandExit; }

		/// 사용자 오브젝트를 초기화 합니다. (윈도우설정이 종료됀 후에 호출)
		virtual VOID InitObject() {};
		/// 사용자 오브젝트를 삭제합니다. (메인루프가 끊난후(EndProcess호출후) 호출)
		virtual VOID ReleaseObject() {};

		/// 윈도우를 SHOW 합니다.
		virtual VOID	ShowWindow(int nCmdShow);

		/// Windows MsgProc
		virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		/**
		 * @brief		메인윈도우를 초기화합니다.
		 * @param hInstance	인스턴스 핸들
		 * @param nCmdShow	활성화 커맨드
		 * @param AppName	윈도우 이름
		 * @param Icon		아이콘 이름
		 * @return 성공여부
		 */
		BOOL			Init(HINSTANCE hInstance, int nCmdShow, WCHAR* AppName, WCHAR* Icon);

		/// 커맨드를 추가합니다.
		virtual VOID	InitializeCommand() {};
		/// 커맨드 윈도우가 시작됩니다.
		virtual VOID	StartCommand(); 							// Consol형식 Command 입력
		/// 커맨드 윈도우가 종료됩니다.
		virtual VOID	EndCommand();

	private:
		HWND			m_hMainWnd;
		BOOL			m_bCommandExit;
		HFONT			m_hFont;

		/// 윈도우 크기 재조정
		VOID	ResizeWindows( VOID );

		/// WM_EDIT_RETURN
		virtual VOID OnEditReturn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		/// WM_COMMAND
		virtual VOID OnCommand(HWND hWnd, INT nID, INT iEvent, LPARAM lParam) { return; }
	};

}

