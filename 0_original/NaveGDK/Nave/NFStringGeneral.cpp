#include "NFStringUtil.h"

namespace Nave { 

	std::string Format(char* format, ...) {
		static char txt[1024];
		va_list l;
		va_start(l,format);
		_vsnprintf( txt, 1024, format, l );
		va_end(l); 
		return txt;
	}

	std::wstring Format(wchar_t* format, ...) {
		static wchar_t txt[1024];
		va_list l;
		va_start(l,format);
		_vsnwprintf( txt, 1024, format, l );
		va_end(l); 
		return txt;
	}

	std::wstring RemoveDots(const std::wstring& str2) 
	{
		std::wstring str=str2;
		// balal/rear/../reareako.txt

		size_t pos1=str.find(L"/..");
		while ( pos1 != std::wstring::npos) 
		{
			BOOL found = FALSE;
			for ( int i=int(pos1)-1; i >= 0; --i) 
			{
				if ( str[i] == L'/' ) 
				{
					str=str.substr(0,i)+str.substr(pos1+3);
					found=TRUE;
					break;
				} 

			}
			if (!found) 
			{
				str=str.substr(pos1+3);
			}


			pos1=str.find(L"/..");
		}

		return str;		
	}

} 