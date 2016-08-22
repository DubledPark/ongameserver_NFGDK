/** 
 *  @file		UIDialog.h
 *  @brief		다이얼로그 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UIObject.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UIStatic.h"
#include "UIScrollBar.h"
#include "UIListBox.h"
#include "UIEditBox.h"
#include "UIComboBox.h"
#include <vector>

namespace Nave2D {

	/** 
	 *  @class        UIDialog
	 *  @brief        다이얼로그 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIDialog : public UIObject
	{
	public:
		/// UIDialog 생성자
		UIDialog(void);
		/// UIDialog 생성자
		UIDialog(const Nave::String& name);
		/// UIDialog 소멸자
		virtual ~UIDialog(void);

	public:
		/// 다이얼로그를 생성합니다.
		virtual BOOL OnCreate(UIObject* pParent)		{ OnSetParent(pParent); OnMove(m_fX, m_fY); return TRUE; }
		/// 다이얼로그를 삭제합니다.
		virtual BOOL OnDestroy()						{ return TRUE; }

		/// WM_SCALE
		virtual VOID OnScale() { return; }
		/// WM_COMMAND
		virtual VOID OnCommand(int iID, int iEvent, LPARAM lParam) { return; }
		/// WM_MOUSEMOVE
		virtual BOOL OnMouseMove(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONDBLCLK
		virtual BOOL OnLButtonDClick(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_LBUTTONDOWN
		virtual BOOL OnLButtonDown(int iMouseX, int iMouseY) { return InRect(iMouseX, iMouseY); }
		/// WM_LBUTTONUP
		virtual BOOL OnLButtonUp(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_RBUTTONDOWN
		virtual BOOL OnRButtonDown(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_RBUTTONUP
		virtual BOOL OnRButtonUp(int iMouseX, int iMouseY) { return FALSE; }
		/// WM_MOUSEWHEEL
		virtual BOOL OnMouseWheel(int iMouseX, int iMouseY, int zDelta) { return FALSE; }

		/// Message Proc
		virtual LRESULT MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// 오브젝트 고유 스트링
	    virtual const char* ToString() { return "UIDialog"; }

		/// 오브젝트를 Process처리한다.
		virtual VOID	OnUpdate(float dt = 0.0f);
		/// 오브젝트를 화면에 그린다.
		virtual VOID	OnRender();

		/// 다이얼로그에 스킨을 설정합니다.
		virtual VOID	SetSkin(LPSPRITEINFO Skin);
		/// 다이얼로그의 텍스쳐를 로드합니다.
		virtual VOID	SetTexture(const Nave::String& textureName);
		/// 텍스쳐에서 화면에 렌더링할 영역을 지정합니다.
		virtual VOID	SetShowArea(float fX, float fY, float fWidth, float fHeight);

		/// 다이얼로그안에 마우스가 들어왔으면 포커스를 지정합니다.
		virtual BOOL	OnSetFocus(int iMouseX, int iMouseY);

		/// 다이얼로그를 이동합니다.
		virtual VOID	OnMove(float fX, float fY);
		/// 다이얼로그를 Show 합니다.
		virtual VOID	OnShow();
		/// 다이얼로그를 Hide 합니다.
		virtual VOID	OnHide();

		/// Modal 형식으로 다이얼로그를 지정합니다.
		VOID			SetModalFlag(BOOL Mode);
		/// 현재 다이얼로그가 Modal 형식인지 얻어옵니다.
		inline BOOL		IsModal() const { return m_bModalMode; }

		/// 다이얼로그의 고유 이름을 얻어옵니다.
		inline const Nave::String& GetName() const;
		/// 다이얼로그의 고유 이름을 설정합니다.
		inline VOID		SetName(Nave::String& name);

		/// 다이얼로그의 투명도를 얻어옵니다.
		inline int		GetOpacity()					{ return (int)m_Sprite.GetOpacity(); }
		/// 다이얼로그의 투명도를 설정합니다.
		inline VOID		SetOpacity( int iOpacity )		{ m_Sprite.SetOpacity((BYTE)iOpacity); }
		/// 다이얼로그의 Blend 모드를 설정합니다.
		virtual VOID	SetBlendMode(UISprite::BLEND BlendMode);

		/// Index에 해당하는 컨트롤을 리턴합니다.
	    UIControl*		GetControl( int iIndex );
		/// pt에 위치한 컨트롤을 리턴합니다.
	    UIControl*		GetControlAtPoint( CPoint pt );

		/// Index에 해당하는 컨트롤의 활성화 상태를 얻어업니다.
		BOOL			GetControlVisible( int iIndex );
		/// Index에 해당하는 컨트롤을 활성화 상태를 처리합니다.
	    VOID			SetControlVisible( int iIndex, BOOL bVisible );

		/// 버튼을 추가합니다.
		UIButton*		AddButton( DWORD nID, float fX, float fY, float fWidth, float fHeight); 
		/// 체크박스를 추가합니다.
		UICheckBox*		AddCheckBox( DWORD nID, float fX, float fY, float fWidth, float fHeight);
		/// Static를 추가합니다.
		UIStatic*		AddStatic( DWORD nID, float fX, float fY, float fWidth, float fHeight);
		/// 스크롤바를 추가합니다.
		UIScrollBar*	AddScrollBar( DWORD nID, float fX, float fY, float fWidth, float fHeight);
		/// 리스트박스를 추가합니다.
		UIListBox*		AddListBox( DWORD nID, float fX, float fY, float fWidth, float fHeight);
		/// 에디트박스를 추가합니다.
		UIEditBox*		AddEditBox( DWORD nID, float fX, float fY, float fWidth, float fHeight, DWORD Style = 0);
		/// 콤보박스를 추가합니다.
		UIComboBox*		AddComboBox( DWORD nID, float fX, float fY, float fWidth, float fHeight);

		/// 모든 컨트롤을 초기화합니다.
		VOID			ClearControl();

	private:
		/// Modal 유무
		BOOL					m_bModalMode;

		/// 마우스 이전위치 Drag할때 사용
		CPoint					m_PrevPoint;
		/// LButton Click 변수
		BOOL					m_bLButtonClick;

		/// 컨트롤 저장변수
		std::vector<UIControl*>	m_vecControl;

		/// 다이얼로그 고유이름
		Nave::String			m_strName;

		/// 다이얼로그 스프라이트 구조체
		SPRITEINFO				m_Skin;
		/// 다이얼로그 스프라이트 객체
		UISprite				m_Sprite;
	};

}