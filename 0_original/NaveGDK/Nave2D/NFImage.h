/** 
 *  @file		NFImage.h
 *  @brief		오브젝트에 스킨을 입히거나 한장의 이미지 객체를 관리할때 사용된다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */
#pragma once

#include <NaveFramework/NFAdapter.h>

namespace Nave2D {

	/// 스프라이트 정보를 관리하는 구조체
	typedef struct SPRITEINFO
	{
		DWORD			dwTexture;				/// 이미지 인덱스
		LPD3DTEXTURE	pTexture;				/// 이미지가
		float			fX,fY;					/// 이미지가 시작되는 X,Y위치
		float			fWidth,fHeight;			/// 이미지의 가로세로 사이즈

		SPRITEINFO()
		{
			dwTexture = 0;
			pTexture = NULL;
			fX = fY = fWidth = fHeight = 0.0f;
		}
		~SPRITEINFO()
		{
			dwTexture = 0;
			pTexture = NULL;
			fX = fY = fWidth = fHeight = 0.0f;
		} 
	}*LPSPRITEINFO;

	/** 
	 *  @class        NFImage
	 *  @brief        하나의 2D이미지를 관리한다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFImage  
	{	
	public:
		/// NFImage 생성자
		NFImage();
		/// NFImage 소멸자
		virtual ~NFImage();

	public:
		/// 모두 초기화한다.
		VOID				ReleaseAll();

		/// 로드된 이미지를 가져옵니다.
		inline LPD3DTEXTURE	GetTexture() { return m_pTexture; }

		/// 스프라이트 개수를 얻어옵니다.
		inline INT			GetSpriteCnt() { return m_iSpriteCnt; }

		/// 텍스쳐 가로크기
		inline float		GetWidth() { return m_fWidth; }

		/// 텍스쳐 세로크기
		inline float		GetHeight() { return m_fHeight; }

		/// 이미지의 키컬러를 설정합니다.
		VOID				SetKeyColor(COLORREF KeyColor);

		/**
		 * @brief	비어있는 텍스쳐를 생성합니다
		 * @param iWidth	가로크기
		 * @param iHeight	세로크기
		 * @param d3dFormat	텍스쳐 포멧
		 * @return  성공여부
		 */
		BOOL				CreateEmptyTexture(int iWidth, int iHeight, D3DFORMAT d3dFormat);

		/**
		 * @brief	텍스쳐를 채웁니다.
		 * @param pByte	채울 내용
		 */
		VOID				FillTexture(BYTE* pByte);

		/**
		 * @brief	16비트 텍스쳐를  채웁니다.
		 * @param iWidth	가로크기
		 * @param iHeight	세로크기
		 * @param pSrc		채울 소스
		 */
		VOID				SetBitTexture16(int iWidth, int iHeight, WORD* pSrc);

		/**
		 * @brief	32비트 텍스쳐를  채웁니다.
		 * @param iWidth	가로크기
		 * @param iHeight	세로크기
		 * @param pSrc		채울 소스
		 */
		VOID				SetBitTexture32(int iWidth, int iHeight, DWORD* pSrc);

		/**
		 * @brief	스프라이트를 생성합니다.
		 * @param iMaxImageArea 스프라이트 개수
		 * @return	성공여부
		 */
		BOOL				CreateImageArea(int iMaxImageArea);

		/**
		 * @brief	스프라이트 내용을 설정합니다.
		 * @param Index		인덱스
		 * @param X			X
		 * @param Y			Y
		 * @param Width		가로크기
		 * @param Height	세로크기
		 * @return	성공여부
		 */
		BOOL				SetImageArea(int Index, float X, float Y, float Width, float Height);

		/**
		 * @brief	텍스쳐를 로드합니다.
		 * @param pDevice		D3D디바이스
		 * @param ImageFile		이미지파일
		 * @return	성공여부
		 */
		BOOL				LoadResourceFile(LPD3DDEVICE pDevice, const WCHAR* ImageFile);
		/**
		 * @brief	텍스쳐를 로드합니다.
		 * @param pDevice		D3D디바이스
		 * @param TextureID		텍스쳐인식아이디
		 * @param FilePoint		텍스쳐 포인터
		 * @param FileLen		텍스쳐 크기
		 * @return	성공여부
		 */
		BOOL				LoadResourceMemory(LPD3DDEVICE pDevice, const WCHAR* TextureID, char* FilePoint, DWORD FileLen);

		/**
		 * @brief	이미지를 로드합니다.
		 * @param pDevice		D3D디바이스
		 * @param ImageFile		텍스쳐 파일
		 * @return	성공여부
		 */
		BOOL				LoadFile(LPD3DDEVICE pDevice, const WCHAR* ImageFile);

		/**
		 * @brief	이미지를 로드합니다.
		 * @param pDevice		D3디바이스
		 * @param TextureID		텍스쳐인식아이디
		 * @param FilePoint		텍스쳐 포인터
		 * @param FileLen		텍스쳐 크기
		 * @return 
		 */
		BOOL				LoadMemory(LPD3DDEVICE pDevice, const WCHAR* TextureID, char* FilePoint, DWORD FileLen);

		/// 기본 스크립트 정보를 설정합니다.
		void				SetDefaultSpriteInfo();

		/**
		 * @brief	스프라이트 Rect를 구합니다.
		 * @param index 인덱스
		 * @return	Rect
		 */
		inline RECT			GetImageRect(int index)
		{
			RECT Rect = { 0, 0, (int)m_fWidth, (int)m_fHeight };
			if(m_pSpriteInfo == NULL) return Rect;
			if(index >= m_iSpriteCnt ) return Rect;

			LPSPRITEINFO p = &m_pSpriteInfo[index];
			SetRect(&Rect, (int)p->fX, (int)p->fY, (int)(p->fX+p->fWidth), (int)(p->fY+p->fHeight));
			return Rect;
		}

		/**
		 * @brief	스프라이트 값을 얻어옵니다.
		 * @param index	인덱스
		 * @return	스프라이트
		 */
		inline LPSPRITEINFO	GetImageStruct(int index)
		{
			if(m_pSpriteInfo == NULL) return &m_stDefault;
			if(index >= m_iSpriteCnt ) return &m_stDefault;
			
			return &m_pSpriteInfo[index];
		}

	private:
		/// 텍스쳐 고유아이디
		DWORD				m_dwTexture;
		/// 텍스쳐 포인터
		LPD3DTEXTURE		m_pTexture;

		/// 스프라이트 개수
		INT					m_iSpriteCnt;	
		/// 스프라이트 객체
		LPSPRITEINFO		m_pSpriteInfo;   
		/// 기본 스프라이트 정보
		SPRITEINFO			m_stDefault;

		/// 텍스쳐 가로크기
		float				m_fWidth;
		/// 텍스쳐 세로크기
		float				m_fHeight;

		/// 키컬러 설정
		BOOL				m_bKeyColor;
		/// 키컬러
		D3DCOLOR			m_dwKeyColor;
	};

}