/** 
 *  @file		NFAdapter.h
 *  @brief		Enumerates available D3D adapters, devices, modes, etc.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>
#include <d3d9types.h>
#include <d3d9caps.h>
#include <D3DX9Math.h>

// D3D 재선언.
#define LPD3D LPDIRECT3D9
#define LPD3DDEVICE LPDIRECT3DDEVICE9
#define LPD3DTEXTURE LPDIRECT3DTEXTURE9
#define LPD3DVERTEXBUFFER LPDIRECT3DVERTEXBUFFER9
#define LPD3DINDEXBUFFER LPDIRECT3DINDEXBUFFER9


#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d

namespace NaveFramework {

	//-----------------------------------------------------------------------------
	// Name: enum VertexProcessingType
	// Desc: Enumeration of all possible D3D vertex processing types.
	//-----------------------------------------------------------------------------
	enum VertexProcessingType
	{
		SOFTWARE_VP,
		MIXED_VP,
		HARDWARE_VP,
		PURE_HARDWARE_VP
	};

	//-----------------------------------------------------------------------------
	// Name: ArrayListType
	// Desc: Indicates how data should be stored in a CArrayList
	//-----------------------------------------------------------------------------
	enum ArrayListType
	{
		AL_VALUE,       // entry data is copied into the list
		AL_REFERENCE,   // entry pointers are copied into the list
	};


	//-----------------------------------------------------------------------------
	// Name: CArrayList
	// Desc: A growable array
	//-----------------------------------------------------------------------------
	class CArrayList
	{
	public:
		CArrayList( ArrayListType Type, UINT BytesPerEntry = 0 );
		~CArrayList( void );
		HRESULT Add( void* pEntry );
		void Remove( UINT Entry );
		void* GetPtr( UINT Entry );
		UINT Count( void ) { return m_NumEntries; }
		BOOL Contains( void* pEntryData );
		void Clear( void ) { m_NumEntries = 0; }

	private:
		ArrayListType m_ArrayListType;
		void* m_pData;
		UINT m_BytesPerEntry;
		UINT m_NumEntries;
		UINT m_NumEntriesAllocated;
	};

	//-----------------------------------------------------------------------------
	// Name: struct D3DAdapterInfo
	// Desc: Info about a display adapter.
	//-----------------------------------------------------------------------------
	struct D3DAdapterInfo
	{
		int AdapterOrdinal;
		D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
		CArrayList* pDisplayModeList; // List of D3DDISPLAYMODEs
		CArrayList* pDeviceInfoList; // List of D3DDeviceInfo pointers
		~D3DAdapterInfo( void );
	};


	//-----------------------------------------------------------------------------
	// Name: struct D3DDeviceInfo
	// Desc: Info about a D3D device, including a list of D3DDeviceCombos (see below) 
	//       that work with the device.
	//-----------------------------------------------------------------------------
	struct D3DDeviceInfo
	{
		int AdapterOrdinal;
		D3DDEVTYPE DevType;
		D3DCAPS9 Caps;
		CArrayList* pDeviceComboList; // List of D3DDeviceCombo pointers
		~D3DDeviceInfo( void );
	};


	//-----------------------------------------------------------------------------
	// Name: struct D3DDSMSConflict
	// Desc: A depth/stencil buffer format that is incompatible with a
	//       multisample type.
	//-----------------------------------------------------------------------------
	struct D3DDSMSConflict
	{
		D3DFORMAT DSFormat;
		D3DMULTISAMPLE_TYPE MSType;
	};


	//-----------------------------------------------------------------------------
	// Name: struct D3DDeviceCombo
	// Desc: A combination of adapter format, back buffer format, and windowed/fullscreen 
	//       that is compatible with a particular D3D device (and the app).
	//-----------------------------------------------------------------------------
	struct D3DDeviceCombo
	{
		int AdapterOrdinal;
		D3DDEVTYPE DevType;
		D3DFORMAT AdapterFormat;
		D3DFORMAT BackBufferFormat;
		BOOL IsWindowed;
		CArrayList* pDepthStencilFormatList; // List of D3DFORMATs
		CArrayList* pMultiSampleTypeList; // List of D3DMULTISAMPLE_TYPEs
		CArrayList* pMultiSampleQualityList; // List of DWORDs (number of quality 
											// levels for each multisample type)
		CArrayList* pDSMSConflictList; // List of D3DDSMSConflicts
		CArrayList* pVertexProcessingTypeList; // List of VertexProcessingTypes
		CArrayList* pPresentIntervalList; // List of D3DPRESENT_INTERVALs

		~D3DDeviceCombo( void );
	};


	typedef BOOL(* CONFIRMDEVICECALLBACK)( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
										D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat );


	/** 
	 *  @class        NFAdapter
	 *  @brief        Enumerates available D3D adapters, devices, modes, etc.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFAdapter
	{
	public:
		NFAdapter();
		~NFAdapter();

	public:
		void SetD3D(IDirect3D9* pD3D) { m_pD3D = pD3D; }
		HRESULT Enumerate();

	public:
		CArrayList* m_pAdapterInfoList;
		// The following variables can be used to limit what modes, formats, 
		// etc. are enumerated.  Set them to the values you want before calling
		// Enumerate().
		CONFIRMDEVICECALLBACK ConfirmDeviceCallback;
		UINT AppMinFullscreenWidth;
		UINT AppMinFullscreenHeight;
		UINT AppMinColorChannelBits; // min color bits per channel in adapter format
		UINT AppMinAlphaChannelBits; // min alpha bits per pixel in back buffer format
		UINT AppMinDepthBits;
		UINT AppMinStencilBits;
		BOOL AppUsesDepthBuffer;
		BOOL AppUsesMixedVP; // whether app can take advantage of mixed vp mode
		BOOL AppRequiresWindowed;
		BOOL AppRequiresFullscreen;
		CArrayList* m_pAllowedAdapterFormatList; // list of D3DFORMATs

	private:
		HRESULT EnumerateDevices( D3DAdapterInfo* pAdapterInfo, CArrayList* pAdapterFormatList );
		HRESULT EnumerateDeviceCombos( D3DDeviceInfo* pDeviceInfo, CArrayList* pAdapterFormatList );
		void BuildDepthStencilFormatList( D3DDeviceCombo* pDeviceCombo );
		void BuildMultiSampleTypeList( D3DDeviceCombo* pDeviceCombo );
		void BuildDSMSConflictList( D3DDeviceCombo* pDeviceCombo );
		void BuildVertexProcessingTypeList( D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo );
		void BuildPresentIntervalList( D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo );

	private:
		IDirect3D9* m_pD3D;
	};

}