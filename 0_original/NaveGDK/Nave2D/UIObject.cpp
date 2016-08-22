#include "Nave2D.h"
#include "UIObject.h"

namespace Nave2D {

	UIObject::UIObject(void)
	{
		m_hParent	= NULL;		// 부모 윈도우 핸들

		m_dID		= -1;
		m_bVisible	= FALSE;

		m_fX		= 0;
		m_fY		= 0;	// 현재 위치
		m_fWidth	= 0;
		m_fHeight	= 0;	// 현재 크기	
	}

	UIObject::~UIObject(void)
	{
		m_bVisible = FALSE;
		OnDestroy();

		m_hParent	= NULL;		// 부모 윈도우 핸들
		m_dID		= -1;
	}

	LRESULT UIObject::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		POINT Point = Nave::GetMousePos();
		int X = Point.x;
		int Y = Point.y;

		switch(uMsg)
		{
		case WM_COMMAND:
			OnCommand(LOWORD(wParam), HIWORD(wParam), lParam);
			break;
		case WM_LBUTTONUP:
			return OnLButtonUp(X, Y);
		case WM_LBUTTONDOWN:
			return OnLButtonDown(X, Y);
		case WM_LBUTTONDBLCLK:
			return OnLButtonDClick(X, Y);
		case WM_MOUSEMOVE:
			return OnMouseMove(X, Y);
		case WM_RBUTTONUP:
			return OnRButtonUp(X, Y);
		case WM_RBUTTONDOWN:
			return OnRButtonDown(X, Y);
		case WM_MOUSEWHEEL:
			return OnMouseWheel(X, Y, GET_WHEEL_DELTA(wParam));
		}
		return FALSE;
	}

}