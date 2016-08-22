/** 
 *  @file		NFBitmap.h
 *  @brief		비트맵 이미지를 로드하여 관리한다.
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */


#pragma once

#include <windows.h>

namespace NaveFramework {

	/** 
	 *  @class        NFBitmap
	 *  @brief        비트맵 이미지를 로드하여 관리한다.
	 *  @remarks      
	 *  				// 사용자마크를 로드하기 위한 작업			\r\n
	 *  				// 사이즈 18x12짜리로 사용					\r\n
	 *  				NFBitmap *pBit = new NFBitmap();			\r\n
	 *  				// 로드된 이미지가의 메모리 버퍼			\r\n
	 *  				BYTE*			pTempBuff = NULL ;			\r\n
	 *																\r\n
	 *  				// 로드하기									\r\n
	 *  				pBit->Load(L"Mark.bmp", &pTempBuff);		\r\n
	 *																\r\n
	 *					// 이 정보를 DB로 전달한다. 바이너리이미지는 DB에 저장되게 된다.\r\n
	 *					Game::DB::USERMARK stMark;					\r\n
	 *					stMark.Size = sizeof(stMark);				\r\n
	 *					memcpy(stMark.Data, pTempBuff, MARKDATA_LEN);\r\n
	 *																\r\n
	 *					// 화면에 마크를 보여주기 위한 작업			\r\n
	 *					NFImage image;								\r\n
	 *					image.CreateEmptyTexture(32, 16, D3DFMT_R5G6B5);\r\n
	 *					image.SetBitTexture16( 18, 12, ( WORD* )stMark.Data ) ;\r\n
	 *					image.CreateImageArea(1);					\r\n
	 *					image.SetImageArea(0, 0, 0, 18, 12);		\r\n
	 *																\r\n
	 *					// 스프라이트로 연결						\r\n
	 *					NFSprite sprite;							\r\n
	 *					sprite.OnCreate(32, 16);					\r\n
	 *					sprite.SetImage(image.GetImageStruct(0));	\r\n
	 *																\r\n
	 *				    _DELETE(pBit);								\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFBitmap
	{
	public:
		/// NFBitmap 생성자
		NFBitmap() ;
		/// NFBitmap 소멸자
		virtual ~NFBitmap() ;
		
		/// 모든 정보를 초기화한다.
		VOID	Clear() ;
		/**
		 * @brief	이미지파일을 로드합니다.
		 * @param pFileName	파일명
		 * @param pOutBits  저장버퍼
		 * @return 
		 */
		BOOL	Load( WCHAR* pFileName, BYTE** pOutBits ) ;
		/// 이미지 버퍼를 가져옵니다.
		WORD*	GetImgBits()		{ return m_pwBitmapPixels ; }
		/// 로드된 메모리 버퍼를 가져옵니다.
		BYTE*	GetByteBits()		{ return m_buffer ; }

	private:
		/// 이미지를 뒤집습니다.
		void	MarkFlipBits(void * pSour, int wbytes, int Height);

	private:
		/// 비트맵 픽셀 버퍼
		WORD*				m_pwBitmapPixels ;
		/// Width
		INT					m_iWidth ;
		/// Height
		INT					m_iHeight ;
		/// Size
		INT					m_iSize ;
		/// 이미지의 메모리 버퍼
		BYTE*				m_buffer ;
	} ;

}