#include <Nave/Nave.h>
#include "Config.h"

CConfig::CConfig(void)
{
}

CConfig::~CConfig(void)
{
}

VOID CConfig::Load(WCHAR* strFile)
{
	Nave::NFSmartPtr<IXMLStreamReader> pXml = CreateXmlStreamReader(strFile);
	pXml->Read(L"Config", m_Config);
}

VOID CConfig::Save(WCHAR* strFile)
{
	Nave::NFSmartPtr<IXMLStreamWriter> pXml = CreateXmlStreamWriter(strFile);
	pXml->Write(L"Config", m_Config);
}