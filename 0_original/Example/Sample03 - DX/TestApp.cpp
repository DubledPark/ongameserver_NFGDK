
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
	// �������� �̹��� ���� ����.
	// �Ǵ� ��ũ��Ʈ�� ���� ������ ������ �������ټ� �־���Ѵ�.
	// �� ������ �ش� �̹����� ��Ʈ�� �ϴ� XML ��ũ��Ʈ�� �����̵ȴ�.
	NFImage* pImage = NFImageManager::GetInstance().AddFromFile(1, L"Login.png");
	// �̹��� ���� ����
	pImage->CreateImageArea(5);
	pImage->SetImageArea(0, 0, 0, 256, 180);
	pImage->SetImageArea(1, 0, 181, 66, 100);
	pImage->SetImageArea(2, 67, 181, 66, 100);
	pImage->SetImageArea(3, 134, 181, 16, 48);
	pImage->SetImageArea(4, 134, 181+16, 16, 16);


	NFImage* pImage2 = NFImageManager::GetInstance().AddFromFile(2, L"ServerSel.png");
	// �̹��� ���� ����
	pImage2->CreateImageArea(7);
	pImage2->SetImageArea(0, 0, 0, 256, 309);
	pImage2->SetImageArea(1, 0, 310, 66, 100);
	pImage2->SetImageArea(2, 67, 310, 66, 100);
	pImage2->SetImageArea(3, 134, 310, 24, 36);
	pImage2->SetImageArea(4, 160, 310, 12, 36);
	pImage2->SetImageArea(5, 173, 310, 12, 23);
	pImage2->SetImageArea(6, 0, 434, 224, 20);

	//////////////////////////////////////////////////////////////////////////////////

	// ���̾�α׸� �����Ѵ�.
	UIDialogManager& udm = UIDialogManager::GetInstance();

	// �� �Ʒ��κ��� Dialog ���� ���̳� ��ũ��Ʈ�� ������ UI ��ũ��Ʈ�� ���ؼ� �����ȴ�.
	// �����̱� ������ ������ ���� ������.

	///////////////////////////////////////////////////////////////////////////////////
	// ID LoginDlg �� ���̾�α׸� �����ϰ�.
	// �̺κ��� UITools �� �̿��� �ڵ�ȭ �ϰ� �ش� ������ XML�� ������ �ȴ�.
	UIDialog* pDlg = udm.CreateUIDialog(L"LoginDlg");
	if(pDlg)
	{
		pDlg->SetPos(50,50);
		// �⺻ ��Ų�� ����.
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

		// Show ����Ʈ�� �߰��Ѵ�.
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
			_swprintf(stTitle, L"Ÿ��Ʋ %d��", i+1);
			pList->AddItem(stTitle);
		}

		UIStatic* pLabel = pDlg2->AddStatic(0x10000004, 15, 247, 240, 20);
		pLabel->SetSkin(pImage2->GetImageStruct(6));
		pLabel->SetAlign(DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		pLabel->SetText(L"����Ʈ�� Ŭ���ϼ���.");

		// Show ����Ʈ�� �߰��Ѵ�.
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