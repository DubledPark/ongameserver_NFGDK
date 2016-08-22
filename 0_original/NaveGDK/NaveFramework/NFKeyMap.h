/** 
 *  @file		NFKeyMap.h
 *  @brief		DirectInput에서 사용할 Key 정보
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <Nave/Nave.h>
#include <dinput.h>
#include <dinputd.h>	// joystick
#include <vector>

#include <Nave/NFXML.h>

//dxguid.lib dxerr9.lib dinput8.lib comctl32.lib
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

namespace NaveFramework { namespace Input {

	#define SAMPLE_BUFFER_SIZE	8  // arbitrary number of buffer elements
	#define MAX_KEY				256
	#define WM_INPUTEVENT		(WM_USER+800)

	#define EKS_NONE			0x00
	#define EKS_DOWN			0x01
	#define EKS_PRESS			0x02
	#define EKS_UP				0x03
	#define MAX_EKS				0x04

	const Nave::String	GetKeyName(int iKey);
	int					GetKeyByName(const Nave::String& szName);
	const Nave::String	GetKeyStateName(int iState);
	int					GetKeyByStateName(const Nave::String& szName);

	typedef struct INPUTMAP
	{
		Nave::String szCmd;			// Map의 문자열
		Nave::int32 iKey;			// Map에서 사용될 Key
		Nave::int32 iModKey;		// Key와 함께 사용될 확장Key
		Nave::int32 iState;		// iKey에서 사용될 State 상태 (Down, Up, Press)
		Nave::int32 iEvent;		// 이 이벤트가 발생했을때 전달될 Event ID	(wParam)
		float		 fParam;		// EventID와 함께 전달될 Param 값.			(lParam)
		Nave::int32 iFlag;			// 이벤트가 발생되면 0->1로 바뀜

		void Serialize(Nave::IXMLStreamWriter& out) const
		{
			out.Begin(L"Input");
			out.Write(L"Command",szCmd);
			out.Write(L"Key",GetKeyName(iKey));
			out.Write(L"ModKey",GetKeyName(iModKey));
			out.Write(L"State",GetKeyStateName(iState));
			out.Write(L"Event",iEvent);
			out.Write(L"Param",fParam);
		}

		void Deserialize(Nave::IXMLStreamReader& in) 
		{
			in.Begin(L"Input");
			in.Read(L"Command",szCmd);
			
			Nave::String key, modKey, state;
			
			in.Read(L"Key",key);
			in.Read(L"ModKey",modKey);
			in.Read(L"State",state);

			iKey = GetKeyByName(key);
			iModKey = GetKeyByName(modKey);
			iState = GetKeyByStateName(state);

			in.Read(L"Event",iEvent);
			in.Read(L"Param",fParam);
			iFlag = 0;
		}
	}*LPINPUTMAP;

	typedef struct INPUTSTRING
	{
		int				iKey;
		Nave::String	szString;
	}*LPINPUTSTRING;


	extern std::vector<INPUTMAP>	g_KeyList;
	extern INPUTSTRING				g_KeyState[MAX_EKS];
	extern INPUTSTRING				g_KeyName[MAX_KEY];

	#define KEY_NONE			0x00
	#define KEY_ESCAPE          0x01
	#define KEY_1               0x02
	#define KEY_2               0x03
	#define KEY_3               0x04
	#define KEY_4               0x05
	#define KEY_5               0x06
	#define KEY_6               0x07
	#define KEY_7               0x08
	#define KEY_8               0x09
	#define KEY_9               0x0A
	#define KEY_0               0x0B
	#define KEY_MINUS           0x0C    /* - on main keyboard */
	#define KEY_EQUALS          0x0D
	#define KEY_BACK            0x0E    /* backspace */
	#define KEY_TAB             0x0F
	#define KEY_Q               0x10
	#define KEY_W               0x11
	#define KEY_E               0x12
	#define KEY_R               0x13
	#define KEY_T               0x14
	#define KEY_Y               0x15
	#define KEY_U               0x16
	#define KEY_I               0x17
	#define KEY_O               0x18
	#define KEY_P               0x19
	#define KEY_LBRACKET        0x1A
	#define KEY_RBRACKET        0x1B
	#define KEY_ENTER	        0x1C    /* Enter on main keyboard */
	#define KEY_LCTRL	        0x1D
	#define KEY_A               0x1E
	#define KEY_S               0x1F
	#define KEY_D               0x20
	#define KEY_F               0x21
	#define KEY_G               0x22
	#define KEY_H               0x23
	#define KEY_J               0x24
	#define KEY_K               0x25
	#define KEY_L               0x26
	#define KEY_SEMICOLON       0x27
	#define KEY_APOSTROPHE      0x28
	#define KEY_GRAVE           0x29    /* accent grave */
	#define KEY_LSHIFT          0x2A
	#define KEY_BACKSLASH       0x2B
	#define KEY_Z               0x2C
	#define KEY_X               0x2D
	#define KEY_C               0x2E
	#define KEY_V               0x2F
	#define KEY_B               0x30
	#define KEY_N               0x31
	#define KEY_M               0x32
	#define KEY_COMMA           0x33
	#define KEY_PERIOD          0x34    /* . on main keyboard */
	#define KEY_SLASH           0x35    /* / on main keyboard */
	#define KEY_RSHIFT          0x36
	#define KEY_MULTIPLY        0x37    /* * on numeric keypad */
	#define KEY_LALT	        0x38    /* left Alt */
	#define KEY_SPACE           0x39
	#define KEY_CAPITAL         0x3A
	#define KEY_F1              0x3B
	#define KEY_F2              0x3C
	#define KEY_F3              0x3D
	#define KEY_F4              0x3E
	#define KEY_F5              0x3F
	#define KEY_F6              0x40
	#define KEY_F7              0x41
	#define KEY_F8              0x42
	#define KEY_F9              0x43
	#define KEY_F10             0x44
	#define KEY_NUMLOCK         0x45
	#define KEY_SCROLL          0x46    /* Scroll Lock */
	#define KEY_NUMPAD7         0x47
	#define KEY_NUMPAD8         0x48
	#define KEY_NUMPAD9         0x49
	#define KEY_SUBTRACT        0x4A    /* - on numeric keypad */
	#define KEY_NUMPAD4         0x4B
	#define KEY_NUMPAD5         0x4C
	#define KEY_NUMPAD6         0x4D
	#define KEY_ADD             0x4E    /* + on numeric keypad */
	#define KEY_NUMPAD1         0x4F
	#define KEY_NUMPAD2         0x50
	#define KEY_NUMPAD3         0x51
	#define KEY_NUMPAD0         0x52
	#define KEY_DECIMAL         0x53    /* . on numeric keypad */
	#define KEY_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
	#define KEY_F11             0x57
	#define KEY_F12             0x58
	#define KEY_F13             0x64    /*                     (NEC PC98) */
	#define KEY_F14             0x65    /*                     (NEC PC98) */
	#define KEY_F15             0x66    /*                     (NEC PC98) */
	#define KEY_KANA            0x70    /* (Japanese keyboard)            */
	#define KEY_ABNT_C1         0x73    /* /? on Brazilian keyboard */
	#define KEY_CONVERT         0x79    /* (Japanese keyboard)            */
	#define KEY_NOCONVERT       0x7B    /* (Japanese keyboard)            */
	#define KEY_YEN             0x7D    /* (Japanese keyboard)            */
	#define KEY_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
	#define KEY_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
	#define KEY_PREVTRACK       0x90    /* Previous Track (KEY_CIRCUMFLEX on Japanese keyboard) */
	#define KEY_AT              0x91    /*                     (NEC PC98) */
	#define KEY_COLON           0x92    /*                     (NEC PC98) */
	#define KEY_UNDERLINE       0x93    /*                     (NEC PC98) */
	#define KEY_KANJI           0x94    /* (Japanese keyboard)            */
	#define KEY_STOP            0x95    /*                     (NEC PC98) */
	#define KEY_AX              0x96    /*                     (Japan AX) */
	#define KEY_UNLABELED       0x97    /*                        (J3100) */
	#define KEY_NEXTTRACK       0x99    /* Next Track */
	#define KEY_NUMPADENTER     0x9C    /* Enter on numeric keypad */
	#define KEY_RCTRL	        0x9D
	#define KEY_MUTE            0xA0    /* Mute */
	#define KEY_CALCULATOR      0xA1    /* Calculator */
	#define KEY_PLAYPAUSE       0xA2    /* Play / Pause */
	#define KEY_MEDIASTOP       0xA4    /* Media Stop */
	#define KEY_VOLUMEDOWN      0xAE    /* Volume - */
	#define KEY_VOLUMEUP        0xB0    /* Volume + */
	#define KEY_WEBHOME         0xB2    /* Web home */
	#define KEY_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
	#define KEY_DIVIDE          0xB5    /* / on numeric keypad */
	#define KEY_SYSRQ           0xB7
	#define KEY_RALT			0xB8    /* right Alt */
	#define KEY_PAUSE           0xC5    /* Pause */
	#define KEY_HOME            0xC7    /* Home on arrow keypad */
	#define KEY_UP              0xC8    /* UpArrow on arrow keypad */
	#define KEY_PAGEUP          0xC9    /* PgUp on arrow keypad */
	#define KEY_LEFT            0xCB    /* LeftArrow on arrow keypad */
	#define KEY_RIGHT           0xCD    /* RightArrow on arrow keypad */
	#define KEY_END             0xCF    /* End on arrow keypad */
	#define KEY_DOWN            0xD0    /* DownArrow on arrow keypad */
	#define KEY_PAGEDOWN        0xD1    /* PgDn on arrow keypad */
	#define KEY_INSERT          0xD2    /* Insert on arrow keypad */
	#define KEY_DELETE          0xD3    /* Delete on arrow keypad */
	#define KEY_LWIN            0xDB    /* Left Windows key */
	#define KEY_RWIN            0xDC    /* Right Windows key */
	#define KEY_APPS            0xDD    /* AppMenu key */
	#define KEY_POWER           0xDE    /* System Power */
	#define KEY_SLEEP           0xDF    /* System Sleep */
	#define KEY_WAKE            0xE3    /* System Wake */
	#define KEY_WEBSEARCH       0xE5    /* Web Search */
	#define KEY_WEBFAVORITES    0xE6    /* Web Favorites */
	#define KEY_WEBREFRESH      0xE7    /* Web Refresh */
	#define KEY_WEBSTOP         0xE8    /* Web Stop */
	#define KEY_WEBFORWARD      0xE9    /* Web Forward */
	#define KEY_WEBBACK         0xEA    /* Web Back */
	#define KEY_MYCOMPUTER      0xEB    /* My Computer */
	#define KEY_MAIL            0xEC    /* Mail */
	#define KEY_MEDIASELECT     0xED    /* Media Select */

	#define MOUSE_NONE			0x0100	/* 마우스 */
	#define MOUSE_BUTTON00		0x0101
	#define MOUSE_BUTTON01		0x0102
	#define MOUSE_BUTTON02		0x0103
	#define MOUSE_BUTTON03		0x0104
	#define MOUSE_BUTTON04		0x0105
	#define MOUSE_BUTTON05		0x0106
	#define MOUSE_BUTTON06		0x0107
	#define MOUSE_BUTTON07		0x0108

	#define PAD_NONE			0x0200	/* 조이스틱 */
	#define PAD_BUTTON01		0x0201
	#define PAD_BUTTON02		0x0202
	#define PAD_BUTTON03		0x0203
	#define PAD_BUTTON04		0x0204
	#define PAD_BUTTON05		0x0205
	#define PAD_BUTTON06		0x0206
	#define PAD_BUTTON07		0x0207
	#define PAD_BUTTON08		0x0208
	#define PAD_BUTTON09		0x0209
	#define PAD_BUTTON10		0x020A
	#define PAD_BUTTON11		0x020B
	#define PAD_BUTTON12		0x020C
	#define PAD_BUTTON13		0x020D
	#define PAD_BUTTON14		0x020E
	#define PAD_BUTTON15		0x020F

	#define KEYMAP_END			0xFFFF


}}