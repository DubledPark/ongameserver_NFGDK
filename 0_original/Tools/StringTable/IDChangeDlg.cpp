// IDChangeDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "StringTable.h"
#include "IDChangeDlg.h"


// CIDChangeDlg ��ȭ �����Դϴ�.

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


// CIDChangeDlg �޽��� ó�����Դϴ�.
BOOL CIDChangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EditID.SetLimitText(6);
	m_EditID.SetWindowText(g_strID.c_str());

	return TRUE;
}
void CIDChangeDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WCHAR strTemp[256];
	m_EditID.GetWindowTextW(strTemp, 256);
	g_strID = strTemp;

	OnOK();
}
