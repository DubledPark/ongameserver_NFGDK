#include "Nave2D.h"
#include <Nave/NFSync.h>
#include "UIDialogManager.h"

using namespace std;	// std::list ��� �Ⱦ��� list ��� �����ص� �ȴ�. 
						// �Ⱦ��� �ݵ�� std::list��� ����ؾ��Ѵ�.

namespace Nave2D {

	/// �޽��� ť
	std::list<HMSG>					s_MsgQue;
	/// ������Ʈ ������ �� �⺻ 1.0
	float							s_fUIScale = 1.0f;

	VOID UpdateMsg()
	{
		if(s_MsgQue.empty())
			return;

		int Count = 0;
		int MaxCount = (int)s_MsgQue.size();
		list<HMSG>::iterator Msg = s_MsgQue.begin();

		do
		{
			HMSG& hMsg = (*Msg);

			if(hMsg.hObj)
				hMsg.hObj->MsgProc(hMsg.uMsg, hMsg.wParam, hMsg.lParam);
	
			// ���� �޽���
			++Msg;
			++Count;
			// ���� �޽����� �տ��� ������.
			s_MsgQue.pop_front();
		}while(Count != MaxCount);
	}

	BOOL SendMessageEx(UIObject* hObj, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(hObj == NULL)  
		{
			::SendMessage(Nave::hMainWnd, uMsg, wParam, lParam);
			return FALSE;
		}

		hObj->MsgProc(uMsg, wParam, lParam);
		return TRUE;
	}

	VOID PostMessageEx(UIObject* hObj, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// �̺κ��� ���� ����Ʈ �ϰ� �Ҽ� ����..
		if(hObj == NULL)
		{
			::PostMessage(Nave::hMainWnd, uMsg, wParam, lParam);
			return;
		}
		HMSG qMsg;
		qMsg.hObj = hObj;
		qMsg.uMsg = uMsg;
		qMsg.wParam = wParam;
		qMsg.lParam = lParam;

		s_MsgQue.push_back(qMsg);
	}

	UIObject* GetParent(UIObject* hObj)
	{
		if(hObj == NULL)  
			return NULL;

		return hObj->GetParent();
	}

	CPoint GetLocalPos(UIObject* hObj)
	{
		CPoint Point(0, 0);
		if(hObj == NULL)
			return Point;

		Point.x = (int)hObj->GetX();
		Point.y = (int)hObj->GetY();
		if(hObj->GetParent())
		{
			Point.x += (int)hObj->GetParent()->GetX();
			Point.y += (int)hObj->GetParent()->GetY();
		}
		return Point;
	}

	BOOL MessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(!UIDialogManager::IsAlive())
			return FALSE;

		return UIDialogManager::GetInstance().MsgProc(uMsg, wParam, lParam);
	}
	
	VOID Render()
	{
		UIDialogManager::GetInstance().Render();
	}

	VOID Update(float fDt)
	{
		UIDialogManager::GetInstance().Update(fDt);
	}

	VOID DialogAlwaysOnTop(UIDialog* ObjPtr)
	{
		UIDialogManager::GetInstance().ShowDialogAlwaysOnTop(ObjPtr);
	}

	float GetUIScale()
	{
		return s_fUIScale;
	}

	VOID SetUIScale(float fScale)
	{
		s_fUIScale = fScale;
	}

	float GetUIScale(float fValue)
	{
		return fValue*s_fUIScale;
	}

}