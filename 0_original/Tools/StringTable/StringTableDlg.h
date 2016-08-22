// StringTableDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CStringTableDlg 대화 상자
class CStringTableDlg : public CDialog
{
// 생성입니다.
public:
	CStringTableDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STRINGTABLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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

	// ESC, Enter로 Close가 되는걸 막기위해서 
	void OnCancel();

public:
	// QuickList 전용 메시지
	afx_msg LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNavigationTest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHeaderRightClick(WPARAM wParam, LPARAM lParam);

	// QuickList를 업데이트 하기 위한 메시지
	void UpdateList();
	void AddLastItem();

	int	iGlobalIndex;

	int FindID(int iMyID, WCHAR* str);

public:
	// SysClose 를 처리하기 위해서.
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
