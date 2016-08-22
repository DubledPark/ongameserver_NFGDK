#include "Global.h"
#include "Config.h"

Config::Config(void)
{
}

Config::~Config(void)
{
}

VOID Config::Load(const WCHAR* strFile)
{
	Nave::NFSmartPtr<IXMLStreamReader> pXml = CreateXmlStreamReader(strFile);
	pXml->Read(L"Config", m_Config);
}

VOID Config::Save(const WCHAR* strFile)
{
	Nave::NFSmartPtr<IXMLStreamWriter> pXml = CreateXmlStreamWriter(strFile);
	pXml->Write(L"Config", m_Config);
}