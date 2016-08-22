/*
 *  @file		Nave2D.h
 *  @brief		UIFramework ���� �Լ�
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NaveFramework.h>
#include "UIObject.h"
#include "UIControl.h"
#include "UIDialog.h"
#include <list>

namespace Nave2D {

	/// Object Scale ������
	#define WM_SCALE			WM_USER + 0x0001
	/// Object Event ������
	#define WM_OBJECTEVENT		WM_USER + 0x0002

	/////////////////////////////////////////////////////////////
	// ���� �Լ�
	/////////////////////////////////////////////////////////////
	/// ������Ʈ �̺�Ʈ
	enum Event{
		TYPE_NONE,		/// �ʱ�ȭ��
		TYPE_OVER,		/// ���콺�� ����������
		TYPE_LDOWN,		/// ���콺 L��ư�� Down
		TYPE_LUP,		/// ���콺 L��ư�� Up
		TYPE_LDBCLICK,	/// ���콺 L��ư�� ����Ŭ�� ������
		TYPE_RDOWN,		/// ���콺 R��ư�� Down
		TYPE_RUP,		/// ���콺 R��ư�� Up
		EDIT_RETURN,	/// Edit�ڽ� ����
		EDIT_TAB,		/// Edit�ڽ� ��
		EDIT_CHAR,		/// Edit�ڽ� �����Է�
		EDIT_ESC,		/// Edit�ڽ� Esc �Է�
		COMBO_CHANG,	/// �޺��ڽ��� ����Ʈ ���� �ٲ����� �߻�
		SET_FOCUS,		/// ������Ʈ�� ��Ŀ���� �������
		KILL_FOCUS,		/// ������Ʈ�� ��Ŀ���� �Ͼ�����
	};

	/// ������Ʈ �޽��� ����ü
	typedef struct HMSG
	{
		UIObject* 	hObj;
		UINT		uMsg;
		WPARAM      wParam;
		LPARAM      lParam;

		~HMSG()
		{
			hObj = NULL;
			uMsg = 0;
			wParam = 0;
			lParam = 0;
		}
	}*PHMSG;

	/// ť�� ó���Ѵ�.
	VOID UpdateMsg();

	/// ������Ʈ�� �޽����� �����Ѵ�.
	BOOL SendMessageEx(UIObject* hObj, UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// �޽��� ť�� �޽����� �߰��մϴ�.
	VOID PostMessageEx(UIObject* hObj, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// ������Ʈ�� �θ� ���ɴϴ�.
	UIObject* GetParent(UIObject* hObj);
	/// ������Ʈ�� ���� ��ǥ�� ���ɴϴ�.
	CPoint	GetLocalPos(UIObject* hObj);

	/// UIDialog �� �ֻ����� �̵��մϴ�.
	VOID DialogAlwaysOnTop(UIDialog* ObjPtr);

	/// ��Ʈ�� ��ü�� �޽��� ���ν���
	BOOL MessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// ��Ʈ�� ��ü�� ������
	VOID Render();
	/// ��Ʈ�� ��ü�� ������Ʈ.
	VOID Update(float fDt);

	/// UI�� �⺻ �����ϰ��� �����ɴϴ�.
	float GetUIScale();
	/// UI�� �����ϰ��� �����մϴ�.
	VOID  SetUIScale(float fScale);
	/// Value���� ������ ���밪�� �����մϴ�.
	float GetUIScale(float fValue);

}