/** 
 *  @file		UICaret.h
 *  @brief		커서를 생성한다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "UISprite.h"

namespace Nave2D { 

	/** 
	 *  @class        UICaret
	 *  @brief        NFEditBox에서 커서를 생성한다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class UICaret : public UISprite
	{
	public:
		/// UICaret 생성자
		UICaret(void);
		/// UICaret 소멸자
		virtual ~UICaret(void);

	public:
		/// 캐럿을 생성합니다.
		virtual VOID OnCreate();
		/// 캐럿을 삭제합니다.
		virtual VOID OnDestroy();
		/// 캐럿의 활성화 상태
		virtual	BOOL IsVisible();

		/// 캐럿의 컬러를 지정합니다.
		VOID SetCaretColor(COLORREF Color);
		/// 캐럿의 사이즈를 설정합니다.
		VOID SetCaretSize(float Width, float Height);

		/// 캐럿을 선택모드로 변경합니다.
		inline void InitCaretTime() { m_CaretTick = timeGetTime(); m_CaretFlag = TRUE; }
	private:
		/// 캐럿 플래그
		BOOL    m_CaretFlag;
		/// 캐럿 갱신 타이머
		DWORD	m_CaretTick;
		/// 캐럿 텍스쳐 인덱스
		DWORD	m_dwTexture;
	};

}
