#include "Nave2D.h"
#include <Nave/NFSync.h>
#include "UIDialogManager.h"

using namespace std;	// std::list 라고 안쓰고 list 라고만 선언해도 된다. 
						// 안쓰면 반드시 std::list라고 사용해야한다.

namespace Nave2D {

	/// 메시지 큐
	std::list<HMSG>					s_MsgQue;
	/// 오브젝트 스케일 값 기본 1.0
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
	
			// 다음 메시지
			++Msg;
			++Count;
			// 사용된 메시지는 앞에서 꺼낸다.
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
		// 이부분을 좀더 스마트 하게 할수 없나..
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