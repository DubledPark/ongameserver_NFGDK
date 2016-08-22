#include "NaveFramework.h"
#include "NFVideoDX9.h"

namespace NaveFramework { 

	NFVideoDX9::NFVideoDX9(void)
	{
		m_bWindowed		= TRUE;		// 윈도우 모드이다.
		m_bDeviceLost	= FALSE;	// 디바이스를 이져먹었다.

		m_hWnd			= NULL;		// 윈도우 핸들

		m_pD3D			= NULL;		// The main D3D object
		m_pd3dDevice	= NULL;		// The D3D rendering device
		m_hMenu			= NULL;

		m_pSprite		= NULL;
		m_pLine			= NULL;

		m_bChild		= FALSE;

		m_iWidth		= 640;		// Width used to create window
		m_iHeight		= 480;		// Height used to create window
	}

	NFVideoDX9::~NFVideoDX9(void)
	{
		Release();
	}

	VOID NFVideoDX9::Release()
	{
//		ChangeDisplaySettings(NULL,NULL);
		DelSwapChain(0);

		// 릴리즈 순서 조심.
		_RELEASE(m_pLine);
		_RELEASE(m_pSprite);
		_RELEASE(m_pd3dDevice);
		_RELEASE(m_pD3D);
	}

	HRESULT NFVideoDX9::InitVideo( HWND hWnd, int Width, int Heigth, BOOL WindowMode)
	{
		m_hWnd		= hWnd;
		m_bWindowed = WindowMode;
		m_iWidth	= Width;
		m_iHeight	= Heigth;
		m_hMenu		= GetMenu(m_hWnd);

		m_bChild	= FALSE;

		HRESULT hr;
		// D3D 초기화
		if(FAILED( hr = InitD3D()))
		{
			return hr;
		}

		if( FAILED( hr = InitDeviceObjects() ) )
		{
			Release();
			return hr;
		}

		// 화면 지우기
		ClearScreen(D3DCOLOR_XRGB(0,0,0));
		Present();

		return S_OK;
	}

	HRESULT	NFVideoDX9::InitChildVideo(HWND hWnd, int Width, int Height)
	{
		m_hWnd		= hWnd;
		m_bWindowed = TRUE;
		m_iWidth	= Width;
		m_iHeight	= Height;
		m_hMenu		= GetMenu(m_hWnd);

		m_bChild	= TRUE;

		HRESULT hr;
		// D3D 초기화
		if(FAILED( hr = InitD3D()))
		{
			return hr;
		}

		if( FAILED( hr = InitDeviceObjects() ) )
		{
			Release();
			return hr;
		}

		return S_OK;
	}

	LPSWAPWND NFVideoDX9::AddSwapChain(HWND hWnd, int Width, int Height)
	{
		LPSWAPWND pSw = new SWAPWND;

		m_SwapList[hWnd] = pSw;

		int iCount = m_SwapList.size();

		pSw->hW = hWnd;
		pSw->Width = Width;
		pSw->Height = Height;

		// 스왑체인 생성하기
		D3DPRESENT_PARAMETERS	par;
		memset(&par, 0, sizeof(D3DPRESENT_PARAMETERS));
		
		par.SwapEffect = D3DSWAPEFFECT_DISCARD;
		par.Windowed = TRUE;
		// W, H가 0으로 들어가면 hWnd의 기본값으로 생성된다.
		par.BackBufferWidth = 0;
		par.BackBufferHeight= 0;
		par.hDeviceWindow = pSw->hW;

		if( FAILED(m_pd3dDevice->CreateAdditionalSwapChain(&par, &pSw->pC)) )
		{
			_DELETE(pSw);
			return NULL;		// Create addtional swap chain failed
		}

		if( FAILED(pSw->pC->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSw->pB)) )
		{
			_DELETE(pSw);
			return NULL;		// Get back buffer Failed
		}

		if ( FAILED(m_pd3dDevice->CreateDepthStencilSurface(par.BackBufferWidth
			, par.BackBufferHeight
			, m_d3dpp.AutoDepthStencilFormat
			, D3DMULTISAMPLE_NONE, 0, 0, &pSw->pS, NULL)))
		{
			_DELETE(pSw);
			return NULL;
		}

		return pSw;
	}

	VOID NFVideoDX9::DelSwapChain(HWND hWnd)
	{
		if(hWnd == 0)
		{
			std::map<HWND, LPSWAPWND>::iterator obj = m_SwapList.begin();
			std::map<HWND, LPSWAPWND>::iterator end = m_SwapList.end();

			while(obj != end)
			{
				obj->second->Release();
				_DELETE(obj->second);
				obj->second = NULL;
				++obj;
			}
			m_SwapList.clear();
		}
		else
		{
			if(m_SwapList.find(hWnd) == m_SwapList.end())
				return;

			m_SwapList[hWnd]->Release();
			_DELETE(m_SwapList[hWnd]);
			m_SwapList.erase(hWnd);
		}
	}

	HRESULT NFVideoDX9::ResizeSwapChain(HWND hWnd, int Width, int Height)
	{
		if(m_SwapList.find(hWnd) == m_SwapList.end())
			return S_FALSE;

		LPSWAPWND pSw = m_SwapList[hWnd];
		pSw->Release();

		pSw->Width = Width;
		pSw->Height = Height;

		// 스왑체인 생성하기
		D3DPRESENT_PARAMETERS	par;
		memset(&par, 0, sizeof(D3DPRESENT_PARAMETERS));
		
		par.SwapEffect = D3DSWAPEFFECT_DISCARD;
		par.Windowed = TRUE;
		// W, H가 0으로 들어가면 hWnd의 기본값으로 생성된다.
		par.BackBufferWidth = 0;
		par.BackBufferHeight= 0;
		par.hDeviceWindow = pSw->hW;

		if( FAILED(m_pd3dDevice->CreateAdditionalSwapChain(&par, &pSw->pC)) )
			return S_FALSE;		// Create addtional swap chain failed

		if( FAILED(pSw->pC->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSw->pB)) )
			return S_FALSE;		// Get back buffer Failed

		if ( FAILED(m_pd3dDevice->CreateDepthStencilSurface(par.BackBufferWidth
			, par.BackBufferHeight
			, m_d3dpp.AutoDepthStencilFormat
			, D3DMULTISAMPLE_NONE, 0, 0, &pSw->pS, NULL)))
			return S_FALSE;

		return S_OK;
	}


	HRESULT	NFVideoDX9::ResizeVideo(int Width, int Height)
	{
		m_iWidth	= Width;
		m_iHeight	= Height;

		// D3D 재 초기화
		InitDevice();
		
		HRESULT hr;
		// 아래 함수에서 InitDeviceObjects() 호출한다.
		if( FAILED( hr = ResizeD3D() ) )
		{
			Release();
			return hr;
		}

		return S_OK;
	}

	HRESULT NFVideoDX9::InitD3D()
	{
		// Create the Direct3D object
		m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
		if( m_pD3D == NULL )
		{
			InfoMessage(L"Direct3D 초기화 실패.");
			return E_FAIL;
		};

		// 어댑터의 정보를 얻어온다.
		m_d3dEnumeration.AppUsesDepthBuffer = TRUE;
		m_d3dEnumeration.SetD3D( m_pD3D );
		m_d3dEnumeration.ConfirmDeviceCallback = ConfirmDeviceHelper;
		if( FAILED( m_d3dEnumeration.Enumerate() ) )
		{
			Release();
			InfoMessage(L"그래픽 카드 어댑터의 정보를 얻지 못했습니다.");
			return E_FAIL;
		}

		if( FAILED( InitialD3DSettings() ) )
		{
			Release();
			InfoMessage(L"Direct3D 해상도 정보설정에 실패하였습니다.");
			return E_FAIL;
		}

		D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
		D3DDeviceInfo* pDeviceInfo = m_d3dSettings.PDeviceInfo();

		// D3DAPP Device 설정.
		InitDevice();

		DWORD behaviorFlags;

		int VertexPro = m_d3dSettings.GetVertexProcessingType();
		switch(m_d3dSettings.GetVertexProcessingType())
		{
		case MIXED_VP:
			behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
			break;
		case HARDWARE_VP:
			behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			break;
		case PURE_HARDWARE_VP:
			behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
			break;
		case SOFTWARE_VP:
		default:
			behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			break;
		}

		// Create the D3DDevice
		if( FAILED( m_pD3D->CreateDevice( m_d3dSettings.AdapterOrdinal(), pDeviceInfo->DevType, 
										m_hWnd, behaviorFlags, &m_d3dpp, &m_pd3dDevice ) ) )
		{
			Release();
			InfoMessage(L"Direct3D Device 셋팅에 실패하였습니다.\nAdapter:%d\nDevType:%d\nVirtexProcessingType : %d", m_d3dSettings.AdapterOrdinal(), pDeviceInfo->DevType, VertexPro);
			return E_FAIL;
		}

		// Store device Caps
		m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );

		// Store render target surface desc
		LPDIRECT3DSURFACE9 pBackBuffer = NULL;
		m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();


		// D3D Sprite 객체 생성.
		if( FAILED( D3DXCreateSprite( m_pd3dDevice, &m_pSprite) ) )
		{
			Release();
			InfoMessage(L"D3DSPRITE 생성에 실패하였습니다");
			return E_FAIL;
		}

		// D3D Line 객체 생성.
		if( FAILED( D3DXCreateLine( m_pd3dDevice, &m_pLine) ) )
		{
			Release();
			InfoMessage(L"D3DLINE 생성에 실패하였습니다");
			return E_FAIL;
		}

		SetFocus(m_hWnd);
		return S_OK;
	}

	VOID NFVideoDX9::InitDevice()
	{
		// 디바이스 설정.
		m_d3dpp.Windowed               = m_bWindowed;
		m_d3dpp.BackBufferCount        = 1;
		m_d3dpp.MultiSampleType        = m_d3dSettings.MultisampleType();
		m_d3dpp.MultiSampleQuality     = m_d3dSettings.MultisampleQuality();
		m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.EnableAutoDepthStencil = m_d3dEnumeration.AppUsesDepthBuffer;
		m_d3dpp.hDeviceWindow          = m_hWnd;
		if( m_d3dEnumeration.AppUsesDepthBuffer )
		{
			m_d3dpp.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
			m_d3dpp.AutoDepthStencilFormat = m_d3dSettings.DepthStencilBufferFormat();
		}
		else
		{
			m_d3dpp.Flags              = 0;
		}

		// Dialog 할수 있게
		m_d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		m_d3dpp.BackBufferWidth  = m_iWidth;
		m_d3dpp.BackBufferHeight = m_iHeight;

		m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
		m_d3dpp.FullScreen_RefreshRateInHz = 0;
		m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();
	
		if( m_bWindowed )
		{
			m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
			m_d3dpp.FullScreen_RefreshRateInHz = 0;
			m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();
		}
		else
		{
			m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
			m_d3dpp.FullScreen_RefreshRateInHz = m_d3dSettings.Fullscreen_DisplayMode.RefreshRate;
			m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();
		}
	}

	HRESULT	NFVideoDX9::InitDeviceObjects()
	{
		// Dialog Box의 Drag 모드일때 자국 안남게
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE,  0,0);

		// Dialog 사용가능
		// D3DFMT_X1R5G5B5, D3DFMT_R5G6B5, D3DFMT_X8R8G8B8 이걸로 BackBuffer이 생성되어야한다.
		if(FAILED(m_pd3dDevice->SetDialogBoxMode(TRUE)))
			return E_FAIL;

		// 디바이스 설정 초기화


		return S_OK;
	}

	HRESULT NFVideoDX9::ChangeVideo(BOOL ChangeWindow)
	{
		if(m_bWindowed == ChangeWindow)
			return S_OK;

		m_bWindowed = ChangeWindow;

		// D3D 재 초기화
		InitDevice();
		
		// 아래 함수에서 InitDeviceObjects() 호출한다.
		ResizeD3D();

		return S_OK;
	}

	//-----------------------------------------------------------------------------
	// Name: FindBestWindowedMode()
	// Desc: Sets up m_d3dSettings with best available windowed mode, subject to 
	//       the bRequireHAL and bRequireREF constraints.  Returns FALSE if no such
	//       mode can be found.
	//-----------------------------------------------------------------------------
	BOOL NFVideoDX9::FindBestWindowedMode( BOOL bRequireHAL, BOOL bRequireREF )
	{
		// Get display mode of primary adapter (which is assumed to be where the window 
		// will appear)
		D3DDISPLAYMODE primaryDesktopDisplayMode;
		m_pD3D->GetAdapterDisplayMode(0, &primaryDesktopDisplayMode);

		D3DAdapterInfo* pBestAdapterInfo = NULL;
		D3DDeviceInfo* pBestDeviceInfo = NULL;
		D3DDeviceCombo* pBestDeviceCombo = NULL;

		for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
		{
			D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
			for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
			{
				D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
				if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
					continue;
				if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
					continue;
				for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
				{
					D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
					BOOL bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
					if (!pDeviceCombo->IsWindowed)
						continue;
					if (pDeviceCombo->AdapterFormat != primaryDesktopDisplayMode.Format)
						continue;
					// If we haven't found a compatible DeviceCombo yet, or if this set
					// is better (because it's a HAL, and/or because formats match better),
					// save it
					if( pBestDeviceCombo == NULL || 
						pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceCombo->DevType == D3DDEVTYPE_HAL ||
						pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
					{
						pBestAdapterInfo = pAdapterInfo;
						pBestDeviceInfo = pDeviceInfo;
						pBestDeviceCombo = pDeviceCombo;
						if( pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
						{
							// This windowed device combo looks great -- take it
							goto EndWindowedDeviceComboSearch;
						}
						// Otherwise keep looking for a better windowed device combo
					}
				}
			}
		}
		EndWindowedDeviceComboSearch:
		if (pBestDeviceCombo == NULL )
			return FALSE;

		m_d3dSettings.pWindowed_AdapterInfo = pBestAdapterInfo;
		m_d3dSettings.pWindowed_DeviceInfo = pBestDeviceInfo;
		m_d3dSettings.pWindowed_DeviceCombo = pBestDeviceCombo;
		m_d3dSettings.IsWindowed = TRUE;
		m_d3dSettings.Windowed_DisplayMode = primaryDesktopDisplayMode;
		m_d3dSettings.Windowed_Width = m_iWidth;
		m_d3dSettings.Windowed_Height = m_iHeight;
		if (m_d3dEnumeration.AppUsesDepthBuffer)
			m_d3dSettings.Windowed_DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
		m_d3dSettings.Windowed_MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
		m_d3dSettings.Windowed_MultisampleQuality = 0;
		m_d3dSettings.Windowed_VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
		m_d3dSettings.Windowed_PresentInterval = *(UINT*)pBestDeviceCombo->pPresentIntervalList->GetPtr(0);
		return TRUE;
	}


	//-----------------------------------------------------------------------------
	// Name: FindBestFullscreenMode()
	// Desc: Sets up m_d3dSettings with best available fullscreen mode, subject to 
	//       the bRequireHAL and bRequireREF constraints.  Returns FALSE if no such
	//       mode can be found.
	//-----------------------------------------------------------------------------
	BOOL NFVideoDX9::FindBestFullscreenMode( BOOL bRequireHAL, BOOL bRequireREF )
	{
		// For fullscreen, default to first HAL DeviceCombo that supports the current desktop 
		// display mode, or any display mode if HAL is not compatible with the desktop mode, or 
		// non-HAL if no HAL is available
		D3DDISPLAYMODE adapterDesktopDisplayMode;
		D3DDISPLAYMODE bestAdapterDesktopDisplayMode;
		D3DDISPLAYMODE bestDisplayMode;
		bestAdapterDesktopDisplayMode.Width = 0;
		bestAdapterDesktopDisplayMode.Height = 0;
		bestAdapterDesktopDisplayMode.Format = D3DFMT_UNKNOWN;
		bestAdapterDesktopDisplayMode.RefreshRate = 0;

		D3DAdapterInfo* pBestAdapterInfo = NULL;
		D3DDeviceInfo* pBestDeviceInfo = NULL;
		D3DDeviceCombo* pBestDeviceCombo = NULL;

		for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
		{
			D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
			m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &adapterDesktopDisplayMode );
			for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
			{
				D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
				if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
					continue;
				if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
					continue;
				for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
				{
					D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
					BOOL bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
					BOOL bAdapterMatchesDesktop = (pDeviceCombo->AdapterFormat == adapterDesktopDisplayMode.Format);
					if (pDeviceCombo->IsWindowed)
						continue;
					// If we haven't found a compatible set yet, or if this set
					// is better (because it's a HAL, and/or because formats match better),
					// save it
					if (pBestDeviceCombo == NULL ||
						pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceInfo->DevType == D3DDEVTYPE_HAL ||
						pDeviceCombo->DevType == D3DDEVTYPE_HAL && pBestDeviceCombo->AdapterFormat != adapterDesktopDisplayMode.Format && bAdapterMatchesDesktop ||
						pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB )
					{
						bestAdapterDesktopDisplayMode = adapterDesktopDisplayMode;
						pBestAdapterInfo = pAdapterInfo;
						pBestDeviceInfo = pDeviceInfo;
						pBestDeviceCombo = pDeviceCombo;
						if (pDeviceInfo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB)
						{
							// This fullscreen device combo looks great -- take it
							goto EndFullscreenDeviceComboSearch;
						}
						// Otherwise keep looking for a better fullscreen device combo
					}
				}
			}
		}
		EndFullscreenDeviceComboSearch:
		if (pBestDeviceCombo == NULL)
			return FALSE;

		// Need to find a display mode on the best adapter that uses pBestDeviceCombo->AdapterFormat
		// and is as close to bestAdapterDesktopDisplayMode's res as possible
		bestDisplayMode.Width = 0;
		bestDisplayMode.Height = 0;
		bestDisplayMode.Format = D3DFMT_UNKNOWN;
		bestDisplayMode.RefreshRate = 0;
		for( UINT idm = 0; idm < pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
		{
			D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
			if( pdm->Format != pBestDeviceCombo->AdapterFormat )
				continue;
			if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
				pdm->Height == bestAdapterDesktopDisplayMode.Height && 
				pdm->RefreshRate == bestAdapterDesktopDisplayMode.RefreshRate )
			{
				// found a perfect match, so stop
				bestDisplayMode = *pdm;
				break;
			}
			else if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
					pdm->Height == bestAdapterDesktopDisplayMode.Height && 
					pdm->RefreshRate > bestDisplayMode.RefreshRate )
			{
				// refresh rate doesn't match, but width/height match, so keep this
				// and keep looking
				bestDisplayMode = *pdm;
			}
			else if( pdm->Width == bestAdapterDesktopDisplayMode.Width )
			{
				// width matches, so keep this and keep looking
				bestDisplayMode = *pdm;
			}
			else if( bestDisplayMode.Width == 0 )
			{
				// we don't have anything better yet, so keep this and keep looking
				bestDisplayMode = *pdm;
			}
		}

		m_d3dSettings.pFullscreen_AdapterInfo = pBestAdapterInfo;
		m_d3dSettings.pFullscreen_DeviceInfo = pBestDeviceInfo;
		m_d3dSettings.pFullscreen_DeviceCombo = pBestDeviceCombo;
		m_d3dSettings.IsWindowed = FALSE;
		m_d3dSettings.Fullscreen_DisplayMode = bestDisplayMode;
		if (m_d3dEnumeration.AppUsesDepthBuffer)
			m_d3dSettings.Fullscreen_DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
		m_d3dSettings.Fullscreen_MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
		m_d3dSettings.Fullscreen_MultisampleQuality = 0;
		m_d3dSettings.Fullscreen_VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
		m_d3dSettings.Fullscreen_PresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
		return TRUE;
	}

	//-----------------------------------------------------------------------------
	// Name: ChooseInitialD3DSettings()
	// Desc: 
	//-----------------------------------------------------------------------------
	HRESULT NFVideoDX9::InitialD3DSettings()
	{
		BOOL bFoundFullscreen = FindBestFullscreenMode( FALSE, FALSE );
		BOOL bFoundWindowed = FindBestWindowedMode( FALSE, FALSE );

		if( !bFoundFullscreen && !bFoundWindowed )
			return D3DAPPERR_NOCOMPATIBLEDEVICES;

		return S_OK;
	}

	//-----------------------------------------------------------------------------
	// Name: ConfirmDeviceHelper()
	// Desc: Static function used by D3DEnumeration
	//-----------------------------------------------------------------------------
	BOOL NFVideoDX9::ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
							D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat )
	{
		DWORD dwBehavior;

		if (vertexProcessingType == SOFTWARE_VP)
			dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		else if (vertexProcessingType == MIXED_VP)
			dwBehavior = D3DCREATE_MIXED_VERTEXPROCESSING;
		else if (vertexProcessingType == HARDWARE_VP)
			dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else if (vertexProcessingType == PURE_HARDWARE_VP)
			dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
		else
			dwBehavior = 0; // TODO: throw exception

		if( dwBehavior & D3DCREATE_PUREDEVICE )
			return FALSE; // GetTransform doesn't work on PUREDEVICE

	/*	
		// Need to support post-pixel processing (for alpha blending)
		if( FAILED( g_Video.m_pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
			adapterFormat, D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
			D3DRTYPE_SURFACE, backBufferFormat ) ) )
		{
			return FALSE;
		}
	*/
		// This sample uses alpha textures and/or straight alpha. Make sure the
		// device supports them
		if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
			return TRUE;
		if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHA )
			return TRUE;

		return FALSE;
	}

	HRESULT NFVideoDX9::Reset()
	{
		if(TestCooperativeLevel()==E_FAIL)
			return ResizeD3D();

		return S_OK;
	}

	HRESULT NFVideoDX9::ResizeD3D()
	{
		// 먼저 Sprite를 삭제하고.. 다시 생성해야한다.
		if(m_pLine)
			m_pLine->OnLostDevice();

		if(m_pSprite)
			m_pSprite->OnLostDevice();

		HRESULT hr = S_OK;
		// Reset the device
		if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) )
		{
			if(hr == D3DERR_DEVICELOST)
				InfoMessage(L"D3DERR_DEVICELOST");
			if(hr == D3DERR_DRIVERINTERNALERROR)
				InfoMessage(L"D3DERR_DRIVERINTERNALERROR");
			if(hr == D3DERR_INVALIDCALL)
				InfoMessage(L"D3DERR_INVALIDCALL");
			if(hr == D3DERR_OUTOFVIDEOMEMORY)
				InfoMessage(L"D3DERR_OUTOFVIDEOMEMORY");
			if(hr == E_OUTOFMEMORY)
				InfoMessage(L"E_OUTOFMEMORY");

			return hr;
		}

		// Store render target surface desc
		LPDIRECT3DSURFACE9 pBackBuffer = NULL;
		m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();

		if(m_pSprite)
			m_pSprite->OnResetDevice();

		if(m_pLine)
			m_pLine->OnResetDevice();
/*
		// D3D Sprite 객체 생성.
		if( FAILED( hr = D3DXCreateSprite( m_pd3dDevice, &m_pSprite) ) )
		{
			return hr;	// 생성실패
		}

		// D3D Line 객체 생성.
		if( FAILED( hr = D3DXCreateLine( m_pd3dDevice, &m_pLine) ) )
		{
			return hr;	// 생성실패
		}
*/
		if( FAILED( hr = InitDeviceObjects() ) )
			return hr;

		m_bDeviceLost = FALSE;

		return S_OK;
	}

	HRESULT	NFVideoDX9::DrawLine(LPD3DXVECTOR2 pLines, int iLineSize, D3DCOLOR Color, float fLineWidth, BOOL bAntialias, BOOL bGLLines)
	{
		m_pLine->SetWidth(fLineWidth);
		m_pLine->SetAntialias(bAntialias);
		m_pLine->SetGLLines(bGLLines);

		HRESULT hr;
		m_pLine->Begin( );
		hr = m_pLine->Draw(pLines, iLineSize, Color);
		m_pLine->End();
		return hr;
	}

	HRESULT	NFVideoDX9::BitBlt(int iX, int iY, LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, BOOL Alpha, D3DCOLOR Color)
	{
		D3DXMATRIX mat_Ortho;
		D3DXMatrixIdentity( &mat_Ortho );

		// 위치 이동
		mat_Ortho._41 = (float)iX;
		mat_Ortho._42 = (float)iY;

		if(Alpha == TRUE || GetAColor(Color) != 255)
		{
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA  );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
		else
		{
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE  );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		}
		HRESULT hr = S_OK;

		m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
		m_pSprite->SetTransform(&mat_Ortho); 
		hr = m_pSprite->Draw(pSrcTexture, pSrcRect, NULL, NULL, Color);
		m_pSprite->End();

		return hr;
	}

	HRESULT	NFVideoDX9::BitBlt(RECT* pDestRect, LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, BOOL Alpha, D3DCOLOR Color)
	{
		HRESULT hr = S_OK;
		int iWidth = pDestRect->right-pDestRect->left;
		int iHeight = pDestRect->bottom-pDestRect->top;

		int tW = pSrcRect->right-pSrcRect->left;
		int tH = pSrcRect->bottom-pSrcRect->top;

		// 가로 세로 스케일 값을 조절한다.
		float fScaleX = (float)iWidth/(float)tW;
		float fScaleY = (float)iHeight/(float)tH;

		D3DXMATRIX mat_Ortho;
		D3DXMatrixIdentity( &mat_Ortho );

		// Scale the world matrix to the size
		mat_Ortho._11 = fScaleX;
		mat_Ortho._22 = fScaleY;
		mat_Ortho._33 = 1.0f;

		// 위치 이동
		mat_Ortho._41 = (float)pDestRect->left;
		mat_Ortho._42 = (float)pDestRect->top;

		if(Alpha == TRUE || GetAColor(Color) != 255)
		{
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA  );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
		else
		{
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE  );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		}

		m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
		m_pSprite->SetTransform(&mat_Ortho); 
		hr = m_pSprite->Draw(pSrcTexture, pSrcRect, NULL, NULL, Color);
		m_pSprite->End();

		return hr;
	}

	HRESULT NFVideoDX9::OnDraw(LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, D3DXMATRIX* World, D3DXVECTOR3* pRotationCenter, D3DXVECTOR3* pTranslation, D3DCOLOR Color)
	{
		HRESULT hr = S_OK;
		m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
		m_pSprite->SetTransform(World); 
		hr = m_pSprite->Draw(pSrcTexture, pSrcRect, pRotationCenter, pTranslation, Color);
		m_pSprite->End();

		return hr;
	}

	HRESULT NFVideoDX9::OnDraw(NFSprite* Image, int iX, int iY)
	{
		if(Image == NULL)
			return E_FAIL;

		if(!Image->IsVisible())
			return E_FAIL;

		if(Image->GetTexture() == NULL)
			return E_FAIL;

		m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  Image->GetSrcBlend()  );
		m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, Image->GetDestBlend() );
		Image->GetMatrix(m_MatOrtho, (float)iX, (float)iY);
		
		D3DXVECTOR3 RotationCenter = D3DXVECTOR3(Image->GetRotCenter().x, Image->GetRotCenter().y, 0.0f);

		HRESULT hr = S_OK;
		// Dx9에서만 정식지원.
		m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
		m_pSprite->SetTransform(&m_MatOrtho); 
		hr = m_pSprite->Draw(Image->GetTexture(), &Image->GetSrcRect(), &RotationCenter, &D3DXVECTOR3((float)0.0f, (float)0.0f, 0.0f), Image->GetColor());
		m_pSprite->End();
		return hr;
	}

	HRESULT NFVideoDX9::TestCooperativeLevel()
	{
		HRESULT hr = m_pd3dDevice->TestCooperativeLevel();
		if(D3DERR_DEVICENOTRESET == hr)
			return E_FAIL;

		return S_OK;
	}

	HRESULT NFVideoDX9::ClearScreen(D3DCOLOR dcColor)
	{
		if( FAILED( m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, dcColor, 1.0f, 0L ) ) )
			return E_FAIL;

		return S_OK;
	}

	///////////////////////////////////////////////////////////////////////////////
	//	Name    : RenderStart()
	//	Param	: 없음
	//	Return	: D3D 디바이스
	//	설  명  : D3D 디바이스에게 렌더링을 시작한다고 알립니다.
	//	Date	: 2003-03-12오전 10:46:52
	///////////////////////////////////////////////////////////////////////////////
	LPD3DDEVICE NFVideoDX9::RenderStart()
	{
		if( m_pd3dDevice == NULL || FAILED( m_pd3dDevice->BeginScene() ) )
			return NULL;

		return m_pd3dDevice;
	}

	///////////////////////////////////////////////////////////////////////////////
	//	Name    : RenderEnd()
	//	Param	: 없음
	//	Return	: 없음
	//	설  명  : D3D 디바이스에게 렌더링을 종료한다고 알립니다.
	//	Date	: 2003-03-12오전 10:46:52
	///////////////////////////////////////////////////////////////////////////////
	HRESULT NFVideoDX9::RenderEnd()
	{
		// End the scene.
		m_pd3dDevice->EndScene();
		return S_OK;
	}

	VOID NFVideoDX9::Present()
	{
		// Present the backbuffer contents to the display
		HRESULT hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

		if( D3DERR_DEVICELOST == hr )
			m_bDeviceLost = TRUE;
	}

}