#pragma once 

class CTestApp : public Nave2D::UIWindowDX9
{
	int		m_iFrame;
	int		m_iCurFrame;
	float	m_fDt;

	Nave2D::NFText	m_Frame;

protected :
	HRESULT InitObjects();
	HRESULT ReleaseObjects();

public :
	VOID OnUpdate(float dt);
	VOID ChangeWindowMode(BOOL bWindowMode);
	VOID RenderObject();
	VOID RenderTopmost();
	VOID OnObjectEvent(INT nID, INT iEvent, LPARAM lParam);
	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public :
	CTestApp(void);
	virtual ~CTestApp(void);
};