#pragma once
#include "afxwin.h"


// CIDChangeDlg 대화 상자입니다.

class CIDChangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CIDChangeDlg)

public:
	CIDChangeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIDChangeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IDCHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditID;
public:
	afx_msg void OnBnClickedOk();
};
