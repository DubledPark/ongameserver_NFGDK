/** 
 *  @file		UIDialogManager.h
 *  @brief		UIDialog를 생성하고 관리하는 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <Nave/NFSingleton.h>
#include <Nave/NFSync.h>
#include "UIDialog.h"
#include "UIControl.h"
#include <list>
#include <map>

namespace Nave2D { 

	/** 
	 *  @class        UIDialogManager
	 *  @brief        UIDialog를 생성, 관리하고 렌더링해주는 메니져 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIDialogManager : public Nave::NFSingleton<UIDialogManager>
	{
	public:
		/// UIDialogManager 생성자
		UIDialogManager(void);
		/// UIDialogManager 소멸자
		~UIDialogManager(void);

	public:
		/// 모든 다이얼로그를 삭제합니다.
		VOID DelectDialogAll();
		/// 다이얼로그를 삭제합니다.
		VOID DeleteDialog(const Nave::String& name);

		/// 새로운 다이얼로그를 생성합니다.
		UIDialog* CreateUIDialog(const Nave::String& name, BOOL modal = FALSE);
		/// 다이얼로그를 가져옵니다.
		UIDialog* GetDialog( const Nave::String& name );

		/// 렌더링 큐에 다이얼로그를 추가합니다.
		BOOL PushShowDialog( const Nave::String& name );
		/// 렌더링 큐에 다이얼로그를 추가합니다.
		BOOL PushShowDialog( UIDialog* dialog );

		/// 렌더링 큐에서 다이얼로그를 제거합니다.
		BOOL PopShowDialog( const Nave::String& name );
		/// 렌더링 큐에서 다이얼로그를 제거합니다.
		BOOL PopShowDialog( UIDialog* dialog );

		/// 렌더링 큐에서 해당위치의 다이얼로그를 리턴합니다.
		UIDialog* PeekShowDialog( const int iIndex )
		{
			if((int)m_ShowDialog.size() <= iIndex)
				return NULL;

			iterShowDialog obj = m_ShowDialog.begin();
			for(int i = 0; i < iIndex; ++i)
				++obj;

			return (UIDialog*)(*obj);
		}

		/// 렌더링 큐에서 해당이름의 다이얼로그를 리턴합니다.
		UIDialog* PeekShowDialog( const Nave::String& name )
		{
			if( m_ShowDialog.empty() )
				return NULL;

			iterShowDialog obj = m_ShowDialog.begin();
			iterShowDialog end = m_ShowDialog.end();

			UIDialog* pDlg;
			while(obj != end)
			{
				pDlg = (*obj);

				if(pDlg->GetName() == name)
					return pDlg;

				++obj;
			}
			return NULL;
		}

		/// 해당 다이얼로그를 최상위로 이동합니다.
		VOID ShowDialogAlwaysOnTop(UIDialog* dialog);
		/// 다이얼로그리스트를 업데이트 합니다.
		VOID Update(float fDt = 0.0f);
		/// 다이얼로그를 렌더링 합니다.
		VOID Render();
		/// 다이얼로그의 메시지를 처리합니다.
		BOOL MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		/// 렌더링 큐를 초기화합니다.
		inline VOID ClearShowDialog() { m_ShowDialog.clear(); }
		/// 렌더링 큐의 개수를 리턴합니다.
		inline int GetNumOfShowDialog() const { return (int)m_ShowDialog.size(); }

	private:
		/// 전체 다이얼로그의 리스트
		std::map<Nave::String, UIDialog*>	m_mapDialog;	
		/// 전체 다이얼로그의 이터레이터
		typedef std::map<Nave::String, UIDialog*>::iterator iterDialog;
		
		/// 다이얼로그의 렌더링 큐
		std::list<UIDialog*>				m_ShowDialog;
		/// 렌더링 큐의 이터레이터
		typedef std::list<UIDialog*>::iterator	iterShowDialog;

		/// Sync 객체
		Nave::NFSync						m_Lock;
	};

}