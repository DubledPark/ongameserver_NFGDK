// StringTable.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CStringTableApp:
// �� Ŭ������ ������ ���ؼ��� StringTable.cpp�� �����Ͻʽÿ�.
//

class CStringTableApp : public CWinApp
{
public:
	CStringTableApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CStringTableApp theApp;