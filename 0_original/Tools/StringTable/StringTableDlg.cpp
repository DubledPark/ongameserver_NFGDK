// StringTableDlg.cpp : 구현 파일
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

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CStringTableDlg 대화 상자




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
	// QuickList전용
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


// CStringTableDlg 메시지 처리기

BOOL CStringTableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	g_strID = L"ID_NONE";


	////////////////////////////////////////////////////////////////////////
	// QuickList 초기화 및 설정.
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


	// 키보드를 이용해 Select Cursor을 Move시키기 위한 함수로FALSE 이면 Keydown으로 커서를 이동하지 못한다.
	m_List.EnableColumnNavigation(TRUE);

	// 0번 칼럼을 FineKey로 사용한다.
	m_List.SetKeyfindColumn(0);

	m_List.SetEditOnDblclk(TRUE);
	m_List.SetEditOnEnter(TRUE);
	m_List.SetEditOnF2(TRUE);

	// 이값이 False 가 되어있어야 Keyfind를 사용할수 있다.
	m_List.SetEditOnWriting(FALSE);

//	m_List.EnableToolTips(TRUE);

	AddLastItem();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CStringTableDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CStringTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStringTableDlg::OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CStringTableDlg::OnLvnBeginlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(pDispInfo->item.pszText != NULL)
	{
		if(pDispInfo->item.iSubItem == 1)
		{
			int iPos = FindID(pDispInfo->item.iItem, pDispInfo->item.pszText);
			if(iPos != 0)
			{
				WCHAR str[512];
				_stprintf(str, L"같은 ID가 %d번에 존재합니다.", iPos);
				MessageBox(str, L"중복 아이디 검사");
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
					// 마지막에서 Enter를 누르면 새로운 아이템을 하단에 추가한다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TRACE(L"LVN_ODFINDITEM send to parent.\n");

	*pResult = 0;
}

void CStringTableDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int item=-1, subitem=-1;

	if(m_List.HitTest(pNMListView->ptAction, item, subitem))
	{
		m_List.SetHotItem(item);
	}

	*pResult = 0;
}

// Quick List용 메시지 
// Item을 선택했을때의 처리루틴.
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

// Quick List용 메시지 
// KeyDown을 이용해 Select Cursor을 이동시키기 위한 값으로 
// test->m_allowChange 가 false 이면 마우스로 더이상 이동을 못한다.
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

// Quick List용 메시지 
// Header에서 RClick 를 했을대 처리 TrackPopup을 Show할때 사용
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

// Quick List용 메시지 
// 오너드로우를 이용해 ListItem을 출력하는 부분이다.
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

	// 호출은 subItem을 선택했을때도 한다.
#ifndef QUICKLIST_NOEDIT
	if(subItem > 0)		// Edit시키기 위해서..
	{
		data->m_allowEdit = true;
//		if(m_fullrow || subItem == 0)
	}
#endif

	// 사용자 컬러일경우
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

	// 여기가 화면에 실제 정보를 뿌려주는 부분
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}

void CStringTableDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnOK();
}

void CStringTableDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	iGlobalIndex = 0;
	g_tableData.clear();

	AddLastItem();

	UpdateList();
}

void CStringTableDlg::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	ofn.lpstrFilter = L"String Table File(*.stb)\0*.stb\0CSV (쉼표로분리) (*.csv)\0*.csv\0All Files(*.*)\0*.*\0\0";
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
		// 초기화.
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

			// 마지막 1개는 개행라인이므로 이걸로 개행라인을 삭제해준다.
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

	// 오픈합니다..
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
	// 초기화.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	ofn.lpstrFilter = L"String Table File(*.stb)\0*.stb\0CSV (쉼표로분리) (*.csv)\0*.csv\0All Files(*.*)\0*.*\0\0";
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

	// 오픈합니다..
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(m_iSelectItem == -1)
		return;

	std::vector<CTableData>::iterator obj = g_tableData.begin();

	for(int i = 0; i < m_iSelectItem; ++i)
	{
		obj++;
	}
	
	g_tableData.erase(obj);

	// 무조건 1개는 기본으로 있어야한다.
	if(g_tableData.empty())
		AddLastItem();

	UpdateList();
}

void CStringTableDlg::OnBnClickedIdchange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIDChangeDlg dlgID;
	int nID = dlgID.DoModal();
}
