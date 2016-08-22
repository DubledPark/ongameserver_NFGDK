#pragma once

#include <Nave/NFXML.h>

using namespace Nave;

typedef struct GLOBAL
{
	Nave::String	Title;
	Nave::uint32	ID;
	Nave::uint32	Port;
	Nave::uint32	MaxConn;
	Nave::uint32	LogSave;
	Nave::String	Version;

	void Serialize(IXMLStreamWriter& out) const
	{
		out.Begin(L"Struct");
		out.Write(L"Title",Title);
		out.Write(L"ID",ID);
		out.Write(L"Port",Port);
		out.Write(L"MaxConn",MaxConn);
		out.Write(L"LogSave",LogSave);
		out.Write(L"Version",Version);
	}

	void Deserialize(IXMLStreamReader& in) {
		in.Begin(L"Struct");
		in.Read(L"Title",Title);
		in.Read(L"ID",ID);
		in.Read(L"Port",Port);
		in.Read(L"MaxConn",MaxConn);
		in.Read(L"LogSave",LogSave);
		in.Read(L"Version",Version);
	}
}GLOBAL, *LPGLOBAL;

typedef struct CONFIG
{
	// 이팩트에는 가속이 없다.
	GLOBAL		Global;

	void Serialize(IXMLStreamWriter& out) const
	{
		out.Begin(L"Struct");
		out.Write(L"Global",Global);
	}

	void Deserialize(IXMLStreamReader& in) 
	{
		in.Begin(L"Struct");
		in.Read(L"Global",Global);
	}
}CONFIG, *LPCONFIG;

class Config
{
	CONFIG	m_Config;

public:
	VOID	Load(const WCHAR* strFile = L"Config.xml");
	VOID	Save(const WCHAR* strFile = L"Config.xml");

	const WCHAR*	GetTitle()		{ return m_Config.Global.Title.c_str();		};
	Nave::uint32	GetID()			{ return m_Config.Global.ID;				};
	Nave::uint32	GetPort()		{ return m_Config.Global.Port;				};
	Nave::uint32	GetMaxConn()	{ return m_Config.Global.MaxConn;			};
	Nave::uint32	GetLogSave()	{ return m_Config.Global.LogSave;			};
	const WCHAR*	GetVersion()	{ return m_Config.Global.Version.c_str();	};

public:
	Config(void);
	~Config(void);
};
