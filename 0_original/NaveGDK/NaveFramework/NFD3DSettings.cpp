//-----------------------------------------------------------------------------
// File: D3DSettings.cpp
//
// Desc: Settings class and change-settings dialog class for the Direct3D 
//       samples framework library.
//
// Copyright (c) 1998-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include "NaveFramework.h"
#include "NFAdapter.h"
#include "NFD3DSettings.h"

namespace NaveFramework { 

	//-----------------------------------------------------------------------------
	// Name: D3DDevTypeToString
	// Desc: Returns the string for the given D3DDEVTYPE.
	//-----------------------------------------------------------------------------
	WCHAR* D3DDevTypeToString(D3DDEVTYPE devType)
	{
		switch (devType)
		{
		case D3DDEVTYPE_HAL:        return L"D3DDEVTYPE_HAL";
		case D3DDEVTYPE_SW:         return L"D3DDEVTYPE_SW";
		case D3DDEVTYPE_REF:        return L"D3DDEVTYPE_REF";
		default:                    return L"Unknown devType";
		}
	}


	//-----------------------------------------------------------------------------
	// Name: MultisampleTypeToString
	// Desc: Returns the string for the given D3DMULTISAMPLE_TYPE.
	//-----------------------------------------------------------------------------
	WCHAR* MultisampleTypeToString(D3DMULTISAMPLE_TYPE MultiSampleType)
	{
		switch (MultiSampleType)
		{
		case D3DMULTISAMPLE_NONE:   return L"D3DMULTISAMPLE_NONE";
		case D3DMULTISAMPLE_NONMASKABLE: return L"D3DMULTISAMPLE_NONMASKABLE";
		case D3DMULTISAMPLE_2_SAMPLES: return L"D3DMULTISAMPLE_2_SAMPLES";
		case D3DMULTISAMPLE_3_SAMPLES: return L"D3DMULTISAMPLE_3_SAMPLES";
		case D3DMULTISAMPLE_4_SAMPLES: return L"D3DMULTISAMPLE_4_SAMPLES";
		case D3DMULTISAMPLE_5_SAMPLES: return L"D3DMULTISAMPLE_5_SAMPLES";
		case D3DMULTISAMPLE_6_SAMPLES: return L"D3DMULTISAMPLE_6_SAMPLES";
		case D3DMULTISAMPLE_7_SAMPLES: return L"D3DMULTISAMPLE_7_SAMPLES";
		case D3DMULTISAMPLE_8_SAMPLES: return L"D3DMULTISAMPLE_8_SAMPLES";
		case D3DMULTISAMPLE_9_SAMPLES: return L"D3DMULTISAMPLE_9_SAMPLES";
		case D3DMULTISAMPLE_10_SAMPLES: return L"D3DMULTISAMPLE_10_SAMPLES";
		case D3DMULTISAMPLE_11_SAMPLES: return L"D3DMULTISAMPLE_11_SAMPLES";
		case D3DMULTISAMPLE_12_SAMPLES: return L"D3DMULTISAMPLE_12_SAMPLES";
		case D3DMULTISAMPLE_13_SAMPLES: return L"D3DMULTISAMPLE_13_SAMPLES";
		case D3DMULTISAMPLE_14_SAMPLES: return L"D3DMULTISAMPLE_14_SAMPLES";
		case D3DMULTISAMPLE_15_SAMPLES: return L"D3DMULTISAMPLE_15_SAMPLES";
		case D3DMULTISAMPLE_16_SAMPLES: return L"D3DMULTISAMPLE_16_SAMPLES";
		default:                    return L"Unknown Multisample Type";
		}
	}


	//-----------------------------------------------------------------------------
	// Name: VertexProcessingTypeToString
	// Desc: Returns the string for the given VertexProcessingType.
	//-----------------------------------------------------------------------------
	WCHAR* VertexProcessingTypeToString(VertexProcessingType vpt)
	{
		switch (vpt)
		{
		case SOFTWARE_VP:      return L"SOFTWARE_VP";
		case MIXED_VP:         return L"MIXED_VP";
		case HARDWARE_VP:      return L"HARDWARE_VP";
		case PURE_HARDWARE_VP: return L"PURE_HARDWARE_VP";
		default:               return L"Unknown VertexProcessingType";
		}
	}


	//-----------------------------------------------------------------------------
	// Name: PresentIntervalToString
	// Desc: Returns the string for the given present interval.
	//-----------------------------------------------------------------------------
	WCHAR* PresentIntervalToString( UINT pi )
	{
		switch( pi )
		{
		case D3DPRESENT_INTERVAL_IMMEDIATE: return L"D3DPRESENT_INTERVAL_IMMEDIATE";
		case D3DPRESENT_INTERVAL_DEFAULT:   return L"D3DPRESENT_INTERVAL_DEFAULT";
		case D3DPRESENT_INTERVAL_ONE:       return L"D3DPRESENT_INTERVAL_ONE";
		case D3DPRESENT_INTERVAL_TWO:       return L"D3DPRESENT_INTERVAL_TWO";
		case D3DPRESENT_INTERVAL_THREE:     return L"D3DPRESENT_INTERVAL_THREE";
		case D3DPRESENT_INTERVAL_FOUR:      return L"D3DPRESENT_INTERVAL_FOUR";
		default:                            return L"Unknown PresentInterval";
		}
	}


}