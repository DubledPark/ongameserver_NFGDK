/** 
 *  @file		UIEditBox.h
 *  @brief		에디트 박스 컨트롤
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIControl.h"
#include "NFText.h"
#include "UICaret.h"
#include <vector>

namespace Nave2D { 

	/// EditBox에서 사용하는 Notify 메시지 Define
	#define WM_EDIT_NOTIFY		WM_USER + 0x00ed

	/// MultiLine EditBox에서 사용할 EditItem 구조체
	typedef struct EDITITEM
	{
		Nave::String	strText;
		NFText			dcText;

		~EDITITEM()
		{
			dcText.OnDestroy();
		}
	}*LPEDITITEM;

	/** 
	 *  @class        UIEditBox
	 *  @brief        에디트박스를 생성하고 관리하는 컨트롤
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIEditBox : public UIControl
	{
	public:
		/// UIEditBox 생성자
		UIEditBox(void);
		/// UIEditBox 소멸자
		virtual ~UIEditBox(void);

	public:
		/**
		 * @brief	에디트박스를 생성합니다.
		 * @param pParent	부모포인트
		 * @param fX		로컬X
		 * @param fY		로컬Y
		 * @param fWidth	가로크기
		 * @param fHeight	세로크기
		 * @param nID		객체 아이디
		 * @warning	fX,fY의 값은 부모의 시작점에서의 로컬좌표이다.
		 * @return	성공여부
		 */		
		virtual BOOL OnCreate(UIObject* pParent, float fX, float fY, float fWidth, float fHeight, DWORD nID, DWORD Style = 0);
		/// 에디트박스를 삭제한다.
		virtual BOOL OnDestroy();

		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY);
		/// WM_LBUTTONDCLICK
		virtual BOOL OnLButtonDClick(int iMouseX, int iMouseY);

		/// Edit Widnows Message Proc
		virtual LRESULT MsgProcEx( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// 포커스를 얻었다.
		virtual VOID SetFocus();
		/// 포커스를 잃었다.
		virtual VOID KillFocus();

		/// 오브젝트 고유 스트링
	    virtual const char* ToString() { return "UIEditBox"; }

		/// Editbox를 스프라이트에 렌더링 합니다.
		virtual VOID OnPaint();

		/// 화면에 렌더링을 합니다.
		virtual VOID OnRender();

		/// Move Objec 처리
		virtual VOID OnMoveObject(float fX, float fY, float fWidth = 0, float fHeight = 0);

		/**
		 * @brief		에디트박스 사이즈 설정
		 * @param fWidth	에디트박스 가로 크기
		 * @param fHeight   에디트박스 세로 크기(콤보리스트제외)
		 */
		virtual VOID SetSize(float fWidth, float fHeight);

		/// 에디트박스 기본스킨 설정
		virtual VOID SetSkin(LPSPRITEINFO Skin);

		/// EditBox의 모든 객체를 삭제합니다.
		VOID ResetContent();

		/// 텍스트 컬러를 설정합니다.
		VOID SetTextColor(COLORREF TextColor);
		/// 캐럿의 컬러를 설정합니다.
		VOID SetCaretColor(COLORREF CaretColor);

		/// 폰트를 설정합니다.
		VOID SetFont(HFONT hFont);
		/// 패스워드 모드로 설정합니다.
		VOID SetPasswordMode(BOOL bPassMode = FALSE);

		/// 스트링을 설정합니다. (멀티라인을 포함한다.)
		VOID SetText(const WCHAR* buff);
		/// 모든 스트링을 얻는다. (멀티라인을 포함한다.)
		const Nave::String GetText();
		/// 스트링 길이를 얻어옵니다.
		int GetTextLen();

		/// Editbox의 ReadOnly 상태얻기
		BOOL GetReadOnly() { return m_bReadOnly; };
		/// EditBox를 ReadOnly로 설정합니다.
		VOID SetReadOnly(BOOL Mode) { m_bReadOnly = Mode; }

		/// 에디트 박스의 입력 최대글자수를 설정합니다.
		VOID SetLimitText(int Max);

		/// 에디트 박스 IME 핸들 얻기
		inline HWND GetIME() { return m_hIME; }

	public:	
		/// 에디트박스 리스트 (서브클래싱으로 Editbox윈도우 메시지 처리시 필요)
		static std::map<DWORD, UIEditBox*>	m_EditObject;
		/// 에디트 박스를 추가한다.
		static VOID RegisterEdit(UIEditBox* pThis);
		/// 에디트 박스를 제거한다.
		static VOID UnRegisterEdit(UIEditBox* pThis);
		/// 에디트 박스 서버클래싱 프로시져
		static LRESULT CALLBACK WndEditProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		/// 스트링을 체크합니다.
		BOOL CheckString();
		/// 새로운 라인아이템을 추가합니다. VK_ENTER 처리
		VOID AddLine();
		/// 마지막 라인아이템을 삭제합니다. VK_DEL 처리
		VOID DelLine();

		/// 현재 입력창의 문자열을 설정한다.
		VOID SetString(const WCHAR* buff);
		/// 현재 입력창의 문자열을 가져온다.
		int GetString(WCHAR* buff);
		/// 현재 입력창의 문자열 크기를 얻어온다.
		int GetStringLen();

	private:
		/// 에디트박스 IME Windows
		HWND		m_hIME;
		/// 에디트박스 폰트
		HFONT		m_hFont;

		/// 에디트박스 스타일
		DWORD		m_dwStyle;
		/// 에디트박스 멀티라인에서 한라인의 간격
		float		m_fItemHeight;

		/// 에디트박스에서 입력되고 있는 문자열
		NFText		m_Text;

		/// 멀티라인 에디트박스에서 입력이 완료된 한라인
		std::vector<LPEDITITEM>	m_vecItem;

		/// 에디트박스의 캐럿
		UICaret		m_Caret;
		/// 캐럿 크기
		CSize		m_CaretSize;

		/// IME COMPOSITION 상태
		BOOL		m_bIMEComp;
		
		/// 에디트 박스 포커스상태
		BOOL		m_bFocus;

		/// 읽기전용 상태표시
		BOOL		m_bReadOnly;
		/// 패스워드 모드 상태표시
		BOOL		m_bPasswordMode;

		/// 최상위가 몇번재 라인인지 (스크롤바가 적용될경우 사용)
		int			m_iTopLine;

		/// 텍스트 컬러
		COLORREF	m_TextColor;

		/// 에디트박스 서브클래싱용 프로시저객체
		WNDPROC		m_OldWndProc;		
	};

}