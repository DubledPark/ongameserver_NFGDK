#include "Nave.h"
#include "NFXMLScript.h"

namespace Nave {

	NFXMLScript::NFXMLScript()
	{
	}

	NFXMLScript::~NFXMLScript()
	{
	}

	VOID NFXMLScript::Load(WCHAR* strFile)
	{
		NFSmartPtr<IXMLStreamReader> pXml = CreateXmlStreamReader(strFile);
		pXml->Read(L"XMLScript", *this);
	}

	VOID NFXMLScript::Save(WCHAR* strFile)
	{
		NFSmartPtr<IXMLStreamWriter> pXml = CreateXmlStreamWriter(strFile);
		pXml->Write(L"XMLScript", *this);
	}

}