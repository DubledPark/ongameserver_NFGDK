/*
 *  @file		Nave2D.h
 *  @brief		UIFramework 관련 함수
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <NaveFramework/NaveFramework.h>
#include "UIObject.h"
#include "UIControl.h"
#include "UIDialog.h"
#include <list>

namespace Nave2D {

	/// Object Scale 디파인
	#define WM_SCALE			WM_USER + 0x0001
	/// Object Event 디파인
	#define WM_OBJECTEVENT		WM_USER + 0x0002

	/////////////////////////////////////////////////////////////
	// 전역 함수
	/////////////////////////////////////////////////////////////
	/// 오브젝트 이벤트
	enum Event{
		TYPE_NONE,		/// 초기화값
		TYPE_OVER,		/// 마우스가 오버했을때
		TYPE_LDOWN,		/// 마우스 L버튼이 Down
		TYPE_LUP,		/// 마우스 L버튼이 Up
		TYPE_LDBCLICK,	/// 마우스 L버튼이 더블클릭 됐을때
		TYPE_RDOWN,		/// 마우스 R버튼이 Down
		TYPE_RUP,		/// 마우스 R버튼이 Up
		EDIT_RETURN,	/// Edit박스 엔터
		EDIT_TAB,		/// Edit박스 탭
		EDIT_CHAR,		/// Edit박스 글자입력
		EDIT_ESC,		/// Edit박스 Esc 입력
		COMBO_CHANG,	/// 콤보박스의 셀렉트 값이 바꼈을때 발생
		SET_FOCUS,		/// 오브젝트가 포커스를 얻었을때
		KILL_FOCUS,		/// 오브젝트가 포커스를 일었을때
	};

	/// 오브젝트 메시지 구조체
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

	/// 큐를 처리한다.
	VOID UpdateMsg();

	/// 오브젝트에 메시지를 전달한다.
	BOOL SendMessageEx(UIObject* hObj, UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// 메시지 큐에 메시지를 추가합니다.
	VOID PostMessageEx(UIObject* hObj, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// 오브젝트의 부모를 얻어옵니다.
	UIObject* GetParent(UIObject* hObj);
	/// 오브젝트의 로컬 좌표를 얻어옵니다.
	CPoint	GetLocalPos(UIObject* hObj);

	/// UIDialog 를 최상위로 이동합니다.
	VOID DialogAlwaysOnTop(UIDialog* ObjPtr);

	/// 컨트롤 객체의 메시지 프로시져
	BOOL MessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// 컨트롤 객체의 렌더링
	VOID Render();
	/// 컨트롤 객체의 업데이트.
	VOID Update(float fDt);

	/// UI의 기본 스케일갓을 가져옵니다.
	float GetUIScale();
	/// UI의 스케일값을 지정합니다.
	VOID  SetUIScale(float fScale);
	/// Value값의 스케일 적용값을 리턴합니다.
	float GetUIScale(float fValue);

}