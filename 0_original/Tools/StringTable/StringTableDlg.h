// StringTableDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CStringTableDlg ��ȭ ����
class CStringTableDlg : public CDialog
{
// �����Դϴ�.
public:
	CStringTableDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STRINGTABLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CQuickList	m_List;

	BOOL		m_fullrow;
	BOOL		m_winColors;
	BOOL		m_normalStyle;

	int			m_iSelectItem;
public:
	afx_msg void OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnOdfinditemList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRdblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

	// ESC, Enter�� Close�� �Ǵ°� �������ؼ� 
	void OnCancel();

public:
	// QuickList ���� �޽���
	afx_msg LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNavigationTest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHeaderRightClick(WPARAM wParam, LPARAM lParam);

	// QuickList�� ������Ʈ �ϱ� ���� �޽���
	void UpdateList();
	void AddLastItem();

	int	iGlobalIndex;

	int FindID(int iMyID, WCHAR* str);

public:
	// SysClose �� ó���ϱ� ���ؼ�.
	afx_msg void OnBnClickedClose();
	afx_msg void OnClose();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_Close;
	CButton m_New;
	CButton m_Load;
	CButton m_Save;
	CButton m_Chang;
public:
	afx_msg void OnBnClickedNew();
public:
	afx_msg void OnBnClickedLoad();
public:
	afx_msg void OnBnClickedSave();
public:
	afx_msg void OnMenuDelete();
public:
	afx_msg void OnBnClickedIdchange();
};
