/** 
 *  @file  NFXMLScript.h
 *  @brief  XML형식의 스크립트를 관리하는 클래스
 *  @remarks 
 *  @author  강동명 (edith2580@gmail.com)
 *  @date  2009-04-25
 */

#pragma once

#include "NFXML.h"

namespace Nave {

	/** 
	 *  @class        NFXMLScript
	 *  @brief        하나의 XML형식의 스크립트를 로드하기 위해서 관리되는 UIScript 클래스
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-25
	 */
	class NFXMLScript
	{
	public:
		NFXMLScript();
		~NFXMLScript();

	public:
		void Serialize(IXMLStreamWriter& out) const
		{
			out.Begin(L"XMLScript");
			out.Write(L"Header",m_stHeader);
			out.Write(L"Attribute",m_stAttribute);
		}

		void Deserialize(IXMLStreamReader& in) {
			in.Begin(L"XMLScript");
			in.Read(L"Header",m_stHeader);
			in.Read(L"Attribute",m_stAttribute);
		}

		VOID	Load(WCHAR* strFile);
		VOID	Save(WCHAR* strFile);

	public:
		typedef struct SCRIPTITEM
		{
			std::map<String, String>	items;
			typedef std::map<String, String>::const_iterator constIter;
			typedef std::map<String, String>::iterator		 iter;

			void Serialize(IXMLStreamWriter& out) const
			{
				out.Begin(L"ScriptItem");
				out.Write(L"Items",items);
			}

			void Deserialize(IXMLStreamReader& in) {
				in.Begin(L"ScriptItem");
				in.Read(L"Items",items);
			}

			int GetInt(const String& key)
			{
				constIter find = items.find(key);
				if(find == items.end())
					return 0;

				return ToInt(find->second);
			}

			float GetFloat(const String& key)
			{
				constIter find = items.find(key);
				if(find == items.end())
					return 0;

				return ToFloat(find->second);
			}

			float2 GetFloat2(const String& key)
			{
				constIter find = items.find(key);
				float2 ret(0,0);

				if(find != items.end())
					swscanf(find->second.c_str(), L"%f, %f", &ret.x, &ret.y);

				return ret;
			}

			float3 GetFloat3(const String& key)
			{
				constIter find = items.find(key);
				float3 ret(0,0,0);

				if(find != items.end())
					swscanf(find->second.c_str(), L"%f, %f, %f", &ret.x, &ret.y, &ret.z);

				return ret;
			}

			float4 GetFloat4(const String& key)
			{
				constIter find = items.find(key);
				float4 ret(0,0,0,0);

				if(find != items.end())
					swscanf(find->second.c_str(), L"%f, %f, %f, %f", &ret.x, &ret.y, &ret.z, &ret.w);

				return ret;
			}

			CPoint GetPoint(const String& key)
			{
				constIter find = items.find(key);
				CPoint ret(0,0);

				if(find != items.end())
					swscanf(find->second.c_str(), L"%d, %d", &ret.x, &ret.y);

				return ret;
			}

			CRect GetRect(const String& key)
			{
				constIter find = items.find(key);
				CRect ret(0,0,0,0);

				if(find != items.end())
					swscanf(find->second.c_str(), L"%d, %d, %d, %d", &ret.left, &ret.top, &ret.right, &ret.bottom);

				return ret;
			}

			BOOL GetString(const String& key, String& value)
			{
				constIter find = items.find(key);
				if(find == items.end())
					return FALSE;

				value = find->second;

				return TRUE;
			}

			VOID SetItem(const String& key, int value)
			{
				WCHAR temp[32];
				swprintf(temp, L"%d", value);

				items[key] = temp;
			}

			VOID SetItem(const String& key, float value)
			{
				WCHAR temp[32];
				swprintf(temp, L"%f", value);

				items[key] = temp;
			}

			VOID SetItem(const String& key, const float2& value)
			{
				WCHAR temp[32];
				swprintf(temp, L"%f, %f", value.x, value.y);

				items[key] = temp;
			}

			VOID SetItem(const String& key, const float3& value)
			{
				WCHAR temp[32];
				swprintf(temp, L"%f, %f, %f", value.x, value.y, value.z);

				items[key] = temp;
			}

			VOID SetItem(const String& key, const float4& value)
			{
				WCHAR temp[32];
				swprintf(temp, L"%f, %f, %f, %f", value.x, value.y, value.z, value.w);

				items[key] = temp;
			}

			VOID SetItem(const String& key, const CPoint& value)
			{
				WCHAR temp[32];
				swprintf(temp, L"%d, %d", value.x, value.y);

				items[key] = temp;
			}

			VOID SetItem(const String& key, const CRect& value)
			{
				WCHAR temp[32];
				swprintf(temp, L"%d, %d, %d, %d", value.left, value.top, value.right, value.bottom);

				items[key] = temp;
			}

			VOID SetItem(const String& key, const String& value)
			{
				items[key] = value;
			}
		}*LPSCRIPTITEM;

		inline LPSCRIPTITEM GetHeader() { return &m_stHeader; }

		inline void SetHeader(LPSCRIPTITEM lpHeader) 
		{
			m_stHeader = *lpHeader;
		}

		inline LPSCRIPTITEM GetAttribute(int iIndex) 
		{ 
			if((int)m_stAttribute.size() <= iIndex)
				return NULL;

			return &m_stAttribute[iIndex]; 
		}

		inline void AddAttribute(LPSCRIPTITEM lpItem) 
		{
			m_stAttribute.push_back(*lpItem);
		}

	private:
		SCRIPTITEM				m_stHeader;

		std::vector<SCRIPTITEM>	m_stAttribute;
	};

}