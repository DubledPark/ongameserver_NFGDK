/** 
 *  @file		NFTypes.h
 *  @brief		Nave Basic Type 선언
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <string>
#include <d3dx9math.h>
#include <atltypes.h>

#pragma warning (disable : 4996)

namespace Nave {

/*
	int				*				signed, signed int					System dependent 
	unsigned int	*				unsigned							System dependent 
	__int8			1				char, signed char					-128 to 127 
	__int16			2				short, short int, signed short int	-32,768 to 32,767 
	__int32			4				signed, signed int					-2,147,483,648 to 2,147,483,647 
	__int64			8				none								-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 
	bool			1				none								false or true 
	char			1				signed char							-128 to 127 
	unsigned char	1				none								0 to 255 
	short			2				short int, signed short int			-32,768 to 32,767 
	unsigned short	2				unsigned short int					0 to 65,535 
	long			4				long int, signed long int			-2,147,483,648 to 2,147,483,647 
	long long		8				none (but equivalent to __int64)	-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 
	unsigned long	4				unsigned long int					0 to 4,294,967,295 
	enum			*				none								Same as int 
	float			4				none								3.4E +/- 38 (7 digits) 
	double			8				none								1.7E +/- 308 (15 digits) 
	long double		same as double	none								same as double 
	wchar_t			2				__wchar_t							0 to 65,535 


	strlen		_tcslen
	strcpy		_tcscpy
	strncpy		_tcsncpy
	strcat		_tcscat
	strcmp		_tcscmp
	strncmp		_tcsncmp
	stricmp		_tcsicmp	
	strnicmp	_tcsnicmp	
	sscanf		_stscanf

	strtok		_tcstok

	_strdate	_tstrdate
	_strtime	_tstrtime

	sprintf		_stprintf
	printf		_tprintf

	atoi		_tstoi
	atof		_tstof

	fopen		_tfopen
	fprintf		_ftprintf
	fgets		_fgetts
*/


// AddFontResource : 특정경로의 ttf폰트를 로드한다, 다한다음 ReleaseAddFone 해줘야한다.
// res = AddFontResource("C:\Fonts\Nordic__.ttf")
// 폰트리스트 EnumFontFamilies 
// gdi+에서 폰트등록 PrivateFontCollection::AddFontFile(...)  

	// basic value types
	typedef std::wstring		StringW;	/// std::wstring
	typedef std::string			StringA;	/// std::string

	typedef StringW				String;		/// default string

	typedef char			    int8;		/// -128 to 127
	typedef short			    int16;		/// -32,768 to 32,767
	typedef int					int32;		/// -2,147,483,648 to 2,147,483,647
	typedef __int64             int64;		/// -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

	typedef unsigned char       uint8;		/// 0 to 255
	typedef unsigned short int  uint16;		/// 0 to 65535
	typedef unsigned int	    uint32;		/// 0 to 4,294,967,295
	typedef unsigned __int64    uint64;		/// 0 to 18,446,744,073,709,551,615

	typedef D3DXVECTOR2			float2;		/// 2vector float
	typedef D3DXVECTOR3			float3;		/// 3vector float
	typedef D3DXVECTOR4			float4;		/// 4vector float
}

