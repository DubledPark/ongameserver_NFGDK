#include "NFKeyMap.h"

namespace NaveFramework { namespace Input {

	std::vector<INPUTMAP>	g_KeyList;

	INPUTSTRING	g_KeyState[MAX_EKS]  = 
	{
		{EKS_NONE			,		L"EKS_NONE"},
		{EKS_DOWN			,		L"EKS_DOWN"},
		{EKS_PRESS			,		L"EKS_PRESS"},
		{EKS_UP				,		L"EKS_UP"},
	};

	INPUTSTRING	g_KeyName[MAX_KEY]  = 
	{
		{KEY_NONE			,		L"KEY_NONE"},
		{KEY_ESCAPE			,		L"KEY_ESCAPE"},
		{KEY_1				,		L"KEY_1"},
		{KEY_2				,		L"KEY_2"},
		{KEY_3				,		L"KEY_3"},
		{KEY_4				,		L"KEY_4"},
		{KEY_5				,		L"KEY_5"},
		{KEY_6				,		L"KEY_6"},
		{KEY_7				,		L"KEY_7"},
		{KEY_8				,		L"KEY_8"},
		{KEY_9				,		L"KEY_9"},
		{KEY_0				,		L"KEY_0"},
		{KEY_MINUS			,		L"KEY_MINUS"},
		{KEY_EQUALS			,		L"KEY_EQUALS"},
		{KEY_BACK			,		L"KEY_BACK"},
		{KEY_TAB			,		L"KEY_TAB"},
		{KEY_Q				,		L"KEY_Q"},
		{KEY_W				,		L"KEY_W"},
		{KEY_E				,		L"KEY_E"},
		{KEY_R				,		L"KEY_R"},
		{KEY_T				,		L"KEY_T"},
		{KEY_Y				,		L"KEY_Y"},
		{KEY_U				,		L"KEY_U"},
		{KEY_I				,		L"KEY_I"},
		{KEY_O				,		L"KEY_O"},
		{KEY_P				,		L"KEY_P"},
		{KEY_LBRACKET		,		L"KEY_LBRACKET"},
		{KEY_RBRACKET  		,		L"KEY_RBRACKET"},
		{KEY_ENTER    		,		L"KEY_ENTER"},
		{KEY_LCTRL  		,		L"KEY_LCTRL"},
		{KEY_A         		,		L"KEY_A"},
		{KEY_S         		,		L"KEY_S"},
		{KEY_D         		,		L"KEY_D"},
		{KEY_F         		,		L"KEY_F"},
		{KEY_G         		,		L"KEY_G"},
		{KEY_H         		,		L"KEY_H"},
		{KEY_J         		,		L"KEY_J"},
		{KEY_K         		,		L"KEY_K"},
		{KEY_L         		,		L"KEY_L"},
		{KEY_SEMICOLON 		,		L"KEY_SEMICOLON"},
		{KEY_APOSTROPHE		,		L"KEY_APOSTROPHE"},
		{KEY_GRAVE     		,		L"KEY_GRAVE"},
		{KEY_LSHIFT    		,		L"KEY_LSHIFT"},
		{KEY_BACKSLASH 		,		L"KEY_BACKSLASH"},
		{KEY_Z         		,		L"KEY_Z"},
		{KEY_X         		,		L"KEY_X"},
		{KEY_C         		,		L"KEY_C"},
		{KEY_V         		,		L"KEY_V"},
		{KEY_B         		,		L"KEY_B"},
		{KEY_N         		,		L"KEY_N"},
		{KEY_M         		,		L"KEY_M"},
		{KEY_COMMA     		,		L"KEY_COMMA"},
		{KEY_PERIOD    		,		L"KEY_PERIOD"},
		{KEY_SLASH     		,		L"KEY_SLASH"},
		{KEY_RSHIFT    		,		L"KEY_RSHIFT"},
		{KEY_MULTIPLY  		,		L"KEY_MULTIPLY"},
		{KEY_LALT     		,		L"KEY_LALT"},
		{KEY_SPACE     		,		L"KEY_SPACE"},
		{KEY_CAPITAL   		,		L"KEY_CAPITAL"},
		{KEY_F1        		,		L"KEY_F1"},
		{KEY_F2        		,		L"KEY_F2"},
		{KEY_F3        		,		L"KEY_F3"},
		{KEY_F4        		,		L"KEY_F4"},
		{KEY_F5        		,		L"KEY_F5"},
		{KEY_F6        		,		L"KEY_F6"},
		{KEY_F7        		,		L"KEY_F7"},
		{KEY_F8        		,		L"KEY_F8"},
		{KEY_F9        		,		L"KEY_F9"},
		{KEY_F10       		,		L"KEY_F10"},
		{KEY_NUMLOCK   		,		L"KEY_NUMLOCK"},
		{KEY_SCROLL    		,		L"KEY_SCROLL"},
		{KEY_NUMPAD7   		,		L"KEY_NUMPAD7"},
		{KEY_NUMPAD8   		,		L"KEY_NUMPAD8"},
		{KEY_NUMPAD9   		,		L"KEY_NUMPAD9"},
		{KEY_SUBTRACT  		,		L"KEY_SUBTRACT"},
		{KEY_NUMPAD4   		,		L"KEY_NUMPAD4"},
		{KEY_NUMPAD5   		,		L"KEY_NUMPAD5"},
		{KEY_NUMPAD6   		,		L"KEY_NUMPAD6"},
		{KEY_ADD       		,		L"KEY_ADD"},
		{KEY_NUMPAD1   		,		L"KEY_NUMPAD1"},
		{KEY_NUMPAD2   		,		L"KEY_NUMPAD2"},
		{KEY_NUMPAD3   		,		L"KEY_NUMPAD3"},
		{KEY_NUMPAD0   		,		L"KEY_NUMPAD0"},
		{KEY_DECIMAL   		,		L"KEY_DECIMAL"},
		{KEY_OEM_102   		,		L"KEY_OEM_102"},
		{KEY_F11       		,		L"KEY_F11"},
		{KEY_F12       		,		L"KEY_F12"},
		{KEY_F13       		,		L"KEY_F13"},
		{KEY_F14       		,		L"KEY_F14"},
		{KEY_F15       		,		L"KEY_F15"},
		{KEY_KANA      		,		L"KEY_KANA"},
		{KEY_ABNT_C1   		,		L"KEY_ABNT_C1"},
		{KEY_CONVERT   		,		L"KEY_CONVERT"},
		{KEY_NOCONVERT 		,		L"KEY_NOCONVERT"},
		{KEY_YEN       		,		L"KEY_YEN"},
		{KEY_ABNT_C2   		,		L"KEY_ABNT_C2"},
		{KEY_NUMPADEQUALS	,		L"KEY_NUMPADEQUALS"},
		{KEY_PREVTRACK 		,		L"KEY_PREVTRACK"},
		{KEY_AT        		,		L"KEY_AT"},
		{KEY_COLON     		,		L"KEY_COLON"},
		{KEY_UNDERLINE 		,		L"KEY_UNDERLINE"},
		{KEY_KANJI     		,		L"KEY_KANJI"},
		{KEY_STOP      		,		L"KEY_STOP"},
		{KEY_AX        		,		L"KEY_AX"},
		{KEY_UNLABELED 		,		L"KEY_UNLABELED"},
		{KEY_NEXTTRACK      ,		L"KEY_NEXTTRACK"},
		{KEY_NUMPADENTER    ,		L"KEY_NUMPADENTER"},
		{KEY_RCTRL			,		L"KEY_RCTRL"},
		{KEY_MUTE           ,		L"KEY_MUTE"},
		{KEY_CALCULATOR     ,		L"KEY_CALCULATOR"},
		{KEY_PLAYPAUSE      ,		L"KEY_PLAYPAUSE"},
		{KEY_MEDIASTOP      ,		L"KEY_MEDIASTOP"},
		{KEY_VOLUMEDOWN     ,		L"KEY_VOLUMEDOWN"},
		{KEY_VOLUMEUP       ,		L"KEY_VOLUMEUP"},
		{KEY_WEBHOME        ,		L"KEY_WEBHOME"},
		{KEY_NUMPADCOMMA    ,		L"KEY_NUMPADCOMMA"},
		{KEY_DIVIDE         ,		L"KEY_DIVIDE"},
		{KEY_SYSRQ          ,		L"KEY_SYSRQ"},
		{KEY_RALT	        ,		L"KEY_RALT"},
		{KEY_PAUSE          ,		L"KEY_PAUSE"},
		{KEY_HOME           ,		L"KEY_HOME"},
		{KEY_UP             ,		L"KEY_UP"},
		{KEY_PAGEUP         ,		L"KEY_PAGEUP"},
		{KEY_LEFT           ,		L"KEY_LEFT"},
		{KEY_RIGHT          ,		L"KEY_RIGHT"},
		{KEY_END            ,		L"KEY_END"},
		{KEY_DOWN           ,		L"KEY_DOWN"},
		{KEY_PAGEDOWN       ,		L"KEY_PAGEDOWN"},
		{KEY_INSERT         ,		L"KEY_INSERT"},
		{KEY_DELETE         ,		L"KEY_DELETE"},
		{KEY_LWIN           ,		L"KEY_LWIN"},
		{KEY_RWIN           ,		L"KEY_RWIN"},
		{KEY_APPS           ,		L"KEY_APPS"},
		{KEY_POWER          ,		L"KEY_POWER"},
		{KEY_SLEEP          ,		L"KEY_SLEEP"},
		{KEY_WAKE           ,		L"KEY_WAKE"},
		{KEY_WEBSEARCH      ,		L"KEY_WEBSEARCH"},
		{KEY_WEBFAVORITES   ,		L"KEY_WEBFAVORITES"},
		{KEY_WEBREFRESH     ,		L"KEY_WEBREFRESH"},
		{KEY_WEBSTOP        ,		L"KEY_WEBSTOP"},
		{KEY_WEBFORWARD     ,		L"KEY_WEBFORWARD"},
		{KEY_WEBBACK        ,		L"KEY_WEBBACK"},
		{KEY_MYCOMPUTER     ,		L"KEY_MYCOMPUTER"},
		{KEY_MAIL           ,		L"KEY_MAIL"},
		{KEY_MEDIASELECT    ,		L"KEY_MEDIASELECT"},
	};

	const Nave::String GetKeyName(int iKey)
	{
		for (int i=0; i<MAX_KEY; ++i)
		{
			if (iKey == g_KeyName[i].iKey)
				return g_KeyName[i].szString;
		}
		return L"";
	}

	int GetKeyByName(const Nave::String& szName)
	{
		for (int i=0; i<MAX_KEY; ++i)
		{
			if(szName.compare(g_KeyName[i].szString) == 0)
				return g_KeyName[i].iKey;
		}

		return -1;
	}

	const Nave::String GetKeyStateName(int iState)
	{
		for (int i=0; i<MAX_EKS; ++i)
		{
			if (iState == g_KeyState[i].iKey)
				return g_KeyState[i].szString;
		}
		return L"";
	}

	int GetKeyByStateName(const Nave::String& szName)
	{
		for (int i=0; i<MAX_KEY; ++i)
		{
			if(szName.compare(g_KeyName[i].szString) == 0)
				return g_KeyState[i].iKey;
		}

		return -1;
	}

}}