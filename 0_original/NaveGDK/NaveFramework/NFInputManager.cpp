#include "NFInputManager.h"

namespace NaveFramework {

	using namespace Input;

	NFInputManager::NFInputManager(void)
	{
		m_hWnd = NULL;
		m_pDI = NULL;

		m_pKeyboard = NULL;
		m_pMouse = NULL;
		m_pJoystick = NULL;
		m_pFFEffect = NULL;
	}

	NFInputManager::~NFInputManager(void)
	{
		ReleaseAll();
	}

	BOOL NFInputManager::Create(HWND hWnd)
	{
		m_hWnd = hWnd;

		HRESULT hr;

		// Create a DInput object
		if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
			return FALSE;

		SetActive(WA_INACTIVE);

		return TRUE;
	}

	VOID NFInputManager::ReleaseAll()
	{
		_DELETE(m_pKeyboard);
		_DELETE(m_pMouse);
		_DELETE(m_pJoystick);
		_DELETE(m_pFFEffect);

		_RELEASE( m_pDI );
	}

	BOOL NFInputManager::CreateKeyboard(BOOL bExclusive, BOOL bForeground, BOOL bImmediate, BOOL bDisableWindowsKey)
	{
		_DELETE(m_pKeyboard);

		m_pKeyboard = new NFKeyboard;

		if(!m_pKeyboard)
			return FALSE;

		return m_pKeyboard->Create(m_hWnd, m_pDI, bExclusive, bForeground, bImmediate, bDisableWindowsKey);		
	}

	BOOL NFInputManager::CreateMouse(BOOL bExclusive, BOOL bForeground)
	{
		_DELETE(m_pMouse);

		m_pMouse = new NFMouse;

		if(!m_pMouse)
			return FALSE;

		return m_pMouse->Create(m_hWnd, m_pDI, bExclusive, bForeground);		
	}

	BOOL NFInputManager::CreateJoystick()
	{
		_DELETE(m_pJoystick);

		m_pJoystick = new NFJoystick;

		if(!m_pJoystick)
			return FALSE;

		BOOL bResult = m_pJoystick->Create(m_hWnd, m_pDI);

		// 초기화 실패시 Pad 초기화
		if(bResult == FALSE)
			_DELETE(m_pJoystick);

		return bResult;
	}

	BOOL NFInputManager::CreateFFDevice()
	{
		_DELETE(m_pFFEffect);

		m_pFFEffect = new NFForceFeedback;

		if(!m_pFFEffect)
			return FALSE;

		return m_pFFEffect->Create(m_hWnd, m_pDI);		
	}	

	HRESULT NFInputManager::SetActive(DWORD nState)
	{
		if(WA_INACTIVE != nState)
		{
			m_bActive = TRUE;

			if(m_pKeyboard)
				m_pKeyboard->SetActive(nState);

			if(m_pMouse)
				m_pMouse->SetActive(nState);

			if(m_pJoystick)
				m_pJoystick->SetActive(nState);

			if(m_pFFEffect)
				m_pFFEffect->SetActive(nState);

			return S_OK;
		}
		m_bActive = FALSE;
		return S_FALSE;
	}

	VOID NFInputManager::Update()
	{
		if(!m_bActive)
			return;

		if(m_pKeyboard)
			m_pKeyboard->Update();

		if(m_pMouse)
			m_pMouse->Update();

		if(m_pJoystick)
			m_pJoystick->Update();

		if(m_pFFEffect)
			m_pFFEffect->Update();
	}

	VOID NFInputManager::ResetKeyMap()
	{
		g_KeyList.clear();
	}

	VOID NFInputManager::AddKeyMap(Nave::String szCmd, int iKey, int iModKey, int iState, int iEvent, float fParam)
	{
		INPUTMAP stKey;

		stKey.szCmd = szCmd;			// Key name String..
		stKey.iKey = iKey;				// Map에서 사용될 Key
		stKey.iModKey = iModKey;		// Key와 함께 사용될 확장Key
		stKey.iState = iState;			// iKey에서 사용될 State 상태 (Down, Up, Press)
		stKey.iEvent = iEvent;			// 이 이벤트가 발생했을때 전달될 Event ID	(wParam)
		stKey.fParam = fParam;			// EventID와 함께 전달될 Param 값.			(lParam)

		g_KeyList.push_back(stKey);
	}

	VOID NFInputManager::AddKeyMapList(INPUTMAP* pList, int iSize)
	{
		for(int i = 0; i < iSize; ++i)
		{
			g_KeyList.push_back(pList[i]);
		}
	}

	void NFInputManager::SaveInputMap(const WCHAR* szFileName)
	{
		Nave::NFSmartPtr<Nave::IXMLStreamWriter> pXml = Nave::CreateXmlStreamWriter(szFileName);
		pXml->Write(L"KeyMap", g_KeyList);

/*
		int iKeySize = g_KeyList.size();

		FILE* pf;
		char Buff[1024];

		pf = fopen(szFileName, "w+");

		if (pf==NULL) 
			return;

		sprintf(Buff, "<KEY> %-30s %-15s %-15s %-15s %-15s %-15s\n"
			, "<Event Name>"
			, "<Key>"
			, "<Mod Key>"
			, "<Key State>"
			, "<Event ID>"
			, "<Param>"
			);

		fputs(Buff, pf);


		for (int i = 0; i < iKeySize; ++i)
		{
			sprintf(Buff, "<KEY> %-30s,%-15s,%-15s,%-15s,%-15d,%-15f\n"
				, g_KeyList[i].szCmd
				, GetKeyName(g_KeyList[i].iKey)
				, GetKeyName(g_KeyList[i].iModKey)
				, GetKeyStateName(g_KeyList[i].iState)
				, g_KeyList[i].iEvent
				, g_KeyList[i].fParam
				);

			fputs(Buff, pf);
		}

		fclose(pf);
*/
	}

	void NFInputManager::LoadInputMap(const WCHAR* szFileName)
	{
		g_KeyList.clear();

		Nave::NFSmartPtr<Nave::IXMLStreamReader> pXml = Nave::CreateXmlStreamReader(szFileName);
		pXml->Read(L"KeyMap", g_KeyList);

/*
		FILE* pf;

		INPUTMAP stKey;
		char Buff[1024];

		pf = fopen(szFileName, "r");

		if (pf==NULL) 
			return;

		char* ptoken;
		char  delimit[] = " \n\r,";

		ResetKeyMap();

		BOOL bHeader = FALSE;

		while(!feof(pf))
		{
			ZeroMemory(&stKey, sizeof(stKey));
			fgets(Buff, 1024, pf);

			if(!bHeader)
			{
				bHeader = TRUE;
				continue;
			}

			ptoken = strtok(Buff, delimit);

			if (ptoken==0) 
				continue;

			if (strcmp("<KEY>", ptoken)!=0) continue;

			ptoken = strtok(NULL, delimit);
			if (ptoken==NULL) continue;
			strcpy(stKey.szCmd, ptoken);

			ptoken = strtok(NULL, delimit);
			if (ptoken==NULL) continue;
			stKey.iKey = GetKeyByName(ptoken);

			ptoken = strtok(NULL, delimit);
			if (ptoken==NULL) continue;
			stKey.iModKey = GetKeyByName(ptoken);

			ptoken = strtok(NULL, delimit);
			if (ptoken==NULL) continue;
			stKey.iState = GetKeyByStateName(ptoken);

			ptoken = strtok(NULL, delimit);
			if (ptoken==NULL) continue;
			stKey.iEvent = (int)atof(ptoken);

			ptoken = strtok(NULL, delimit);
			if (ptoken==NULL) continue;
			stKey.fParam = (float)atof(ptoken);

			g_KeyList.push_back(stKey);
		}
		fclose(pf);
*/
	}

}