#include <math.h>
#include "NFForceFeedback.h"

namespace NaveFramework { 

	struct FF_ENUM_CONTEXT
	{
		LPDIRECTINPUT8 pDI;
		LPDIRECTINPUTDEVICE8 pDevice;
	};

	//-----------------------------------------------------------------------------
	// Name: EnumFFDevicesCallback()
	// Desc: Called once for each enumerated force feedback device. If we find
	//       one, create a device interface on it so we can play with it.
	//-----------------------------------------------------------------------------
	BOOL CALLBACK EnumFFDevicesCallback( const DIDEVICEINSTANCE* pInst, 
										 VOID* pContext )
	{
		FF_ENUM_CONTEXT* pEnumContext = (FF_ENUM_CONTEXT*) pContext;

		HRESULT              hr;

		// Obtain an interface to the enumerated force feedback device.
		hr = pEnumContext->pDI->CreateDevice( pInst->guidInstance, &pEnumContext->pDevice, NULL );

		// If it failed, then we can't use this device for some
		// bizarre reason.  (Maybe the user unplugged it while we
		// were in the middle of enumerating it.)  So continue enumerating
		if( FAILED(hr) ) 
			return DIENUM_CONTINUE;

		return DIENUM_STOP;
	}

	//-----------------------------------------------------------------------------
	// Name: EnumAxesCallback()
	// Desc: Callback function for enumerating the axes on a joystick and counting
	//       each force feedback enabled axis
	//-----------------------------------------------------------------------------
	BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
									VOID* pContext )
	{
		DWORD* pdwNumForceFeedbackAxis = (DWORD*) pContext;

		if( (pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0 )
			(*pdwNumForceFeedbackAxis)++;

		return DIENUM_CONTINUE;
	}

	NFForceFeedback::NFForceFeedback(void)
	{
		m_hWnd = NULL;
		m_pDevice = NULL;
		m_pEffect = NULL;
		m_lLifeTime = 0;
		m_dwNumForceFeedbackAxis = 0;
	}

	NFForceFeedback::~NFForceFeedback(void)
	{
		Release();
	}

	VOID NFForceFeedback::Release()
	{
		// Unacquire the device one last time just in case 
		// the app tried to exit while the device is still acquired.
		if( m_pDevice ) 
			m_pDevice->Unacquire();

		// Release any DirectInput objects.
		_RELEASE( m_pEffect );
		_RELEASE( m_pDevice );
	}

	BOOL NFForceFeedback::Create(HWND hWnd, LPDIRECTINPUT8 pDI)
	{
		if(pDI == NULL)
			return FALSE;

		m_hWnd = hWnd;

		DIPROPDWORD dipdw;

		FF_ENUM_CONTEXT enumContext;
		enumContext.pDI = pDI;
		enumContext.pDevice = NULL;

		// Look for a force feedback device we can use
		if( FAILED( pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
									 EnumFFDevicesCallback, &enumContext,
									 DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK ) ) )
		{
			return FALSE;
		}


		m_pDevice = enumContext.pDevice;

		// Make sure we got a joystick
		if( NULL == m_pDevice )
			return FALSE;


		// Set the data format to "simple joystick" - a predefined data format. A
		// data format specifies which controls on a device we are interested in,
		// and how they should be reported.
		//
		// This tells DirectInput that we will be passing a DIJOYSTATE structure to
		// IDirectInputDevice8::GetDeviceState(). Even though we won't actually do
		// it in this sample. But setting the data format is important so that the
		// DIJOFS_* values work properly.
		if( FAILED( m_pDevice->SetDataFormat( &c_dfDIJoystick ) ) )
			return FALSE;

		// Set the cooperative level to let DInput know how this device should
		// interact with the system and with other DInput applications.
		// Exclusive access is required in order to perform force feedback.
		if( FAILED( m_pDevice->SetCooperativeLevel( hWnd,
														 DISCL_EXCLUSIVE | 
														 DISCL_FOREGROUND ) ) )
		{
			return FALSE;
		}

		// Since we will be playing force feedback effects, we should disable the
		// auto-centering spring.
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = FALSE;

		if( FAILED( m_pDevice->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph ) ) )
			return FALSE;

		// Enumerate and count the axes of the joystick 
		if ( FAILED( m_pDevice->EnumObjects( EnumAxesCallback, 
												  (VOID*)&m_dwNumForceFeedbackAxis, DIDFT_AXIS ) ) )
			return FALSE;

		// This simple sample only supports one or two axis joysticks
		if( m_dwNumForceFeedbackAxis > 2 )
			m_dwNumForceFeedbackAxis = 2;

		// This application needs only one effect: Applying raw forces.
		DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
		LONG            rglDirection[2] = { 0, 0 };
		DICONSTANTFORCE cf              = { 0 };

		DIEFFECT eff;
		ZeroMemory( &eff, sizeof(eff) );
		eff.dwSize                  = sizeof(DIEFFECT);
		eff.dwFlags                 = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.dwDuration              = INFINITE;
		eff.dwSamplePeriod          = 0;
		eff.dwGain                  = DI_FFNOMINALMAX;
		eff.dwTriggerButton         = DIEB_NOTRIGGER;
		eff.dwTriggerRepeatInterval = 0;
		eff.cAxes                   = m_dwNumForceFeedbackAxis;
		eff.rgdwAxes                = rgdwAxes;
		eff.rglDirection            = rglDirection;
		eff.lpEnvelope              = 0;
		eff.cbTypeSpecificParams    = sizeof(DICONSTANTFORCE);
		eff.lpvTypeSpecificParams   = &cf;
		eff.dwStartDelay            = 0;

		// Create the prepared effect
		if( FAILED( m_pDevice->CreateEffect( GUID_ConstantForce, 
												  &eff, &m_pEffect, NULL ) ) )
		{
			return FALSE;
		}

		if( NULL == m_pEffect )
			return FALSE;


		SetActive(WA_ACTIVE);
		return TRUE;
	}

	HRESULT NFForceFeedback::SetActive(DWORD nState)
	{
		if( NULL == m_pEffect || m_pDevice == NULL) 
			return S_FALSE;

		if(WA_INACTIVE != nState)
		{
			HRESULT hr = m_pDevice->Acquire();

			if(FAILED(hr))
				return S_FALSE;

			if( m_pEffect ) 
                m_pEffect->Start( 1, 0 ); // Start the effect

			m_bActive = TRUE;
			return hr;
		}

		m_bActive = FALSE;
		return S_FALSE;
	}

	//-----------------------------------------------------------------------------
	// Name: SetDeviceForcesXY()
	// Desc: Apply the X and Y forces to the effect we prepared.
	//-----------------------------------------------------------------------------
	HRESULT NFForceFeedback::SetFF(LONG lX, LONG lY, float fLifeTime)
	{
		m_lLifeTime = (LONG)(fLifeTime*1000);
		m_lLastTime = timeGetTime();

		// Modifying an effect is basically the same as creating a new one, except
		// you need only specify the parameters you are modifying
		LONG rglDirection[2] = { 0, 0 };

		DICONSTANTFORCE cf;

		if( m_dwNumForceFeedbackAxis == 1 )
		{
			// If only one force feedback axis, then apply only one direction and 
			// keep the direction at zero
			cf.lMagnitude = lX;
			rglDirection[0] = 0;
		}
		else
		{
			// If two force feedback axis, then apply magnitude from both directions 
			rglDirection[0] = lX;
			rglDirection[1] = lY;
			cf.lMagnitude = (DWORD)sqrt( (double)lX * (double)lX +
										 (double)lY * (double)lY );
		}

		DIEFFECT eff;
		ZeroMemory( &eff, sizeof(eff) );
		eff.dwSize                = sizeof(DIEFFECT);
		eff.dwFlags               = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.cAxes                 = m_dwNumForceFeedbackAxis;
		eff.rglDirection          = rglDirection;
		eff.lpEnvelope            = 0;
		eff.cbTypeSpecificParams  = sizeof(DICONSTANTFORCE);
		eff.lpvTypeSpecificParams = &cf;
		eff.dwStartDelay            = 0;

		// Now set the new parameters and start the effect immediately.
		return m_pEffect->SetParameters( &eff, DIEP_DIRECTION |
											   DIEP_TYPESPECIFICPARAMS |
											   DIEP_START );
	}

	VOID NFForceFeedback::Update()
	{
		if(!m_bActive)
			return;

		if( NULL == m_pEffect || m_pDevice == NULL) 
			return;

		if(m_lLifeTime == 0)
			return;

		LONG lTick = timeGetTime();
		if(abs(lTick-m_lLastTime) >= m_lLifeTime)
		{
			SetFF(0, 0, 0);			
			return;
		}


		HRESULT hr = m_pDevice->Acquire();
		while( hr == DIERR_INPUTLOST ) 
			hr = m_pDevice->Acquire();
	}

}