/** 
 *  @file		UIDialogManager.h
 *  @brief		UIDialog�� �����ϰ� �����ϴ� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
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
	 *  @brief        UIDialog�� ����, �����ϰ� ���������ִ� �޴��� Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-06
	 */
	class UIDialogManager : public Nave::NFSingleton<UIDialogManager>
	{
	public:
		/// UIDialogManager ������
		UIDialogManager(void);
		/// UIDialogManager �Ҹ���
		~UIDialogManager(void);

	public:
		/// ��� ���̾�α׸� �����մϴ�.
		VOID DelectDialogAll();
		/// ���̾�α׸� �����մϴ�.
		VOID DeleteDialog(const Nave::String& name);

		/// ���ο� ���̾�α׸� �����մϴ�.
		UIDialog* CreateUIDialog(const Nave::String& name, BOOL modal = FALSE);
		/// ���̾�α׸� �����ɴϴ�.
		UIDialog* GetDialog( const Nave::String& name );

		/// ������ ť�� ���̾�α׸� �߰��մϴ�.
		BOOL PushShowDialog( const Nave::String& name );
		/// ������ ť�� ���̾�α׸� �߰��մϴ�.
		BOOL PushShowDialog( UIDialog* dialog );

		/// ������ ť���� ���̾�α׸� �����մϴ�.
		BOOL PopShowDialog( const Nave::String& name );
		/// ������ ť���� ���̾�α׸� �����մϴ�.
		BOOL PopShowDialog( UIDialog* dialog );

		/// ������ ť���� �ش���ġ�� ���̾�α׸� �����մϴ�.
		UIDialog* PeekShowDialog( const int iIndex )
		{
			if((int)m_ShowDialog.size() <= iIndex)
				return NULL;

			iterShowDialog obj = m_ShowDialog.begin();
			for(int i = 0; i < iIndex; ++i)
				++obj;

			return (UIDialog*)(*obj);
		}

		/// ������ ť���� �ش��̸��� ���̾�α׸� �����մϴ�.
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

		/// �ش� ���̾�α׸� �ֻ����� �̵��մϴ�.
		VOID ShowDialogAlwaysOnTop(UIDialog* dialog);
		/// ���̾�α׸���Ʈ�� ������Ʈ �մϴ�.
		VOID Update(float fDt = 0.0f);
		/// ���̾�α׸� ������ �մϴ�.
		VOID Render();
		/// ���̾�α��� �޽����� ó���մϴ�.
		BOOL MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		/// ������ ť�� �ʱ�ȭ�մϴ�.
		inline VOID ClearShowDialog() { m_ShowDialog.clear(); }
		/// ������ ť�� ������ �����մϴ�.
		inline int GetNumOfShowDialog() const { return (int)m_ShowDialog.size(); }

	private:
		/// ��ü ���̾�α��� ����Ʈ
		std::map<Nave::String, UIDialog*>	m_mapDialog;	
		/// ��ü ���̾�α��� ���ͷ�����
		typedef std::map<Nave::String, UIDialog*>::iterator iterDialog;
		
		/// ���̾�α��� ������ ť
		std::list<UIDialog*>				m_ShowDialog;
		/// ������ ť�� ���ͷ�����
		typedef std::list<UIDialog*>::iterator	iterShowDialog;

		/// Sync ��ü
		Nave::NFSync						m_Lock;
	};

}