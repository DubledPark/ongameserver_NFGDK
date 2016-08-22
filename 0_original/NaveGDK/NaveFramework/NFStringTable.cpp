#include "NFStringTable.h"
#include <Nave/NFStringUtil.h>
#include <map>

namespace NaveFramework {

	/// ½ºÅÂÆ½ ¼±¾ð
	std::map<Nave::String, NFStringTable*>	NFStringTable::s_strTableMap;

	NFStringTable::NFStringTable()
	{
	}

	NFStringTable::~NFStringTable()
	{
		Release();
	}

	VOID NFStringTable::Release()
	{
	}

	BOOL NFStringTable::Load(const WCHAR* filename)
	{
		FILE* fp = _wfopen(filename, L"r");
		if(fp == NULL)
			return FALSE;

		HEADER stHeader;
		fread(&stHeader, sizeof(stHeader), 1, fp);

		if(strcmp(stHeader.sFI, "stb") != 0)
		{
			fclose(fp);
			return FALSE;
		}

		m_STableMap.clear();
		m_STableIDMap.clear();

		int iTotalCnt = stHeader.iCount;
		int iLen;

		Nave::String stID;
		Nave::String stStr;
		Nave::uint32 hashID;

		WCHAR strBuff[1024];
		for(int i = 0; i < iTotalCnt; ++i)
		{
			// ID
			fread(&iLen, sizeof(int), 1, fp);
			fread(strBuff, sizeof(WCHAR)*iLen, 1, fp);
			strBuff[iLen] = 0;

			stID = strBuff;
			hashID = Nave::Hash(strBuff);
			m_STableIDMap[hashID] = stID;

			// String
			fread(&iLen, sizeof(int), 1, fp);
			fread(strBuff, sizeof(WCHAR)*iLen, 1, fp);
			strBuff[iLen] = 0;

			stStr = strBuff;

			m_STableMap[hashID] = stStr;
		}

		fclose(fp);
		return TRUE;
	}

	Nave::String NFStringTable::FindID(Nave::uint32 HashID)
	{
		iterSTableIDMap obj = m_STableIDMap.find(HashID);

		if(obj == m_STableIDMap.end())
			return L"";

		return obj->second;
	}

	Nave::String NFStringTable::FindString(Nave::uint32 HashID)
	{
		iterSTableMap obj = m_STableMap.find(HashID);

		if(obj == m_STableMap.end())
			return L"";

		return obj->second;
	}

	Nave::String NFStringTable::FindString(const WCHAR* strID)
	{
		return FindString(Nave::Hash(strID));
	}


	/// Static
	BOOL NFStringTable::LoadSTable(const WCHAR* id, const WCHAR* filename)
	{
		Nave::String stID = id;

		NFStringTable* pTable = new NFStringTable();

		if(pTable == NULL)
			return FALSE;

		s_strTableMap[stID] = pTable;

		return pTable->Load(filename);
	}

	VOID NFStringTable::ReleaseAll()
	{
		strTableIter obj = s_strTableMap.begin();
		strTableIter end = s_strTableMap.end();

		while(obj != end)
		{
			_DELETE(obj->second);

			++obj;
		}
	}

	Nave::String NFStringTable::FindID(const WCHAR* strFirst, Nave::uint32 HashID)
	{
		strTableIter find = s_strTableMap.find(strFirst);

		if(find == s_strTableMap.end())
			return L"";

		return s_strTableMap[strFirst]->FindID(HashID);
	}

	Nave::String NFStringTable::FindString(const WCHAR* strFirst, Nave::uint32 HashID)
	{
		strTableIter find = s_strTableMap.find(strFirst);

		if(find == s_strTableMap.end())
			return L"";

		return s_strTableMap[strFirst]->FindString(HashID);
	}

	Nave::String NFStringTable::FindString(const WCHAR* strFirst, const WCHAR* strID)
	{
		strTableIter find = s_strTableMap.find(strFirst);

		if(find == s_strTableMap.end())
			return L"";

		return s_strTableMap[strFirst]->FindString(strID);
	}

}