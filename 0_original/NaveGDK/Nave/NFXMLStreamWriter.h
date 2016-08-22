/** 
 *  @file		NFXMLStreamWriter.h
 *  @brief		XML Stream Writer 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class			NFXMLStreamWriter
	 *  @brief			XML Write
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFXMLStreamWriter : public IXMLStreamWriter
	{
		TiXmlDocument* pXmlDoc;

		std::list<TiXmlNode*> pNodeList;

		void writeStartDocument() 
		{
			// xml 도큐먼트에 Header을 추가한다.
			// 이건 xml 에 인코딩 헤더를 추가하는 루틴.
			if(!pXmlDoc)
				return;

			pXmlDoc->InsertDeclaration(L"1.0", L"");
		}

		void writeEndDocument() 
		{
			pNodeList.clear();

			// 현재 xml 도큐먼트를 안쓴다.
			if(!pXmlDoc)
				return;

			pXmlDoc->SaveFile();
		}

	public:
		NFXMLStreamWriter(const WCHAR* xmlFile)  : pXmlDoc(NULL)
		{
			pXmlDoc = new TiXmlDocument(xmlFile);

			writeStartDocument();
		}

		~NFXMLStreamWriter() 
		{
			writeEndDocument();

			if(pXmlDoc)
			{
				delete pXmlDoc;
				pXmlDoc = NULL;
			}
		}

		void NewNode(const WCHAR* name)
		{
			if(pNodeList.empty())
				pNodeList.push_back(pXmlDoc->InsertEndChild(TiXmlElement(name)));
			else
				pNodeList.push_back(pNodeList.back()->InsertEndChild(TiXmlElement(name)));
		}

		void Begin(const WCHAR* type)
		{
			SetAttribute(L"type", type, 0);
		}

		void End()
		{
			pNodeList.pop_back();
		}

		void SetAttribute(const WCHAR* name, const WCHAR* value, int depthtype = 1)
		{
			pNodeList.back()->ToElement()->SetAttribute(name, value, depthtype);
		}

		void Write(const WCHAR* name, signed short value)
		{
			NewNode(name);

			SetAttribute(L"type", L"int16");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, unsigned short value)
		{
			NewNode(name);

			SetAttribute(L"type", L"uint16");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, int value)
		{
			NewNode(name);

			SetAttribute(L"type", L"int32");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, unsigned int value)
		{
			NewNode(name);

			SetAttribute(L"type", L"uint32");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, long value)
		{
			NewNode(name);

			SetAttribute(L"type", L"long");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, unsigned long value)
		{
			NewNode(name);

			SetAttribute(L"type", L"ulong");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, double value)
		{
			NewNode(name);

			SetAttribute(L"type", L"double");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, float value)
		{
			NewNode(name);

			SetAttribute(L"type", L"float");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, const float2& value)
		{
			NewNode(name);

			SetAttribute(L"type", L"float2");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, const float3& value)
		{
			NewNode(name);

			SetAttribute(L"type", L"float3");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, const float4& value)
		{
			NewNode(name);

			SetAttribute(L"type", L"float4");
			SetAttribute(L"value", Nave::To<Nave::String>(value).c_str());

			End();
		}

		void Write(const WCHAR* name, const CHAR* buffer)
		{
			NewNode(name);

			SetAttribute(L"type", L"string");
			SetAttribute(L"value", Nave::ToString(buffer).c_str());

			End();
		}

		void Write(const WCHAR* name, const WCHAR* buffer)
		{
			NewNode(name);

			SetAttribute(L"type", L"wstring");
			SetAttribute(L"value", Nave::ToString(buffer).c_str());

			End();
		}

		void Write(const WCHAR* name, const ISerializable& value)
		{
			NewNode(name);

			value.Serialize(*this);

			End();
		}

	};

	/// XML Writer 객체 생성
	Nave::NFSmartPtr<IXMLStreamWriter> CreateXmlStreamWriter(const WCHAR* xmlFile);

}