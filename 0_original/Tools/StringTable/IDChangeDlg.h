#pragma once
#include "afxwin.h"


// CIDChangeDlg ��ȭ �����Դϴ�.

class CIDChangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CIDChangeDlg)

public:
	CIDChangeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CIDChangeDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IDCHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditID;
public:
	afx_msg void OnBnClickedOk();
};
