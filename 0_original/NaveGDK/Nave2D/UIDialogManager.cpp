#include "Nave2D.h"
#include "UIDialogManager.h"

namespace Nave2D { 

	UIDialogManager::UIDialogManager(void)
	{
	}

	UIDialogManager::~UIDialogManager(void)
	{
		DelectDialogAll();
	}

	VOID UIDialogManager::DelectDialogAll()
	{
		Nave::NFSyncLock Sync(&m_Lock);

		ClearShowDialog();

		iterDialog obj = m_mapDialog.begin();
		iterDialog end = m_mapDialog.end();

		UIDialog* pDlg;

		while(obj != end)
		{
			pDlg = obj->second;

			_DELETE(pDlg);
		
			++obj;
		}

		m_mapDialog.clear();
	}

	VOID UIDialogManager::DeleteDialog(const Nave::String& name)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		iterDialog obj = m_mapDialog.find(name);

		if(obj == m_mapDialog.end())
			return;

		UIDialog* pDlg = obj->second;
		_DELETE(pDlg);

		m_mapDialog.erase(obj);
	}

	UIDialog* UIDialogManager::CreateUIDialog(const Nave::String& name, BOOL modal)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		UIDialog* pDlg;

		pDlg = new UIDialog(name);
		pDlg->SetModalFlag(modal);

		m_mapDialog[name] = pDlg;

		return pDlg;
	}

	UIDialog* UIDialogManager::GetDialog( const Nave::String& name )
	{
		Nave::NFSyncLock Sync(&m_Lock);

		iterDialog obj = m_mapDialog.find(name);

		if(obj == m_mapDialog.end())
			return NULL;

		UIDialog* pDlg = obj->second;
		return pDlg;
	}

	BOOL UIDialogManager::PushShowDialog( const Nave::String& name )
	{
		UIDialog* pDlg = GetDialog(name);

		if(pDlg == NULL)
			return FALSE;

		m_ShowDialog.push_back(pDlg);
		return TRUE;
	}

	BOOL UIDialogManager::PushShowDialog( UIDialog* dialog )
	{
		if(dialog == NULL)
			return FALSE;

		m_ShowDialog.push_back(dialog);
		return TRUE;
	}

	BOOL UIDialogManager::PopShowDialog( const Nave::String& name )
	{
		UIDialog* pDlg = GetDialog(name);

		iterShowDialog obj = m_ShowDialog.begin();
		iterShowDialog end = m_ShowDialog.end();

		while(obj != end)
		{
			if( (*obj)->Equals( *pDlg ) )
			{
				m_ShowDialog.erase(obj);
				return TRUE;	
			}
			++obj;
		}

		return FALSE;
	}

	BOOL UIDialogManager::PopShowDialog( UIDialog* dialog )
	{
		iterShowDialog obj = m_ShowDialog.begin();
		iterShowDialog end = m_ShowDialog.end();

		while(obj != end)
		{
			if( (*obj)->Equals( *dialog ) )
			{
				m_ShowDialog.erase(obj);
				return TRUE;	
			}
			++obj;
		}

		return FALSE;
	}


	VOID UIDialogManager::ShowDialogAlwaysOnTop(UIDialog* dialog)
	{
		iterShowDialog obj = m_ShowDialog.begin();
		iterShowDialog end = m_ShowDialog.end();

		while(obj != end)
		{
			if( (*obj)->Equals( *dialog ) )
			{
				m_ShowDialog.erase(obj);
				break;	
			}
			++obj;
		}

		m_ShowDialog.push_back(dialog);
	}

	VOID UIDialogManager::Update(float fDt)
	{
		if(m_ShowDialog.empty())
			return;

		iterShowDialog obj = m_ShowDialog.begin();
		iterShowDialog end = m_ShowDialog.end();

		UIDialog* pDlg;

		while(obj != end)
		{
			pDlg = (*obj);
			pDlg->OnUpdate(fDt);
			++obj;
		}
	}

	VOID UIDialogManager::Render()
	{
		if(m_ShowDialog.empty())
			return;

		iterShowDialog obj = m_ShowDialog.begin();
		iterShowDialog end = m_ShowDialog.end();

		UIDialog* pDlg;

		while(obj != end)
		{
			pDlg = (*obj);
			pDlg->OnRender();
			++obj;
		}
	}

	BOOL UIDialogManager::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(m_ShowDialog.empty())
			return FALSE;

		iterShowDialog obj = m_ShowDialog.end();
		iterShowDialog end = m_ShowDialog.begin();

		UIDialog* pDlg;

		do		
		{
			--obj;

			pDlg = (*obj);

			if(pDlg->MsgProc(uMsg, wParam, lParam))
				return TRUE;

		}while(obj != end);

		return FALSE;
	}

}