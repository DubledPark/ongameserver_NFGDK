// StringTableDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "StringTable.h"
#include "StringTableDlg.h"
#include "IDChangeDlg.h"
#include <Nave/Nave.h>
#include <Nave/NFTokenizer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CStringTableDlg ��ȭ ����




CStringTableDlg::CStringTableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStringTableDlg::IDD, pParent)
{
	m_fullrow = TRUE;
	m_winColors = TRUE;
	m_normalStyle = FALSE;
	m_iSelectItem = -1;
	iGlobalIndex = 0;


	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStringTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_CLOSE, m_Close);
	DDX_Control(pDX, IDC_NEW, m_New);
	DDX_Control(pDX, IDC_LOAD, m_Load);
	DDX_Control(pDX, IDC_Save, m_Save);
	DDX_Control(pDX, IDC_IDCHANGE, m_Chang);
}

BEGIN_MESSAGE_MAP(CStringTableDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, &CStringTableDlg::OnLvnBegindragList1)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST1, &CStringTableDlg::OnLvnBeginlabeleditList1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, &CStringTableDlg::OnLvnEndlabeleditList1)
	ON_NOTIFY(LVN_ODFINDITEM, IDC_LIST1, &CStringTableDlg::OnLvnOdfinditemList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CStringTableDlg::OnNMRclickList1)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST1, &CStringTableDlg::OnNMRdblclkList1)


	//////////////////////////////////////////////
	// QuickList����
	ON_MESSAGE(WM_QUICKLIST_NAVIGATIONTEST, OnNavigationTest) 
	ON_MESSAGE(WM_QUICKLIST_CLICK, OnListClick) 
	ON_MESSAGE(WM_QUICKLIST_HEADERRIGHTCLICK, OnHeaderRightClick)
	ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, OnGetListItem) 
	ON_BN_CLICKED(IDC_CLOSE, &CStringTableDlg::OnBnClickedClose)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_NEW, &CStringTableDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_LOAD, &CStringTableDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_Save, &CStringTableDlg::OnBnClickedSave)
	ON_COMMAND(ID_MENU_DELETE, &CStringTableDlg::OnMenuDelete)
	ON_BN_CLICKED(IDC_IDCHANGE, &CStringTableDlg::OnBnClickedIdchange)
END_MESSAGE_MAP()


// CStringTableDlg �޽��� ó����

BOOL CStringTableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	g_strID = L"ID_NONE";


	////////////////////////////////////////////////////////////////////////
	// QuickList �ʱ�ȭ �� ����.
	////////////////////////////////////////////////////////////////////////
	ListView_SetExtendedListViewStyleEx(m_List.m_hWnd, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
//	ListView_SetExtendedListViewStyleEx(m_List.m_hWnd, LVS_EX_SUBITEMIMAGES , LVS_EX_SUBITEMIMAGES );
//	ListView_SetExtendedListViewStyleEx(m_List.m_hWnd, LVS_EX_TRACKSELECT, LVS_EX_TRACKSELECT );
//	ListView_SetExtendedListViewStyleEx(m_List.m_hWnd, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES );
	ListView_SetExtendedListViewStyleEx(m_List.m_hWnd, LVS_EX_HEADERDRAGDROP, LVS_EX_HEADERDRAGDROP);

	// Gride
	ListView_SetExtendedListViewStyleEx(m_List.m_hWnd, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);


	m_List.InsertColumn(0, L"Number", LVCFMT_LEFT, 50);
	m_List.InsertColumn(1, L"ID", LVCFMT_LEFT, 120);
	m_List.InsertColumn(2, L"String", LVCFMT_LEFT, 500);


	// Ű���带 �̿��� Select Cursor�� Move��Ű�� ���� �Լ���FALSE �̸� Keydown���� Ŀ���� �̵����� ���Ѵ�.
	m_List.EnableColumnNavigation(TRUE);

	// 0�� Į���� FineKey�� ����Ѵ�.
	m_List.SetKeyfindColumn(0);

	m_List.SetEditOnDblclk(TRUE);
	m_List.SetEditOnEnter(TRUE);
	m_List.SetEditOnF2(TRUE);

	// �̰��� False �� �Ǿ��־�� Keyfind�� ����Ҽ� �ִ�.
	m_List.SetEditOnWriting(FALSE);

//	m_List.EnableToolTips(TRUE);

	AddLastItem();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CStringTableDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CStringTableDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CStringTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStringTableDlg::OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

void CStringTableDlg::OnLvnBeginlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
/*	if(pDispInfo->item.iSubItem == 0)
	{
		lstrcpyn(	pDispInfo->item.pszText, 
					g_tableData[pDispInfo->item.iItem].id.c_str(), 
					pDispInfo->item.cchTextMax);
	}
*/
	*pResult = 0;
}

int CStringTableDlg::FindID(int iMyID, WCHAR* str)
{
	int iSize = g_tableData.size();

	for(int i = 0; i < iSize; ++i)
	{
		if(iMyID == i)
			continue;

		if(_tcscmp(g_tableData[i].id, str) == 0)
			return i+1;
	}
	return 0;
}

void CStringTableDlg::OnLvnEndlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(pDispInfo->item.pszText != NULL)
	{
		if(pDispInfo->item.iSubItem == 1)
		{
			int iPos = FindID(pDispInfo->item.iItem, pDispInfo->item.pszText);
			if(iPos != 0)
			{
				WCHAR str[512];
				_stprintf(str, L"���� ID�� %d���� �����մϴ�.", iPos);
				MessageBox(str, L"�ߺ� ���̵� �˻�");
				*pResult = 0;
				return;
			}

			wcscpy(g_tableData[pDispInfo->item.iItem].id,pDispInfo->item.pszText);
		}
		else if(pDispInfo->item.iSubItem == 2)
			wcscpy(g_tableData[pDispInfo->item.iItem].str, pDispInfo->item.pszText);

		//Select next item?
		if(m_List.GetLastEndEditKey() == VK_RETURN)
		{
			//If column 0, select in next column
			if(pDispInfo->item.iSubItem == 1)
			{
				m_List.EditSubItem(pDispInfo->item.iItem, 2);
			}
			else
			{
				//Select next item if possible
				if(pDispInfo->item.iItem+1 < m_List.GetItemCount())
					m_List.EditSubItem(pDispInfo->item.iItem+1, 1);
				else
				{
					// ���������� Enter�� ������ ���ο� �������� �ϴܿ� �߰��Ѵ�.
					AddLastItem();

					m_List.EditSubItem(pDispInfo->item.iItem+1, 1);
				}
			}
		}
	}

	*pResult = 0;
}

void CStringTableDlg::OnLvnOdfinditemList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVFINDITEM pFindInfo = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	TRACE(L"LVN_ODFINDITEM send to parent.\n");

	*pResult = 0;
}

void CStringTableDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


	POINT pt = Nave::GetMousePos();

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU1));

	m_iSelectItem = pNMListView->iItem;

	//Pop up sub menu 0
	CMenu* popup = menu.GetSubMenu(0);
	
	popup->TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON,
							pt .x, 
							pt .y, 
							this); //AfxGetMainWnd());


/*
	int item=-1, subitem=-1;
	bool oncheck=false;
	bool onimage=false;

	if(m_list.HitTest(pNMListView->ptAction, item, subitem, &oncheck, &onimage))
	{
		TRACE(L"User right hit item %d, subitem %d \n", item, subitem);

		if(!oncheck && !onimage)
		{
			//Hit on text. Toggle bold/italic
			int temp =  (m_data[item].m_bold[subitem] ? 1: 0) + 
						(m_data[item].m_italic[subitem] ? 2: 0);

			temp =(temp+1)%4;

			bool a=((temp&0x01) != 0);
			m_data[item].m_bold[subitem] =  ( (temp&0x01) != 0);
			a=((temp&0x02) != 0);
			m_data[item].m_italic[subitem] = ( (temp&0x02) != 0);
			
			m_list.RedrawSubitems(item, item, subitem);
		}
	}
*/

	*pResult = 0;
}

void CStringTableDlg::OnNMRdblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int item=-1, subitem=-1;

	if(m_List.HitTest(pNMListView->ptAction, item, subitem))
	{
		m_List.SetHotItem(item);
	}

	*pResult = 0;
}

// Quick List�� �޽��� 
// Item�� ������������ ó����ƾ.
LRESULT CStringTableDlg::OnListClick(WPARAM wParam, LPARAM lParam)
{
	//Make sure message comes from list box
	ASSERT( (HWND)wParam == m_List.GetSafeHwnd() );

	CQuickList::CListHitInfo *hit= (CQuickList::CListHitInfo*) lParam;

	TRACE(L"User hit item %d, subitem %d ", hit->m_item, hit->m_subitem);

	/*
	if(hit->m_onButton)
	{
		TRACE(L"On check box. ");
		m_data[hit->m_item].m_check[hit->m_subitem] = !m_data[hit->m_item].m_check[hit->m_subitem];

		m_list.RedrawCheckBoxs(hit->m_item, hit->m_item, hit->m_subitem);
	}
	else
	if(hit->m_onImage)
	{
		TRACE(L"On image. ");
		m_data[hit->m_item].m_image[hit->m_subitem] = (m_data[hit->m_item].m_image[hit->m_subitem]+1)%IMAGECOUNT;

		m_list.RedrawImages(hit->m_item, hit->m_item, hit->m_subitem);
	}


	TRACE(L"\n");
*/
	return 0;
}

// Quick List�� �޽��� 
// KeyDown�� �̿��� Select Cursor�� �̵���Ű�� ���� ������ 
// test->m_allowChange �� false �̸� ���콺�� ���̻� �̵��� ���Ѵ�.
LRESULT CStringTableDlg::OnNavigationTest(WPARAM wParam, LPARAM lParam)
{
#ifndef QUICKLIST_NONAVIGATION
	//Make sure message comes from list box
	ASSERT( (HWND)wParam == m_List.GetSafeHwnd() );

	CQuickList::CNavigationTest* test = (CQuickList::CNavigationTest*) lParam;

	//Don't allow navigation to progress column
	if(test->m_newColumn == 3)
		test->m_allowChange = false;

#endif

	return 0;
}

// Quick List�� �޽��� 
// Header���� RClick �� ������ ó�� TrackPopup�� Show�Ҷ� ���
LRESULT CStringTableDlg::OnHeaderRightClick(WPARAM wParam, LPARAM lParam)
{
	//Make sure message comes from list box
	ASSERT( (HWND)wParam == m_List.GetSafeHwnd() );

	CQuickList::CHeaderRightClick *hit= 
		(CQuickList::CHeaderRightClick*) lParam;
/*
	//Load menu
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU1));

	//Pop up sub menu 0
	CMenu* popup = menu.GetSubMenu(0);
	
	popup->TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON,
							hit->m_mousePos.x, 
							hit->m_mousePos.y, 
							this); //AfxGetMainWnd());
*/	

	return 0;
}

// Quick List�� �޽��� 
// ���ʵ�ο츦 �̿��� ListItem�� ����ϴ� �κ��̴�.
LRESULT CStringTableDlg::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
	ASSERT( (HWND)wParam == m_List.GetSafeHwnd() );

	//lParam is a pointer to the data that is needed for the element
	CQuickList::CListItemData* data = (CQuickList::CListItemData*) lParam;

	//Get which item and subitem that is asked for.
	int item = data->GetItem();
	int subItem = data->GetSubItem();

	CTableData& tdata = g_tableData[item];

	UpdateData();

	// ȣ���� subItem�� ������������ �Ѵ�.
#ifndef QUICKLIST_NOEDIT
	if(subItem > 0)		// Edit��Ű�� ���ؼ�..
	{
		data->m_allowEdit = true;
//		if(m_fullrow || subItem == 0)
	}
#endif

	// ����� �÷��ϰ��
#ifndef QUICKLIST_NOCOLORS
	if(m_winColors==0)
	{
		if(data->GetItem()%2 == 0)
		{
			data->m_colors.m_textColor =			RGB( 10, 5, 20);
			data->m_colors.m_backColor =			RGB( 180, 180, 255);
			#ifndef QUICKLIST_NONAVIGATION
			data->m_colors.m_navigatedTextColor =	RGB( 200, 220, 230);
			data->m_colors.m_navigatedBackColor =	RGB( 140, 60, 70);
			#endif

			data->m_colors.m_hotTextColor =			RGB( 170, 10, 150);
			
			data->m_colors.m_selectedBackColorNoFocus = RGB( 20, 20, 120);
			data->m_colors.m_selectedBackColor =	RGB( 30, 0, 96);
			data->m_colors.m_selectedTextColor =	RGB( 240, 200, 128);
		}
		else
		{
			data->m_colors.m_textColor =			RGB( 40, 10, 40);
			data->m_colors.m_backColor =			RGB( 220, 220, 255);
			#ifndef QUICKLIST_NONAVIGATION
			data->m_colors.m_navigatedTextColor =	RGB( 200, 200, 230);
			data->m_colors.m_navigatedBackColor =	RGB( 160, 70, 70);
			#endif

			data->m_colors.m_hotTextColor =			RGB( 170, 10, 150);
			
			data->m_colors.m_selectedBackColorNoFocus = RGB( 20, 60, 120);
			data->m_colors.m_selectedBackColor =	RGB( 30, 50, 96);
			data->m_colors.m_selectedTextColor =	RGB( 240, 250, 128);
		}

	}
#endif
/*
#ifndef QUICKLIST_NOTEXTSTYLE 
	if(m_normalStyle==0)
	{
		data->m_textStyle.m_bold = data.m_bold[subItem];
		data->m_textStyle.m_italic = data.m_italic[subItem];
	}
#endif
*/

	// ���Ⱑ ȭ�鿡 ���� ������ �ѷ��ִ� �κ�
	if(data->GetSubItem() == 0)
	{
		data->m_text = Nave::ToNumber(item+1).c_str();
	}
	else if(data->GetSubItem() == 1)
	{
		data->m_text = Nave::ToString(tdata.id).c_str();
	}
	else
	{
		data->m_text = Nave::ToString(tdata.str).c_str();
	}
/*
#ifndef QUICKLIST_NOTOOLTIP
	//Tool tip text
	data->m_tooltip = CString(L"Tooltip: ") + data->m_text;
#endif
*/
	return 0;
}

void CStringTableDlg::UpdateList()
{
	m_List.LockWindowUpdate();
	m_List.SetItemCount( (int) g_tableData.size() );
	m_List.UnlockWindowUpdate();
	m_List.RedrawItems(
			m_List.GetTopIndex(),
			m_List.GetTopIndex()+m_List.GetCountPerPage());
}

void CStringTableDlg::AddLastItem()
{
	g_tableData.push_back( CTableData(g_strID, ++iGlobalIndex) );
	UpdateList();
}

void CStringTableDlg::OnCancel()
{
	return;
}

void CStringTableDlg::OnBnClickedClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog::OnOK();
}

void CStringTableDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnOK();
}

void CStringTableDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	RECT rect;
	GetClientRect(&rect);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	m_List.MoveWindow(10, 10, w-20, h-60);

	m_Close.GetClientRect(&rect);
	m_Close.MoveWindow(w-rect.right-10, h-rect.bottom-10, rect.right, rect.bottom);

	m_New.GetClientRect(&rect);
	m_New.MoveWindow(10, h-rect.bottom-10, rect.right, rect.bottom);
	int ileft = 10+rect.right+10;
	m_Load.MoveWindow(ileft, h-rect.bottom-10, rect.right, rect.bottom);
	ileft = ileft+rect.right+10;
	m_Save.MoveWindow(ileft, h-rect.bottom-10, rect.right, rect.bottom);
	ileft = ileft+rect.right+15;
	m_Chang.MoveWindow(ileft, h-rect.bottom-10, rect.right, rect.bottom);
	
}

void CStringTableDlg::OnBnClickedNew()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	iGlobalIndex = 0;
	g_tableData.clear();

	AddLastItem();

	UpdateList();
}

void CStringTableDlg::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WCHAR szFile[1024];
	WCHAR szFileTitle[_MAX_PATH];
	OPENFILENAME ofn;
	BOOL opchk;

	memset(szFile, 0, sizeof(szFile));
	memset(szFileTitle, 0, sizeof(szFileTitle));
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.nMaxFile = 1024;
	ofn.nMaxFileTitle = _MAX_PATH;
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile         = szFile;
	ofn.lpstrFileTitle    = szFileTitle;

	ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
	ofn.lpstrTitle = L"String Table Load";
	ofn.lpstrFilter = L"String Table File(*.stb)\0*.stb\0CSV (��ǥ�κи�) (*.csv)\0*.csv\0All Files(*.*)\0*.*\0\0";
	ofn.lpstrDefExt = L"";
	opchk = GetOpenFileName(&ofn);
	if (opchk == FALSE) 
		return;	

	int len = wcslen(ofn.lpstrFileTitle);
	WCHAR strExt[8];
	wcscpy(strExt, &ofn.lpstrFileTitle[len-3]);

	if(wcscmp(strExt, L"csv") == 0)
	{
		FILE* fp = _wfopen(ofn.lpstrFile, L"rt");
		if(fp == NULL)
			return;

		/////////////////////////////
		// �ʱ�ȭ.
		g_tableData.clear();
		/////////////////////////////

		CTableData data;

		char strRead[4096];

		while(!feof(fp))
		{
			memset(strRead, 0, 4096);
			fgets(strRead, 4096, fp);

			len = strlen(strRead);

			if(len == 0)
				break;

			// ������ 1���� ��������̹Ƿ� �̰ɷ� ��������� �������ش�.
			strRead[len-1] = 0;

			Nave::NFTokenizerW token(Nave::ToString(strRead), L",");

			wcscpy(data.id, token.NextToken().c_str());
			len = wcslen(data.id);
			wcscpy(data.str, token.NextToken().c_str());

			g_tableData.push_back(data);
		}

		fclose(fp);

		iGlobalIndex = g_tableData.size();
		UpdateList();
		return;
	}

	// �����մϴ�..
	FILE* fp = _wfopen(ofn.lpstrFile, L"r");
	if(fp == NULL)
		return;

	NaveFramework::NFStringTable::HEADER stHeader;
	fread(&stHeader, sizeof(stHeader), 1, fp);

	if(strcmp(stHeader.sFI, "stb") != 0)
	{
		fclose(fp);
		return;
	}

	/////////////////////////////
	// �ʱ�ȭ.
	iGlobalIndex = stHeader.iCount;
	g_tableData.clear();
	/////////////////////////////

	int iTotalCnt = stHeader.iCount;
	int iLen;

	CTableData data;

	WCHAR strBuff[1024];
	for(int i = 0; i < iTotalCnt; ++i)
	{
		// ID
		fread(&iLen, sizeof(int), 1, fp);
		fread(strBuff, sizeof(WCHAR)*iLen, 1, fp);
		strBuff[iLen] = 0;

		wcscpy(data.id, Nave::ToString(strBuff).c_str());

		// String
		fread(&iLen, sizeof(int), 1, fp);
		fread(strBuff, sizeof(WCHAR)*iLen, 1, fp);
		strBuff[iLen] = 0;

		wcscpy(data.str, Nave::ToString(strBuff).c_str());

		g_tableData.push_back(data);
	}

	fclose(fp);

	UpdateList();
}

void CStringTableDlg::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WCHAR szFile[1024];
	WCHAR szFileTitle[_MAX_PATH];
	OPENFILENAME ofn;
	BOOL opchk;

	memset(szFile, 0, sizeof(szFile));
	memset(szFileTitle, 0, sizeof(szFileTitle));
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.nMaxFile = 1024;
	ofn.nMaxFileTitle = _MAX_PATH;
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile         = szFile;
	ofn.lpstrFileTitle    = szFileTitle;

	ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
	ofn.lpstrTitle = L"String Table Save";
	ofn.lpstrFilter = L"String Table File(*.stb)\0*.stb\0CSV (��ǥ�κи�) (*.csv)\0*.csv\0All Files(*.*)\0*.*\0\0";
	ofn.lpstrDefExt = L"";
	opchk = GetSaveFileName(&ofn);
	if (opchk == FALSE) 
		return;	

	int len = wcslen(ofn.lpstrFileTitle);
	WCHAR strExt[8];
	wcscpy(strExt, &ofn.lpstrFileTitle[len-3]);

	if(wcscmp(strExt, L"csv") == 0)
	{
		CFile cFile(ofn.lpstrFile, CFile::modeCreate|CFile::modeWrite);

		BYTE Header[2] = { 0xFF, 0xFE };
		cFile.Write(Header, 2);

		int iCount = g_tableData.size();

		Nave::String strid;
		Nave::String strstr;
		Nave::String strBuff;

		for(int i = 0; i < iCount; ++i)
		{
			strid = g_tableData[i].id;
			strstr = g_tableData[i].str;

			strBuff = strid+L","+strstr+L"\n";

			cFile.Write(strBuff.c_str(), strBuff.size()*sizeof(WCHAR));
		}

		cFile.Close();

		return;
	}

	// �����մϴ�..
	FILE* fp = _wfopen(ofn.lpstrFile, L"w");
	if(fp == NULL)
		return;

	NaveFramework::NFStringTable::HEADER stHeader;
	ZeroMemory(&stHeader, sizeof(stHeader));

	strcpy(stHeader.sFI, "stb");

	stHeader.iCount = g_tableData.size();

	fwrite(&stHeader, sizeof(stHeader), 1, fp);

	int iLen;
	for(int i = 0; i < stHeader.iCount; ++i)
	{
		iLen = wcslen(g_tableData[i].id);

		fwrite(&iLen, sizeof(int), 1, fp ); 
		fwrite(g_tableData[i].id, sizeof(WCHAR)*iLen, 1, fp); 

		iLen = wcslen(g_tableData[i].str);

		fwrite(&iLen, sizeof(int), 1, fp); 
		fwrite(g_tableData[i].str, sizeof(WCHAR)*iLen, 1, fp); 
	}

	fclose(fp);
}

void CStringTableDlg::OnMenuDelete()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if(m_iSelectItem == -1)
		return;

	std::vector<CTableData>::iterator obj = g_tableData.begin();

	for(int i = 0; i < m_iSelectItem; ++i)
	{
		obj++;
	}
	
	g_tableData.erase(obj);

	// ������ 1���� �⺻���� �־���Ѵ�.
	if(g_tableData.empty())
		AddLastItem();

	UpdateList();
}

void CStringTableDlg::OnBnClickedIdchange()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CIDChangeDlg dlgID;
	int nID = dlgID.DoModal();
}
