/** 
 *  @file		NFSprite.h
 *  @brief		화면에 이미지를 렌더링 걸기위한 객체
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFAdapter.h"

namespace NaveFramework { 

	/** 
	 *  @class        NFSprite
	 *  @brief        화면에 이미지를 렌더링 걸기위한 객체
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFSprite
	{
	public:
		/// 블랜드 모드
		enum BLEND
		{
			BLEND_NONE,				/// 그냥 찍기, D3DBLEND_ONE, D3DBLEND_ZERO
			// 알파계열
			BLEND_ALPHA,			/// 알파반투명, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA
			BLEND_ADD,				/// 덧셈, D3DBLEND_ONE, D3DBLEND_ONE
			BLEND_TRANSTOWHITEOUT,	/// 덧셈연산같이 되면서 투명->화이트아웃 효과. D3DBLEND_SRCALPHA, D3DBLEND_DESTALPHA
			BLEND_SPRBRIGHT,		/// 스프라이트의 자체 명도 조절이 된다. 유용한가? D3DBLEND_SRCALPHA, D3DBLEND_INVDESTALPHA
			BLEND_LIGHTSRC,			/// 광원으로 쓰일 D3DBLEND_SRCALPHA + D3DBLEND_ONE
			// 컬러 계열
			BLEND_COLORINV,			/// 흰색이 아닐수록 밑색이 비쳐보임. 색이 반전돼서 덧씌워진달까... 검은색은 안나옴. D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR
			BLEND_SCREEN,			/// 덜 흰부분이 덧씌워짐.. 셀로판지랄까.. 검은색일땐 202랑 비슷
			BLEND_HARDLIGHT,		/// 흰색이면 Dodge 검은색이면 Burn
			BLEND_INVERSE			/// 반전한다. 흰색이면 좀 흰거같고 검정색이면 좀 검은 것 같다.
		};

	public:
		/// NFSprite 생성자
		NFSprite(void);
		/// NFSprite 소멸자
		virtual ~NFSprite(void);

	public:
		/// 블랜드 모드 지정
		void SetBlendMode(BLEND BlendMode);

		/// 스프라이트를 생성한다.
		virtual VOID	OnCreate( float fWidth = 32, float fHeight = 32);
		/// 스프라이트를 삭제한다.
		virtual VOID	OnDestroy();

		/// 이미지를 연결한다.
		VOID	SetImage(LPD3DTEXTURE pTexture, float fSX, float fSY, float fWidth, float fHeight);
	    	
		/// 위치를 지정한다.
		VOID			SetPos(float fX, float fY);
		/// 오브젝트 사이즈를 키운다.
		virtual VOID	SetSize(float fWidth, float fHeight);	
		/// 컬러를 지정한다.
		VOID			SetColor(COLORREF Color);

		/// 투명도를 얻어온다.
		inline float	GetOpacity()						{ return m_fOpacity; }
		/// 투명도를 설정한다.
		VOID			SetOpacity( float fOpacity );

		/// 오브젝트를 Z축으로 회전시킨다.
		inline  void	SetRotateZ( float dvRotateZ )		{ m_RotaionZ = dvRotateZ; }
		/// 오브젝트 회전값을 얻어온다.
		inline  float	GetRotateZ()						{ return m_RotaionZ; }

		/// 렌더랑 영역을 지정한다.
		inline	void	SetSrcRect( RECT* rect )			{ m_BltSrcRect = *rect; }
		
		/// 이미지의 활성화 상태
		virtual	BOOL	IsVisible()							{ return m_IsVisible; }

		/// 텍스쳐 구하기
		inline	LPD3DTEXTURE	GetTexture()				{ return m_pTexture; }
		/// 컬러 구하기
		inline	DWORD			GetColor()					{ return m_dwColor; }
		/// Src블랜드 모드 구하기
		inline	DWORD			GetSrcBlend()				{ return m_dbSrc; }
		/// Dest블랜드 모드 구하기
		inline	DWORD			GetDestBlend()				{ return m_dbDest; }
		/// 회전 중심점 구하기
		inline	Nave::float2&	GetRotCenter()				{ return m_RotCenter; }
		/// 출력 영역 구하기
		inline	RECT&			GetSrcRect()				{ return m_BltSrcRect; }
		/// 출력 매트릭스 구하기
		virtual VOID			GetMatrix(D3DXMATRIX& mat, float fX = -1, float fY = -1);

		/// 가로 크기
		inline	float			GetWidth()					{ return m_fWidth; }
		/// 세로 크기
		inline	float			GetHeight()					{ return m_fHeight; }

	protected:
		/// 출력 X
		float				m_fX;
		/// 출력 Y
		float				m_fY;
		/// 출력 가로 크기
		float				m_fWidth;
		/// 출력 세로 크기
		float				m_fHeight;

		/// 투명도
		float				m_fOpacity;

		/// 텍스쳐
		LPD3DTEXTURE		m_pTexture;

		/// 렌더링 영역
		RECT				m_BltSrcRect;

		/// 활성화 상태
		BOOL				m_IsVisible;

		/// 컬러
		DWORD				m_dwColor;

		/// Scale X
		float				m_fScaleX;
		/// Scale Y
		float				m_fScaleY;

		/// Rotate Z
		float				m_RotaionZ;
		/// 회전 중심
		Nave::float2		m_RotCenter;
		/// Src블랜드 모드
		DWORD				m_dbSrc;
		/// Dest블랜드 모드
		DWORD				m_dbDest;		
	};

}