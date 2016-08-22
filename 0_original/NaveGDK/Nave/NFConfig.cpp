#include <windows.h>
#include <assert.h>
#include "NFConfig.h"

namespace Nave { 

	NFConfig::NFConfig() : m_pXmlDoc(NULL), m_pRoot(NULL)
	{
	}

	NFConfig::~NFConfig()
	{
		Close();
	}

	void NFConfig::Close()
	{
		m_pRoot = NULL;

		if(m_pXmlDoc)
		{
			delete m_pXmlDoc;
			m_pXmlDoc = NULL;
		}
	}

	BOOL NFConfig::Parse_XML(WCHAR* lpszFilename)
	{
		try
		{
			BOOL okey = m_pXmlDoc->LoadFile(lpszFilename);
			if(okey)
			{
				m_pRoot = m_pXmlDoc->FirstChild(L"Config");

				if( !m_pRoot)
					return FALSE;
			}

			return TRUE;
		}
		catch(...)
		{
			return FALSE;
		}	
	}

	void NFConfig::Get(WCHAR* category, WCHAR* name, SHORT& value)
	{
		int v;
		Get(category, name, v);
		value = (SHORT)v;
	}

	void NFConfig::Get(WCHAR* category, WCHAR* name, USHORT& value)
	{
		int v;
		Get(category, name, v);
		value = (USHORT)v;
	}

	void NFConfig::Get(WCHAR* category, WCHAR* name, INT& value)
	{
		WCHAR Buff[32];
		Get(category, name, Buff);
		value = _wtoi(Buff);
	}

	void NFConfig::Get(WCHAR* category, WCHAR* name, bool& value)
	{
		int v;
		Get(category, name, v);

		if(v) 
			value = true;
		else 
			value = false;
	}

	void NFConfig::Get(WCHAR* category, WCHAR* name, UINT& value)
	{
		int v;
		Get(category, name, v);
		value = (UINT)v;
	}

	void NFConfig::Get(WCHAR* category, WCHAR* name, DOUBLE& value)
	{
		WCHAR Buff[32];
		Get(category, name, Buff);
		value = _wtof(Buff);
	}

	void NFConfig::Get(WCHAR* category, WCHAR* name, FLOAT& value)
	{
		WCHAR Buff[32];
		Get(category, name, Buff);
		value = (FLOAT)_wtof(Buff);
	}

	void NFConfig::Get(WCHAR* category, WCHAR* name, WCHAR* value)
	{
		try
		{
			TiXmlNode *pCate = m_pRoot->FirstChild(category);

			TiXmlNode *pValue = pCate->FirstChild(name);
			const WCHAR* nameStr = (pValue->ToElement())->Attribute(L"value");

			wcscpy(value, Nave::ToUnicode(nameStr).c_str());
		}
		catch(...)
		{
		}
	}

	void NFConfig::Set(WCHAR* category, WCHAR* name, SHORT value)
	{
		WCHAR Buff[32];
		swprintf(Buff, L"%d", value);
		Set(category, name, Buff);
	}

	void NFConfig::Set(WCHAR* category, WCHAR* name, USHORT value)
	{
		WCHAR Buff[32];
		swprintf(Buff, L"%d", value);
		Set(category, name, Buff);
	}

	void NFConfig::Set(WCHAR* category, WCHAR* name, INT value)
	{
		WCHAR Buff[32];
		swprintf(Buff, L"%d", value);
		Set(category, name, Buff);
	}

	void NFConfig::Set(WCHAR* category, WCHAR* name, bool value)
	{
		WCHAR Buff[32];
		swprintf(Buff, L"%d", (int)value);
		Set(category, name, Buff);
	}

	void NFConfig::Set(WCHAR* category, WCHAR* name, UINT value)
	{
		WCHAR Buff[32];
		swprintf(Buff, L"%d", value);
		Set(category, name, Buff);
	}

	void NFConfig::Set(WCHAR* category, WCHAR* name, DOUBLE value)
	{
		WCHAR Buff[32];
		swprintf(Buff, L"%f", value);
		Set(category, name, Buff);

	}

	void NFConfig::Set(WCHAR* category, WCHAR* name, FLOAT value)
	{
		WCHAR Buff[32];
		swprintf(Buff, L"%f", value);
		Set(category, name, Buff);
	}

	void NFConfig::Set(WCHAR* category, WCHAR* name, WCHAR* value)
	{
		try
		{
			TiXmlNode *pCate = m_pRoot->FirstChild(category);
			if(pCate == NULL)
				pCate = m_pRoot->InsertEndChild(TiXmlElement(category));

			TiXmlNode *pValue = pCate->InsertEndChild(TiXmlElement(name));
			pValue->ToElement()->SetAttribute(L"value", (WCHAR*)Nave::ToString(value).c_str());
		}
		catch(...)
		{
		}
	}

}