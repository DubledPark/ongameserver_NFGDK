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

typedef struct ASSIST
{
	Nave::uint32	Port;

	void Serialize(IXMLStreamWriter& out) const
	{
		out.Begin(L"Struct");
		out.Write(L"Port",Port);
	}

	void Deserialize(IXMLStreamReader& in) {
		in.Begin(L"Struct");
		in.Read(L"Port",Port);
	}
}ASSIST, *LPASSIST;

typedef struct WORLDMRS
{
	Nave::uint32	Port;
	Nave::uint32	MaxWorld;

	void Serialize(IXMLStreamWriter& out) const
	{
		out.Begin(L"Struct");
		out.Write(L"Port",Port);
		out.Write(L"MaxWorld",MaxWorld);
	}

	void Deserialize(IXMLStreamReader& in) {
		in.Begin(L"Struct");
		in.Read(L"Port",Port);
		in.Read(L"MaxWorld",MaxWorld);
	}
}WORLDMRS, *LPWORLDMRS;

typedef struct DATABASE
{
	Nave::String	IP;
	Nave::String	DBName;
	Nave::String	ID;
	Nave::String	PASS;

	void Serialize(IXMLStreamWriter& out) const
	{
		out.Begin(L"Struct");
		out.Write(L"IP",IP);
		out.Write(L"DBName",DBName);
		out.Write(L"ID",ID);
		out.Write(L"PASS",PASS);
	}

	void Deserialize(IXMLStreamReader& in) {
		in.Begin(L"Struct");
		in.Read(L"IP",IP);
		in.Read(L"DBName",DBName);
		in.Read(L"ID",ID);
		in.Read(L"PASS",PASS);
	}
}DATABASE, *LPDATABASE;

typedef struct CONFIG
{
	// 이팩트에는 가속이 없다.
	GLOBAL		Global;
	ASSIST		Assist;
	WORLDMRS	WorldMRS;
	DATABASE	Database;

	void Serialize(IXMLStreamWriter& out) const
	{
		out.Begin(L"Struct");
		out.Write(L"Global",Global);
		out.Write(L"Assist",Assist);
		out.Write(L"WorldMRS",WorldMRS);
		out.Write(L"Database",Database);
	}

	void Deserialize(IXMLStreamReader& in) {
		in.Begin(L"Struct");
		in.Read(L"Global",Global);
		in.Read(L"Assist",Assist);
		in.Read(L"WorldMRS",WorldMRS);
		in.Read(L"Database",Database);
	}
}CONFIG, *LPCONFIG;

class CConfig
{
	CONFIG	m_Config;

public:
	VOID	Load(WCHAR* strFile = L"Config.xml");
	VOID	Save(WCHAR* strFile = L"Config.xml");

	const WCHAR*	GetTitle()		{ return m_Config.Global.Title.c_str();		};
	Nave::uint32	GetID()			{ return m_Config.Global.ID;				};
	Nave::uint32	GetPort()		{ return m_Config.Global.Port;				};
	Nave::uint32	GetMaxConn()	{ return m_Config.Global.MaxConn;			};
	Nave::uint32	GetLogSave()	{ return m_Config.Global.LogSave;			};
	const WCHAR*	GetVersion()	{ return m_Config.Global.Version.c_str();	};

	Nave::uint32	GetAssistPort()	{ return m_Config.Assist.Port;				};

	Nave::uint32	GetMRSPort()	{ return m_Config.WorldMRS.Port;			};
	Nave::uint32	GetMRSWorld()	{ return m_Config.WorldMRS.MaxWorld;		};

	const WCHAR*	GetDBIP()		{ return m_Config.Database.IP.c_str();		};
	const WCHAR*	GetDBName()		{ return m_Config.Database.DBName.c_str();	};
	const WCHAR*	GetDBID()		{ return m_Config.Database.ID.c_str();		};
	const WCHAR*	GetDBPass()		{ return m_Config.Database.PASS.c_str();	};

public:
	CConfig(void);
	~CConfig(void);
};
