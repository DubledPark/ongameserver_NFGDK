
#include "main.h"
#include "TestApp.h"

#include <Nave2D/UIDialogManager.h>
#include <Nave2D/NFImageManager.h>

using namespace Nave2D;

CTestApp::CTestApp(void)
{
}

CTestApp::~CTestApp(void)
{
}

HRESULT CTestApp::InitObjects()
{
	m_iFrame	= 0;
	m_iCurFrame = 0;
	m_fDt		= 1.0f;

	m_Frame.OnCreate();

	//////////////////////////////////////////////////////////////////////////////////
	// 툴에의한 이미지 영역 생성.
	// 또는 스크립트에 의한 영역을 강제로 생성해줄수 있어야한다.
	// 이 영역은 해당 이미지를 컨트롤 하는 XML 스크립트로 저장이된다.
	NFImage* pImage = NFImageManager::GetInstance().AddFromFile(1, L"Login.png");
	// 이미지 영역 생성
	pImage->CreateImageArea(5);
	pImage->SetImageArea(0, 0, 0, 256, 180);
	pImage->SetImageArea(1, 0, 181, 66, 100);
	pImage->SetImageArea(2, 67, 181, 66, 100);
	pImage->SetImageArea(3, 134, 181, 16, 48);
	pImage->SetImageArea(4, 134, 181+16, 16, 16);


	NFImage* pImage2 = NFImageManager::GetInstance().AddFromFile(2, L"ServerSel.png");
	// 이미지 영역 생성
	pImage2->CreateImageArea(7);
	pImage2->SetImageArea(0, 0, 0, 256, 309);
	pImage2->SetImageArea(1, 0, 310, 66, 100);
	pImage2->SetImageArea(2, 67, 310, 66, 100);
	pImage2->SetImageArea(3, 134, 310, 24, 36);
	pImage2->SetImageArea(4, 160, 310, 12, 36);
	pImage2->SetImageArea(5, 173, 310, 12, 23);
	pImage2->SetImageArea(6, 0, 434, 224, 20);

	//////////////////////////////////////////////////////////////////////////////////

	// 다이얼로그를 생성한다.
	UIDialogManager& udm = UIDialogManager::GetInstance();

	// 이 아래부분은 Dialog 생성 툴이나 스크립트로 생성한 UI 스크립트에 의해서 생성된다.
	// 샘플이기 때문에 간단한 예를 보여줌.

	///////////////////////////////////////////////////////////////////////////////////
	// ID LoginDlg 란 다이얼로그를 생성하고.
	// 이부분을 UITools 을 이용해 자동화 하고 해당 정보는 XML로 저장이 된다.
	UIDialog* pDlg = udm.CreateUIDialog(L"LoginDlg");
	if(pDlg)
	{
		pDlg->SetPos(50,50);
		// 기본 스킨을 생성.
		pDlg->SetSkin(pImage->GetImageStruct(0));
		pDlg->SetVisible(TRUE);

		UIButton* pLogin = pDlg->AddButton(0x00000001, 53, 139, 66, 25);
		pLogin->SetSkin(pImage->GetImageStruct(1));

		UIButton* pExit = pDlg->AddButton(0x00000002, 137, 139, 66, 25);
		pExit->SetSkin(pImage->GetImageStruct(2));

		UIEditBox* pID = pDlg->AddEditBox(0x00000003, 87, 50, 123, 17);
		pID->SetText(L"AAAA");

		UIEditBox* pPass = pDlg->AddEditBox(0x00000004, 87, 81, 123, 17);

		UICheckBox* pCheck = pDlg->AddCheckBox(0x00000005, 195, 109, 16, 16);
		pCheck->SetSkin(pImage->GetImageStruct(3));

		// Show 리스트에 추가한다.
		udm.PushShowDialog(pDlg);
	}
	///////////////////////////////////////////////////////////////////////////////////

	UIDialog* pDlg2 = udm.CreateUIDialog(L"ServerSelDlg");
	if(pDlg2)
	{
		pDlg2->SetPos(150,50);
		pDlg2->SetSkin(pImage2->GetImageStruct(0));
		pDlg2->SetVisible(TRUE);

		UIButton* pLogin = pDlg2->AddButton(0x10000001, 53, 272, 66, 25);
		pLogin->SetSkin(pImage2->GetImageStruct(1));

		UIButton* pExit = pDlg2->AddButton(0x10000002, 137, 272, 66, 25);
		pExit->SetSkin(pImage2->GetImageStruct(2));

		UIListBox* pList = pDlg2->AddListBox(0x10000003, 8, 42, 240, 200);
		pList->SetItemHeight(20);
		pList->SetTextColor(RGB(255,255,255));
		
		pList->SetScrollButtonSkin(pImage2->GetImageStruct(3));
		pList->SetScrollSkin(pImage2->GetImageStruct(4));
		pList->SetScrollThumbSkin(pImage2->GetImageStruct(5));
		pList->SetCursorSkin(pImage2->GetImageStruct(6));

		WCHAR stTitle[32];
		for(int i = 0; i < 20; ++i)
		{
			_swprintf(stTitle, L"타이틀 %d번", i+1);
			pList->AddItem(stTitle);
		}

		UIStatic* pLabel = pDlg2->AddStatic(0x10000004, 15, 247, 240, 20);
		pLabel->SetSkin(pImage2->GetImageStruct(6));
		pLabel->SetAlign(DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		pLabel->SetText(L"리스트를 클릭하세요.");

		// Show 리스트에 추가한다.
		udm.PushShowDialog(pDlg2);
	}

	return S_OK;
}

HRESULT CTestApp::ReleaseObjects()
{
	m_Frame.OnDestroy();

	return S_OK;
}

VOID CTestApp::ChangeWindowMode(BOOL bWindowMode)
{
	UIWindowDX9::ChangeWindowMode(bWindowMode);
}

VOID CTestApp::RenderObject()
{
}

VOID CTestApp::RenderTopmost()
{
	m_Frame.OnRender(0,0);
}

VOID CTestApp::OnUpdate(float dt)
{
	UIWindowDX9::OnUpdate(dt);
	m_fDt -= dt;
	m_iCurFrame++;

	if (m_fDt <= 0)
	{
		WCHAR strBuff[256];
		m_fDt = 1.0f;
		m_iFrame = m_iCurFrame;
		m_iCurFrame = 0;

		swprintf(strBuff, L"Frame : %d", m_iFrame);
		m_Frame.TextOut(255, RGB(255,0,0), strBuff);
	}
}

VOID CTestApp::OnObjectEvent(INT nID, INT iEvent, LPARAM lParam)
{

}

LRESULT CTestApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_SYSKEYDOWN :
		{
			int wmId, wmEvent;
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);

			switch(wmId)
			{
			case VK_RETURN :
				ChangeWindowMode(!m_bWindowMode);
				break;
			}
		}
		break;
	}
	return UIWindowDX9::MsgProc(hWnd, uMsg, wParam, lParam);
}