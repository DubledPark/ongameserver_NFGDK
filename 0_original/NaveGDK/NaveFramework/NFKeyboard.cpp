#include "NFKeyboard.h"

namespace NaveFramework {

	using namespace Input;

	NFKeyboard::NFKeyboard(void)
	{
		m_hWnd = NULL;
		m_pKeyboard = NULL;

		m_bExclusive = TRUE;
		m_bForeground = TRUE;
		m_bImmediate = TRUE;	// Buffer ����� ����ϸ� ���̰ɸ��� Ű�����ε����� �����Ⱚ�� ó���ɼ� �ִ�. Buffer����� Down ó���� �Ѵٰ� �Ҷ� ����ϱ� ����.
		m_bDisableWindowsKey = TRUE;

		ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
	}

	NFKeyboard::~NFKeyboard(void)
	{
		Release();
	}

	VOID NFKeyboard::Release()
	{
		// Unacquire the device one last time just in case 
		// the app tried to exit while the device is still acquired.
		if( m_pKeyboard ) 
			m_pKeyboard->Unacquire();

		// Release any DirectInput objects.
		_RELEASE( m_pKeyboard );
	}

	BOOL NFKeyboard::Create(HWND hWnd, LPDIRECTINPUT8 pDI, BOOL bExclusive, BOOL bForeground, BOOL bImmediate, BOOL bDisableWindowsKey)
	{
		if(pDI == NULL)
			return FALSE;

		m_hWnd = hWnd;
		m_bExclusive = bExclusive;
		m_bForeground = bForeground;
		m_bImmediate = bImmediate;
		m_bDisableWindowsKey = bDisableWindowsKey;

		HRESULT hr;
		DWORD   dwCoopFlags;

		if( m_bExclusive )
			dwCoopFlags = DISCL_EXCLUSIVE;
		else
			dwCoopFlags = DISCL_NONEXCLUSIVE;

		if( m_bForeground )
			dwCoopFlags |= DISCL_FOREGROUND;
		else
			dwCoopFlags |= DISCL_BACKGROUND;

		// Disabling the windows key is only allowed only if we are in foreground nonexclusive
		if( m_bDisableWindowsKey && !m_bExclusive && m_bForeground )
			dwCoopFlags |= DISCL_NOWINKEY;

		// Ű���� ����
		// Obtain an interface to the system keyboard device.
		if( FAILED( hr = pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
			return FALSE;


		// Set the data format to "keyboard format" - a predefined data format 
		//
		// A data format specifies which controls on a device we
		// are interested in, and how they should be reported.
		//
		// This tells DirectInput that we will be passing an array
		// of 256 bytes to IDirectInputDevice::GetDeviceState.
		if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
			return FALSE;

		// Set the cooperativity level to let DirectInput know how
		// this device should interact with the system and with other
		// DirectInput applications.
		hr = m_pKeyboard->SetCooperativeLevel( m_hWnd, dwCoopFlags );
		if( hr == DIERR_UNSUPPORTED && !m_bForeground && m_bExclusive )
		{
			Release();
			return FALSE;
		}

		if( FAILED(hr) )
			return FALSE;

		if( !m_bImmediate )
		{
			// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
			//
			// DirectInput uses unbuffered I/O (buffer size = 0) by default.
			// If you want to read buffered data, you need to set a nonzero
			// buffer size.
			//
			// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
			//
			// The buffer size is a DWORD property associated with the device.
			DIPROPDWORD dipdw;

			dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
			dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			dipdw.diph.dwObj        = 0;
			dipdw.diph.dwHow        = DIPH_DEVICE;
			dipdw.dwData            = SAMPLE_BUFFER_SIZE; // Arbitary buffer size

			if( FAILED( hr = m_pKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
				return FALSE;
		}

		SetActive(WA_ACTIVE);

		return TRUE;
	}

	HRESULT NFKeyboard::SetActive(DWORD nState)
	{
		if(WA_INACTIVE != nState)
		{
			m_bActive = TRUE;
			return m_pKeyboard->Acquire();
		}

		m_bActive = FALSE;
		// Acquire the newly created device
		return S_FALSE;
	}

	VOID NFKeyboard::Update()
	{
		if(!m_bActive)
			return;

		if( m_bImmediate )
		{
			// �� ����� Input���� ���ͼ� ó���ϴ� ������� Press ��ĵ��� 
			if( FAILED( ReadImmediateData() ) )
				return;
		}
		else
		{
			// ���۹������ ���۰� �ʱ�ȭ �ȴ�.
			if( FAILED( ReadBufferedData() ) )
				return;
		}

		UpdateKeyMap();
	}

	HRESULT NFKeyboard::ReadImmediateData()
	{
		HRESULT hr;
		int     i;

		BYTE	diks[MAX_KEY];

		if( NULL == m_pKeyboard ) 
			return S_OK;

		// Get the input's device state, and put the state in dims
		ZeroMemory( diks, sizeof(diks) );
		hr = m_pKeyboard->GetDeviceState( sizeof(diks), diks );
		if( FAILED(hr) ) 
		{
			hr = m_pKeyboard->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_pKeyboard->Acquire();

			if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) 
				m_bActive = FALSE;

			return S_OK; 
		}
	    
		//   0     1     2    3
		// None, Down, Press, Up

		// �ٿ�Ű�� ������ Ȯ���Ѵ�.
		for( i = 0; i < MAX_KEY; ++i ) 
		{
			if( diks[i] & 0x80 )	// Press
			{
				if(m_bKeyState[i] == EKS_NONE)	// 0�̸� Down
					m_bKeyState[i] = EKS_DOWN;
				else					// 0�� �ƴϸ� Press
					m_bKeyState[i] = EKS_PRESS;
			}
			else if(m_bKeyState[i] == EKS_UP)
				m_bKeyState[i] = EKS_NONE;	// None
			else if(m_bKeyState[i] != EKS_NONE)
				m_bKeyState[i] = EKS_UP;	// Up
		}

		return S_OK;
	}


	//-----------------------------------------------------------------------------
	// Name: ReadBufferedData()
	// Desc: Read the input device's state when in buffered mode and display it.
	//-----------------------------------------------------------------------------
	HRESULT NFKeyboard::ReadBufferedData()
	{
		DIDEVICEOBJECTDATA didod[ SAMPLE_BUFFER_SIZE ];  // Receives buffered data 
		DWORD              dwElements;
		DWORD              i;
		int				   iKey;
		HRESULT            hr;

		if( NULL == m_pKeyboard ) 
			return S_OK;

		dwElements = SAMPLE_BUFFER_SIZE;
		hr = m_pKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
										 didod, &dwElements, 0 );
		if( hr != DI_OK ) 
		{
			hr = m_pKeyboard->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_pKeyboard->Acquire();

			// Update the dialog text 
			if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) 
				m_bActive = FALSE;

			return S_OK; 
		}

		// Press ó��
		//   0     1     2    3
		// None, Down, Press, Up
		for( i = 0; i < MAX_KEY; ++i ) 
		{
			if(m_bKeyState[i] == EKS_DOWN)
				m_bKeyState[i] = EKS_PRESS;
			else if(m_bKeyState[i] == EKS_UP)
				m_bKeyState[i] = EKS_NONE;
		}

		// Study each of the buffer elements and process them.
		//
		// Since we really don't do anything, our "processing"
		// consists merely of squirting the name into our
		// local buffer.
		for( i = 0; i < dwElements; ++i ) 
		{
			iKey = didod[i].dwOfs;

			if(didod[ i ].dwData & 0x80)
				m_bKeyState[iKey] = EKS_DOWN;
			else
				m_bKeyState[iKey] = EKS_UP;
		}

		return S_OK;
	}

	VOID NFKeyboard::UpdateKeyMap()
	{
		if(g_KeyList.empty())
			return;

		int iSize = g_KeyList.size();
		
		for(int i = 0; i < iSize; ++i)
		{
			// Map�� ���ǵ� Key���� ���°� ���ǵ� State�� ������ �켱 1���հ�.
			if(m_bKeyState[g_KeyList[i].iKey] == g_KeyList[i].iState)
			{
				// ModKey�� ���ǵǾ��ִ��� Ȯ��.
				// ���Ű�� ���ǵǾ����� �ʰų� ModKey�� Press �����̸� Key�� ��������� ��� ��������
				if(g_KeyList[i].iModKey == KEY_NONE || m_bKeyState[g_KeyList[i].iModKey] == EKS_PRESS)
				{
					if(g_KeyList[i].iFlag == 1)
						continue;

					// Key �� ���ȴٰ� �̺�Ʈ�� �����ش�.
					PostMessage(m_hWnd, WM_INPUTEVENT, (WPARAM)g_KeyList[i].iEvent, (LPARAM)g_KeyList[i].fParam);

					g_KeyList[i].iFlag = 1;
				}				
				else
					g_KeyList[i].iFlag = 0;
			}
			else 
				g_KeyList[i].iFlag = 0;
		}
	}

}