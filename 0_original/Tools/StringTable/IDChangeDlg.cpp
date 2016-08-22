// IDChangeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StringTable.h"
#include "IDChangeDlg.h"


// CIDChangeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CIDChangeDlg, CDialog)

CIDChangeDlg::CIDChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIDChangeDlg::IDD, pParent)
{

}

CIDChangeDlg::~CIDChangeDlg()
{
}

void CIDChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditID);
}


BEGIN_MESSAGE_MAP(CIDChangeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CIDChangeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CIDChangeDlg 메시지 처리기입니다.
BOOL CIDChangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EditID.SetLimitText(6);
	m_EditID.SetWindowText(g_strID.c_str());

	return TRUE;
}
void CIDChangeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WCHAR strTemp[256];
	m_EditID.GetWindowTextW(strTemp, 256);
	g_strID = strTemp;

	OnOK();
}
