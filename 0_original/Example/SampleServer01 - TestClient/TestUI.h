// LabyrinthUI.h: interface for the CTestUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LABYRINTHUI_H__5068B9A5_51BB_4371_AFFE_42A05C23DA62__INCLUDED_)
#define AFX_LABYRINTHUI_H__5068B9A5_51BB_4371_AFFE_42A05C23DA62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetTest.h"

class CTestUI : public NaveTest::UIWindow
			, public CNetTest::IEventListener
{
	CNetTest	m_Net;
	BOOL		m_bSend;

	CHAR		m_strSend[508];

	DWORD		m_PrevTick;
	int			m_iRecv;
	int			m_iSend;

	VOID OnCommand(HWND hWnd, INT nID, INT iEvent, LPARAM lParam);
	
public:
	VOID Update();

	virtual void EventIRC(CHAR* strCmd, CHAR* strMsg);
	virtual void EventTEST(CHAR* strMsg);
	
	virtual void EventConnect(BOOL bConnect);
	virtual void EventSocketClose();

public:
	VOID	InitializeCommand();

	// 사용자 오브젝트를 초기화 합니다. (윈도우설정이 종료됀 후에 호출)
    VOID InitObject();
	// 사용자 오브젝트를 삭제합니다. (메인루프가 끊난후(EndProcess호출후) 호출)
    VOID ReleaseObject();

	VOID	StartCommand();
	// Windows MsgProc
    LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Connect(WCHAR* strParam);
	void SendTest();
	void SendMsg(WCHAR* strParam);

public:	
	VOID						UpdateInfo();

	CTestUI();
	virtual ~CTestUI();

};

#endif // !defined(AFX_LABYRINTHUI_H__5068B9A5_51BB_4371_AFFE_42A05C23DA62__INCLUDED_)
