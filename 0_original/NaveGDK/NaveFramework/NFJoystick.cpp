#include "NFJoystick.h"

namespace NaveFramework {

	using namespace Input;

	struct DI_ENUM_CONTEXT
	{
		DIJOYCONFIG* pPreferredJoyCfg;
		BOOL bPreferredJoyCfgValid;
		LPDIRECTINPUT8 pDI;
		LPDIRECTINPUTDEVICE8 pJoy;
	};

	struct DI_ENUM_CONTEXT2
	{
		LPDIRECTINPUTDEVICE8 pJoy;
	};

	//-----------------------------------------------------------------------------
	// Name: EnumJoysticksCallback()
	// Desc: Called once for each enumerated joystick. If we find one, create a
	//       device interface on it so we can play with it.
	//-----------------------------------------------------------------------------
	BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
	{
		DI_ENUM_CONTEXT* pEnumContext = (DI_ENUM_CONTEXT*) pContext;
		HRESULT hr;

		// Skip anything other than the perferred joystick device as defined by the control panel.  
		// Instead you could store all the enumerated joysticks and let the user pick.
		if( pEnumContext->bPreferredJoyCfgValid &&
			!IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
			return DIENUM_CONTINUE;

		// Obtain an interface to the enumerated joystick.
		hr = pEnumContext->pDI->CreateDevice( pdidInstance->guidInstance, &pEnumContext->pJoy, NULL );

		// If it failed, then we can't use this joystick. (Maybe the user unplugged
		// it while we were in the middle of enumerating it.)
		if( FAILED(hr) ) 
			return DIENUM_CONTINUE;

		// Stop enumeration. Note: we're just taking the first joystick we get. You
		// could store all the enumerated joysticks and let the user pick.
		return DIENUM_STOP;
	}


	//-----------------------------------------------------------------------------
	// Name: EnumObjectsCallback()
	// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
	//       joystick. This function enables user interface elements for objects
	//       that are found to exist, and scales axes min/max values.
	//-----------------------------------------------------------------------------
	BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
	{
		DI_ENUM_CONTEXT2* pEnumContext = (DI_ENUM_CONTEXT2*) pContext;

		// For axes that are returned, set the DIPROP_RANGE property for the
		// enumerated axis in order to scale min/max values.
		if( pdidoi->dwType & DIDFT_AXIS )
		{
			DIPROPRANGE diprg; 
			diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
			diprg.diph.dwHow        = DIPH_BYID; 
			diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
			diprg.lMin              = -1000; 
			diprg.lMax              = +1000; 
	    
			// Set the range for the axis
			if( FAILED( pEnumContext->pJoy->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
				return DIENUM_STOP;
	         
		}
/*
		static int nSliderCount = 0;  // Number of returned slider controls
		static int nPOVCount = 0;     // Number of returned POV controls

		// Set the UI to reflect what objects the joystick supports
		if (pdidoi->guidType == GUID_XAxis)
		{
			EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS_TEXT ), TRUE );
		}
		if (pdidoi->guidType == GUID_YAxis)
		{
			EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS_TEXT ), TRUE );
		}
		if (pdidoi->guidType == GUID_ZAxis)
		{
			EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS_TEXT ), TRUE );
		}
		if (pdidoi->guidType == GUID_RxAxis)
		{
			EnableWindow( GetDlgItem( hDlg, IDC_X_ROT ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_X_ROT_TEXT ), TRUE );
		}
		if (pdidoi->guidType == GUID_RyAxis)
		{
			EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT_TEXT ), TRUE );
		}
		if (pdidoi->guidType == GUID_RzAxis)
		{
			EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT_TEXT ), TRUE );
		}
		if (pdidoi->guidType == GUID_Slider)
		{
			switch( nSliderCount++ )
			{
				case 0 :
					EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0 ), TRUE );
					EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0_TEXT ), TRUE );
					break;

				case 1 :
					EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1 ), TRUE );
					EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1_TEXT ), TRUE );
					break;
			}
		}
		if (pdidoi->guidType == GUID_POV)
		{
			switch( nPOVCount++ )
			{
				case 0 :
					EnableWindow( GetDlgItem( hDlg, IDC_POV0 ), TRUE );
					EnableWindow( GetDlgItem( hDlg, IDC_POV0_TEXT ), TRUE );
					break;

				case 1 :
					EnableWindow( GetDlgItem( hDlg, IDC_POV1 ), TRUE );
					EnableWindow( GetDlgItem( hDlg, IDC_POV1_TEXT ), TRUE );
					break;

				case 2 :
					EnableWindow( GetDlgItem( hDlg, IDC_POV2 ), TRUE );
					EnableWindow( GetDlgItem( hDlg, IDC_POV2_TEXT ), TRUE );
					break;

				case 3 :
					EnableWindow( GetDlgItem( hDlg, IDC_POV3 ), TRUE );
					EnableWindow( GetDlgItem( hDlg, IDC_POV3_TEXT ), TRUE );
					break;
			}
		}
*/
		return DIENUM_CONTINUE;
	}

	NFJoystick::NFJoystick(void)
	{
		m_hWnd = NULL;
		m_pJoy = NULL;

		ZeroMemory( &m_JoyState, sizeof(m_JoyState) );
		ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
	}

	NFJoystick::~NFJoystick(void)
	{
		Release();
	}

	VOID NFJoystick::Release()
	{
		// Unacquire the device one last time just in case 
		// the app tried to exit while the device is still acquired.
		if( m_pJoy ) 
			m_pJoy->Unacquire();

		// Release any DirectInput objects.
		_RELEASE( m_pJoy );
	}

	BOOL NFJoystick::Create(HWND hWnd, LPDIRECTINPUT8 pDI)
	{
		if(pDI == NULL)
			return FALSE;

		m_hWnd = hWnd;

		DIJOYCONFIG PreferredJoyCfg = {0};
		DI_ENUM_CONTEXT enumContext;
		enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
		enumContext.bPreferredJoyCfgValid = FALSE;
		enumContext.pDI = pDI;
		enumContext.pJoy = NULL;

		IDirectInputJoyConfig8* pJoyConfig = NULL;  
		if( FAILED(pDI->QueryInterface( IID_IDirectInputJoyConfig8, (void **) &pJoyConfig ) ) )
			return FALSE;

		PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
		if( SUCCEEDED( pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached
			enumContext.bPreferredJoyCfgValid = TRUE;

		_RELEASE( pJoyConfig );

		// Look for a simple joystick we can use for this sample program.
		if( FAILED( pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
									 EnumJoysticksCallback,
									 &enumContext, DIEDFL_ATTACHEDONLY ) ) )
			return FALSE;


		m_pJoy = enumContext.pJoy;

		// Make sure we got a joystick
		if( NULL == m_pJoy )
			return FALSE;

		// Set the data format to "simple joystick" - a predefined data format 
		//
		// A data format specifies which controls on a device we are interested in,
		// and how they should be reported. This tells DInput that we will be
		// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
		if( FAILED( m_pJoy->SetDataFormat( &c_dfDIJoystick2 ) ) )
			return FALSE;

		// Set the cooperative level to let DInput know how this device should
		// interact with the system and with other DInput applications.
		if( FAILED( m_pJoy->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
			return FALSE;

		DI_ENUM_CONTEXT2 enumContext2;
		enumContext2.pJoy = m_pJoy;
		// Enumerate the joystick objects. The callback function enabled user
		// interface elements for objects that are found, and sets the min/max
		// values property for discovered axes.
		if( FAILED( m_pJoy->EnumObjects( EnumObjectsCallback, 
										&enumContext2, DIDFT_ALL ) ) )
			return FALSE;


		ZeroMemory( &m_JoyState, sizeof(m_JoyState) );

		SetActive(WA_ACTIVE);
		return TRUE;
	}

	HRESULT NFJoystick::SetActive(DWORD nState)
	{
		if(WA_INACTIVE != nState)
		{
			m_bActive = TRUE;
			return m_pJoy->Acquire();
		}

		m_bActive = FALSE;
		// Acquire the newly created device
		return S_FALSE;
	}

	VOID NFJoystick::Update()
	{
		if(!m_bActive)
			return;

		if( NULL == m_pJoy ) 
			return;

		HRESULT     hr;
		ZeroMemory( &m_JoyState, sizeof(m_JoyState) );

		// Poll the device to read the current state
		hr = m_pJoy->Poll(); 
		if( FAILED(hr) )  
		{
			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.
			hr = m_pJoy->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_pJoy->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
			return; 
		}

		// Get the input's device state
		if( FAILED( hr = m_pJoy->GetDeviceState( sizeof(DIJOYSTATE2), &m_JoyState ) ) )
			return; // The device should have been acquired during the Poll()

		// 다운키의 정보를 확인한다.
		for( int i = 0; i < 15; ++i ) 
		{
			if( m_JoyState.rgbButtons[i] & 0x80 )	// Press
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

		UpdateKeyMap();
	}

	VOID NFJoystick::UpdateKeyMap()
	{
		if(g_KeyList.empty())
			return;

		int iSize = g_KeyList.size();

		for(int i = 0; i < iSize; ++i)
		{
			// 키보드보다작으면 리턴
			if(g_KeyList[i].iKey < PAD_NONE || g_KeyList[i].iKey >= KEYMAP_END)
				continue;

			switch(g_KeyList[i].iKey)
			{
			case PAD_BUTTON01:
			case PAD_BUTTON02:
			case PAD_BUTTON03:
			case PAD_BUTTON04:
			case PAD_BUTTON05:
			case PAD_BUTTON06:
			case PAD_BUTTON07:
			case PAD_BUTTON08:
			case PAD_BUTTON09:
			case PAD_BUTTON10:
			case PAD_BUTTON11:
			case PAD_BUTTON12:
			case PAD_BUTTON13:
			case PAD_BUTTON14:
			case PAD_BUTTON15:
				{
					int iKey = g_KeyList[i].iKey-PAD_BUTTON01;
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