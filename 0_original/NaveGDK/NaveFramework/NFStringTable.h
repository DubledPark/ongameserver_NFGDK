/** 
 *  @file		NFStringTable.h
 *  @brief		String 리스트를 관리하는 String Table 클래스이다.
 *  @remarks 
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-05-06
 */
#pragma once

#include <Nave/Nave.h>
#include <map>

namespace NaveFramework {

	class NFStringTable 
	{
	public:
		static std::map<Nave::String, NFStringTable*>				s_strTableMap;
		typedef std::map<Nave::String, NFStringTable*>::iterator	strTableIter;

		static BOOL LoadSTable(const WCHAR* id, const WCHAR* filename);
		static VOID ReleaseAll();

		static Nave::String	FindString(const WCHAR* strFirst, const WCHAR* strID);
		static Nave::String	FindString(const WCHAR* strFirst, Nave::uint32 HashID);
		static Nave::String	FindID(const WCHAR* strFirst, Nave::uint32 HashID);

	public:
		NFStringTable();
		~NFStringTable();

	public:
		typedef struct tagHeader
		{
			char	sFI[4];		// 타입
			int		iCount;		// 개수
			BYTE	dDummy[4];	// 더미
		}HEADER;

	public:
		BOOL Load(const WCHAR* filename);
		VOID Release();

		Nave::String	FindString(const WCHAR* strID);
		Nave::String	FindString(Nave::uint32 HashID);
		Nave::String	FindID(Nave::uint32 HashID);

	private:
		std::map<Nave::uint32, Nave::String>			m_STableMap;
		typedef std::map<Nave::uint32, Nave::String>::iterator	iterSTableMap;

		std::map<Nave::uint32, Nave::String>			m_STableIDMap;
		typedef std::map<Nave::uint32, Nave::String>::iterator	iterSTableIDMap;
	};
}