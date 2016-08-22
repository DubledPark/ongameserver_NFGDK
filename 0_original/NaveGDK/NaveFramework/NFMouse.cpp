#include "NFMouse.h"

namespace NaveFramework {

	using namespace Input;

	NFMouse::NFMouse(void)
	{
		m_hWnd = NULL;
		m_pMouse = NULL;

		m_bExclusive = TRUE;
		m_bForeground = TRUE;
		m_bImmediate = TRUE;	// Buffer 방식을 사용하면 렉이걸릴때 키보드인덱스가 쓰레기값이 처리될수 있다. Buffer방식은 Down 처리만 한다고 할때 사용하기 좋음.
		ZeroMemory( &m_MouseState, sizeof(m_MouseState) );
		ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
		SetMousePos(0,0,0);
	}

	NFMouse::~NFMouse(void)
	{
		Release();
	}

	VOID NFMouse::Release()
	{
		// Unacquire the device one last time just in case 
		// the app tried to exit while the device is still acquired.
		if( m_pMouse ) 
			m_pMouse->Unacquire();

		// Release any DirectInput objects.
		_RELEASE( m_pMouse );
	}

	BOOL NFMouse::Create(HWND hWnd, LPDIRECTINPUT8 pDI, BOOL bExclusive, BOOL bForeground)
	{
		if(pDI == NULL)
			return FALSE;

		m_hWnd = hWnd;
		m_bExclusive = bExclusive;
		m_bForeground = bForeground;
		m_bImmediate = TRUE;

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

		// 마우스 설정
		// Obtain an interface to the system mouse device.
		if( FAILED( hr = pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL ) ) )
			return FALSE;


		// Set the data format to "mouse format" - a predefined data format 
		//
		// A data format specifies which controls on a device we
		// are interested in, and how they should be reported.
		//
		// This tells DirectInput that we will be passing a
		// DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
		if( FAILED( hr = m_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
			return FALSE;

		// Set the cooperativity level to let DirectInput know how
		// this device should interact with the system and with other
		// DirectInput applications.
		hr = m_pMouse->SetCooperativeLevel( m_hWnd, dwCoopFlags );
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

			if( FAILED( hr = m_pMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
				return FALSE;

			POINT Point;
			GetCursorPos(&Point);
			SetMousePos(Point.x,Point.y,0);
		}
		else
			SetMousePos(0,0,0);

		ZeroMemory( &m_MouseState, sizeof(m_MouseState) );

		SetActive(WA_ACTIVE);

		return TRUE;
	}

	VOID NFMouse::SetMousePos(LONG lX, LONG lY, LONG lZ)
	{
		m_lMouseX = lX;
		m_lMouseY = lY;
		m_lMouseZ = lZ;
	}

	HRESULT NFMouse::SetActive(DWORD nState)
	{
		if(WA_INACTIVE != nState)
		{
			m_bActive = TRUE;
			return m_pMouse->Acquire();
		}

		m_bActive = FALSE;
		// Acquire the newly created device
		return S_FALSE;
	}

	VOID NFMouse::Update()
	{
		if(!m_bActive)
			return;

		if( m_bImmediate )
		{
			// 이 방식이 Input에서 얻어와서 처리하는 방식으로 Press 방식등을 
			if( FAILED( ReadImmediateData() ) )
				return;
		}
		else
		{
			// 버퍼방식으로 버퍼가 초기화 된다.
			if( FAILED( ReadBufferedData() ) )
				return;
		}

		m_lMouseX += GetStateX();
		m_lMouseY += GetStateY();
		m_lMouseZ += GetStateZ();

		UpdateKeyMap();
	}

	//-----------------------------------------------------------------------------
	// Name: ReadImmediateData()
	// Desc: Read the input device's state when in immediate mode and display it.
	//-----------------------------------------------------------------------------
	HRESULT NFMouse::ReadImmediateData()
	{
		HRESULT       hr;

		if( NULL == m_pMouse ) 
			return S_OK;

		// Get the input's device state, and put the state in dims
		ZeroMemory( &m_MouseState, sizeof(m_MouseState) );
		hr = m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_MouseState );
		if( FAILED(hr) ) 
		{
			// DirectInput may be telling us that the input stream has been
			// interrupted.  We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done.
			// We just re-acquire and try again.

			// If input is lost then acquire and keep trying 
			hr = m_pMouse->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_pMouse->Acquire();

			// Update the dialog text 
			if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) 
				m_bActive = FALSE;

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
			return S_OK; 
		}

		//   0     1     2    3
		// None, Down, Press, Up

		// 다운키의 정보를 확인한다.
		for( int i = 0; i < 8; ++i ) 
		{
			if( m_MouseState.rgbButtons[i] & 0x80 )	// Press
			{
				if(m_bKeyState[i] == EKS_NONE)	// 0이면 Down
					m_bKeyState[i] = EKS_DOWN;
				else					// 0이 아니면 Press
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
	HRESULT NFMouse::ReadBufferedData()
	{
		DIDEVICEOBJECTDATA didod[ SAMPLE_BUFFER_SIZE ];  // Receives buffered data 
		DWORD              dwElements;
		DWORD              i;
		int				   iKey;
		HRESULT            hr;

		if( NULL == m_pMouse ) 
			return S_OK;

		dwElements = SAMPLE_BUFFER_SIZE;
		ZeroMemory( &m_MouseState, sizeof(m_MouseState) );
		hr = m_pMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
		if( hr != DI_OK )
		{
			hr = m_pMouse->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_pMouse->Acquire();

			// Update the dialog text 
			if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) 
				m_bActive = FALSE;

			return S_OK; 
		}

		for( i = 0; i < dwElements; i++ ) 
		{
			switch( didod[ i ].dwOfs )
			{
			case DIMOFS_BUTTON0:
			case DIMOFS_BUTTON1:
			case DIMOFS_BUTTON2:
			case DIMOFS_BUTTON3:
			case DIMOFS_BUTTON4:
			case DIMOFS_BUTTON5:
			case DIMOFS_BUTTON6:
			case DIMOFS_BUTTON7:
				m_MouseState.rgbButtons[didod[ i ].dwOfs-DIMOFS_BUTTON0] = (BYTE)didod[ i ].dwData;
				break;

			case DIMOFS_X:
				m_MouseState.lX += didod[ i ].dwData;
				break;
			case DIMOFS_Y:
				m_MouseState.lY += didod[ i ].dwData;
				break;
			case DIMOFS_Z:
				m_MouseState.lZ += didod[ i ].dwData;
				break;
			}
		}

		// Press 처리
		//   0     1     2    3
		// None, Down, Press, Up
		for( i = 0; i < 8; ++i ) 
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
		for( i = 0; i < 8; ++i ) 
		{
			iKey = DIMOFS_BUTTON0+i;

			if(m_MouseState.rgbButtons[i] & 0x80)
				m_bKeyState[iKey] = EKS_DOWN;
			else
				m_bKeyState[iKey] = EKS_UP;
		}

		return S_OK;
	}

	VOID NFMouse::UpdateKeyMap()
	{
		if(g_KeyList.empty())
			return;

		int iSize = g_KeyList.size();

		for(int i = 0; i < iSize; ++i)
		{
			// 키보드보다작으면 리턴
			if(g_KeyList[i].iKey < MOUSE_NONE || g_KeyList[i].iKey >= PAD_NONE)
				continue;

			switch(g_KeyList[i].iKey)
			{
			case MOUSE_BUTTON00:
			case MOUSE_BUTTON01:
			case MOUSE_BUTTON02:
			case MOUSE_BUTTON03:
			case MOUSE_BUTTON04:
			case MOUSE_BUTTON05:
			case MOUSE_BUTTON06:
			case MOUSE_BUTTON07:
				{
					int iKey = g_KeyList[i].iKey-MOUSE_BUTTON00;
					if(m_bKeyState[iKey] == g_KeyList[i].iState)
					{
						if(g_KeyList[i].iFlag == 1)
							continue;

						// Key 가 눌렸다고 이벤트를 날려준다.
						PostMessage(m_hWnd, WM_INPUTEVENT, (WPARAM)g_KeyList[i].iEvent, (LPARAM)g_KeyList[i].fParam);

						g_KeyList[i].iFlag = 1;
					}
					else
						g_KeyList[i].iFlag = 0;
				}
				break;
			}
		}
	}


}