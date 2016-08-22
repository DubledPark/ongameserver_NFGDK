#include "NFXML.h"

namespace Nave { 

	Nave::NFSmartPtr<IXMLStreamReader> CreateXmlStreamReader(const WCHAR* xmlFile)
	{
		return new NFXMLStreamReader(xmlFile);
	}

	Nave::NFSmartPtr<IXMLStreamWriter> CreateXmlStreamWriter(const WCHAR* xmlFile)
	{
		return new NFXMLStreamWriter(xmlFile);
	}

}