/** 
 *  @file		NFStringGeneral.h
 *  @brief		문자열 파싱 및 생성클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <string>

namespace Nave { 

	inline std::pair<const wchar_t*, const wchar_t*> Split(const wchar_t* ibegin,const wchar_t* iend, wchar_t delimiter) {
		for ( const wchar_t* i=ibegin; i != iend; ++i) {
			if ( (*i) == delimiter ) {
				if (i == ibegin) {
					++ibegin;
					continue;
				}
				return std::pair<const wchar_t*,const wchar_t*>(ibegin,i);
			}
		}
		return std::pair<const wchar_t*,const wchar_t*>(ibegin,iend);
	}

	template <typename fc> inline std::pair<fc, fc> Split(fc ibegin,const fc& iend, typename fc::value_type delimiter) {

		for ( fc i=ibegin; i != iend; ++i) {
			if ( (*i) == delimiter ) {
				if (i == ibegin) {
					++ibegin;
					continue;
				}
				return std::pair<fc,fc>(ibegin,i);
			}
		}
		return std::pair<fc,fc>(ibegin,iend);
	}

	template <typename iterator, typename container> void
		Split(iterator ibegin, const iterator& iend, typename iterator::value_type delimiter,
		container& result)
	{
		std::pair<iterator, iterator> p(ibegin, ibegin);
        while(p.second!=iend)
		{
			p=Split(p.first, iend, delimiter);
			container::value_type str(p.first, p.second);
			result.push_back(str);
			p.first=p.second;
		}
	}

	inline void LeftTrim(std::string& s) {	s.erase(0, s.find_first_not_of(" ")); }
	inline void LeftTrim(std::wstring& s) {	s.erase(0, s.find_first_not_of(L" ")); }

	inline void RightTrim(std::string& s) { s.resize(s.find_last_not_of(" ") + 1);	}
	inline void RightTrim(std::wstring& s) { s.resize(s.find_last_not_of(L" ") + 1);	}

	template <typename fc > inline void Trim(fc& s)
	{
		LeftTrim(s);
		RightTrim(s);
	}


	std::string Format(char* format, ...);
	std::wstring Format(wchar_t* format, ...);

	std::string PositionalFormat(char* format, ...);
	std::wstring PositionalFormat(wchar_t* format, ...);

	template <typename fc> inline fc ParseFileNameNoPath(const fc& str) {
		fc tmp=ParseFileName(str);
	
		fc::size_type pos = tmp.find_last_of(L"\\/:");
	
		if (pos != std::wstring::npos)
		    return tmp.substr(pos+1);
	    return tmp;
	}
	

	/**
	Extract filename,  "filename|itemname" -> filename

	file name includes path

		"c:\data\bla|something" -> "c:\data\bla"

	*/
	template <typename fc> inline fc ParseFileName(const fc& str) {
		std::pair<typename fc::const_iterator,typename fc::const_iterator> pair=Split(str.begin(),str.end(),L'|');
		return fc(pair.first, pair.second);
	}

	/**
	extracts the item name "filename|itenmae" -> itenname

	"c:\data\blabla.xml\hellsemething" -> "something"

	*/

	template <typename fc> inline fc ParseItemName(const fc& str) {
		std::pair<typename fc::const_iterator,typename fc::const_iterator> pair=Split(str.begin(),str.end(),L'|');
		if ( pair.second == str.end() ) return fc();
		return fc(++pair.second, str.end());
	}


	/**
    "c:\data\blabla.xml\hellsemething" -> "c:\data"
	*/

	inline std::wstring ParseFilePath(const std::wstring& str) {
		std::wstring::size_type pos = str.find_last_of(L"\\/:");
		if (pos != std::wstring::npos)
		    return str.substr(0,pos);
	    return str;
	}

	std::wstring RemoveDots(const std::wstring& str2);

} 