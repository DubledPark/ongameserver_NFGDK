/** 
 *  @file		NFStringCompare.h
 *  @brief		문자열 Compare 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	template <typename fc> inline BOOL Compare(fc i1,const fc& iend1,fc i2,const fc& iend2) {
		for (;(i1 != iend1) && ( i2 != iend2); ++i1, ++i2) {
			if ((*i1) != (*i2)) return FALSE;
		}
		if ( (i1 == iend1) && ( i2 == iend2)) return TRUE;
		return FALSE;
	}

	template <typename fc> inline BOOL Compare(std::pair<fc,fc>& a, std::pair<fc,fc>& b) {
		Compare(a.first,a.second,b.first,b.second);
	}

	inline BOOL Compare(std::string::const_iterator i1,const std::string::const_iterator& iend1, const char* i2) {
		for (;(i1 != iend1) && ( *i2 != 0); ++i1, ++i2) {
			if ((*i1) != (*i2)) return FALSE;
		}
		if ( (i1 == iend1) && ( *i2 == 0)) return TRUE;
		return FALSE;
	}

	inline BOOL Compare(std::wstring::const_iterator i1,const std::wstring::const_iterator& iend1, const wchar_t* i2) {
		for (;(i1 != iend1) && ( *i2 != 0); ++i1, ++i2) {
			if ((*i1) != (*i2)) return FALSE;
		}
		if ( (i1 == iend1) && ( *i2 == 0)) return TRUE;
		return FALSE;
	}
	
	inline BOOL Compare(const std::pair<std::string::const_iterator,std::string::const_iterator>& a, const char* b) {
		return Compare(a.first,a.second,b);
	}

} 
