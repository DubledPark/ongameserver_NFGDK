/** 
 *  @file		NFBitmap.h
 *  @brief		��Ʈ�� �̹����� �ε��Ͽ� �����Ѵ�.
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */


#pragma once

#include <windows.h>

namespace NaveFramework {

	/** 
	 *  @class        NFBitmap
	 *  @brief        ��Ʈ�� �̹����� �ε��Ͽ� �����Ѵ�.
	 *  @remarks      
	 *  				// ����ڸ�ũ�� �ε��ϱ� ���� �۾�			\r\n
	 *  				// ������ 18x12¥���� ���					\r\n
	 *  				NFBitmap *pBit = new NFBitmap();			\r\n
	 *  				// �ε�� �̹������� �޸� ����			\r\n
	 *  				BYTE*			pTempBuff = NULL ;			\r\n
	 *																\r\n
	 *  				// �ε��ϱ�									\r\n
	 *  				pBit->Load(L"Mark.bmp", &pTempBuff);		\r\n
	 *																\r\n
	 *					// �� ������ DB�� �����Ѵ�. ���̳ʸ��̹����� DB�� ����ǰ� �ȴ�.\r\n
	 *					Game::DB::USERMARK stMark;					\r\n
	 *					stMark.Size = sizeof(stMark);				\r\n
	 *					memcpy(stMark.Data, pTempBuff, MARKDATA_LEN);\r\n
	 *																\r\n
	 *					// ȭ�鿡 ��ũ�� �����ֱ� ���� �۾�			\r\n
	 *					NFImage image;								\r\n
	 *					image.CreateEmptyTexture(32, 16, D3DFMT_R5G6B5);\r\n
	 *					image.SetBitTexture16( 18, 12, ( WORD* )stMark.Data ) ;\r\n
	 *					image.CreateImageArea(1);					\r\n
	 *					image.SetImageArea(0, 0, 0, 18, 12);		\r\n
	 *																\r\n
	 *					// ��������Ʈ�� ����						\r\n
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
		/// NFBitmap ������
		NFBitmap() ;
		/// NFBitmap �Ҹ���
		virtual ~NFBitmap() ;
		
		/// ��� ������ �ʱ�ȭ�Ѵ�.
		VOID	Clear() ;
		/**
		 * @brief	�̹��������� �ε��մϴ�.
		 * @param pFileName	���ϸ�
		 * @param pOutBits  �������
		 * @return 
		 */
		BOOL	Load( WCHAR* pFileName, BYTE** pOutBits ) ;
		/// �̹��� ���۸� �����ɴϴ�.
		WORD*	GetImgBits()		{ return m_pwBitmapPixels ; }
		/// �ε�� �޸� ���۸� �����ɴϴ�.
		BYTE*	GetByteBits()		{ return m_buffer ; }

	private:
		/// �̹����� �������ϴ�.
		void	MarkFlipBits(void * pSour, int wbytes, int Height);

	private:
		/// ��Ʈ�� �ȼ� ����
		WORD*				m_pwBitmapPixels ;
		/// Width
		INT					m_iWidth ;
		/// Height
		INT					m_iHeight ;
		/// Size
		INT					m_iSize ;
		/// �̹����� �޸� ����
		BYTE*				m_buffer ;
	} ;

}