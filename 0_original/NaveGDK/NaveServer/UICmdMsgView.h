/** 
 *  @file  		UICmdMsgView.h
 *  @brief 		Server UIFramework���� ����ϴ� List, Info��Ʈ��
 *  @remarks 
 *  @author  	������(edith2580@gmail.com)
 *  @date  		2009-05-09
 */
#pragma once

namespace NaveServer { 
	
	class UICmdMsgView 
	{
	public:
		UICmdMsgView();
		~UICmdMsgView();

	public:
		#define INFO_HEIGHT		300
		#define ID_INFO			201
		#define ID_MSGVIEW		202

		static HWND s_hInfoWnd; // ����Ʈ
		static HWND s_hListWnd;	// ����Ʈ

		static HWND InitMsgView( HWND hParent, HFONT hFont );
		static VOID ClrarMsgView( void );

		static VOID UpdateInfo(WCHAR* strInfo);

		static VOID ResizeLog( HWND hParent );

		// ListView �Է�.
		static LRESULT OnVkeyToItem( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	};
}