/** 
 *  @file		NFImage.h
 *  @brief		������Ʈ�� ��Ų�� �����ų� ������ �̹��� ��ü�� �����Ҷ� ���ȴ�.
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */
#pragma once

#include <NaveFramework/NFAdapter.h>

namespace Nave2D {

	/// ��������Ʈ ������ �����ϴ� ����ü
	typedef struct SPRITEINFO
	{
		DWORD			dwTexture;				/// �̹��� �ε���
		LPD3DTEXTURE	pTexture;				/// �̹�����
		float			fX,fY;					/// �̹����� ���۵Ǵ� X,Y��ġ
		float			fWidth,fHeight;			/// �̹����� ���μ��� ������

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
	 *  @brief        �ϳ��� 2D�̹����� �����Ѵ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFImage  
	{	
	public:
		/// NFImage ������
		NFImage();
		/// NFImage �Ҹ���
		virtual ~NFImage();

	public:
		/// ��� �ʱ�ȭ�Ѵ�.
		VOID				ReleaseAll();

		/// �ε�� �̹����� �����ɴϴ�.
		inline LPD3DTEXTURE	GetTexture() { return m_pTexture; }

		/// ��������Ʈ ������ ���ɴϴ�.
		inline INT			GetSpriteCnt() { return m_iSpriteCnt; }

		/// �ؽ��� ����ũ��
		inline float		GetWidth() { return m_fWidth; }

		/// �ؽ��� ����ũ��
		inline float		GetHeight() { return m_fHeight; }

		/// �̹����� Ű�÷��� �����մϴ�.
		VOID				SetKeyColor(COLORREF KeyColor);

		/**
		 * @brief	����ִ� �ؽ��ĸ� �����մϴ�
		 * @param iWidth	����ũ��
		 * @param iHeight	����ũ��
		 * @param d3dFormat	�ؽ��� ����
		 * @return  ��������
		 */
		BOOL				CreateEmptyTexture(int iWidth, int iHeight, D3DFORMAT d3dFormat);

		/**
		 * @brief	�ؽ��ĸ� ä��ϴ�.
		 * @param pByte	ä�� ����
		 */
		VOID				FillTexture(BYTE* pByte);

		/**
		 * @brief	16��Ʈ �ؽ��ĸ�  ä��ϴ�.
		 * @param iWidth	����ũ��
		 * @param iHeight	����ũ��
		 * @param pSrc		ä�� �ҽ�
		 */
		VOID				SetBitTexture16(int iWidth, int iHeight, WORD* pSrc);

		/**
		 * @brief	32��Ʈ �ؽ��ĸ�  ä��ϴ�.
		 * @param iWidth	����ũ��
		 * @param iHeight	����ũ��
		 * @param pSrc		ä�� �ҽ�
		 */
		VOID				SetBitTexture32(int iWidth, int iHeight, DWORD* pSrc);

		/**
		 * @brief	��������Ʈ�� �����մϴ�.
		 * @param iMaxImageArea ��������Ʈ ����
		 * @return	��������
		 */
		BOOL				CreateImageArea(int iMaxImageArea);

		/**
		 * @brief	��������Ʈ ������ �����մϴ�.
		 * @param Index		�ε���
		 * @param X			X
		 * @param Y			Y
		 * @param Width		����ũ��
		 * @param Height	����ũ��
		 * @return	��������
		 */
		BOOL				SetImageArea(int Index, float X, float Y, float Width, float Height);

		/**
		 * @brief	�ؽ��ĸ� �ε��մϴ�.
		 * @param pDevice		D3D����̽�
		 * @param ImageFile		�̹�������
		 * @return	��������
		 */
		BOOL				LoadResourceFile(LPD3DDEVICE pDevice, const WCHAR* ImageFile);
		/**
		 * @brief	�ؽ��ĸ� �ε��մϴ�.
		 * @param pDevice		D3D����̽�
		 * @param TextureID		�ؽ����νľ��̵�
		 * @param FilePoint		�ؽ��� ������
		 * @param FileLen		�ؽ��� ũ��
		 * @return	��������
		 */
		BOOL				LoadResourceMemory(LPD3DDEVICE pDevice, const WCHAR* TextureID, char* FilePoint, DWORD FileLen);

		/**
		 * @brief	�̹����� �ε��մϴ�.
		 * @param pDevice		D3D����̽�
		 * @param ImageFile		�ؽ��� ����
		 * @return	��������
		 */
		BOOL				LoadFile(LPD3DDEVICE pDevice, const WCHAR* ImageFile);

		/**
		 * @brief	�̹����� �ε��մϴ�.
		 * @param pDevice		D3����̽�
		 * @param TextureID		�ؽ����νľ��̵�
		 * @param FilePoint		�ؽ��� ������
		 * @param FileLen		�ؽ��� ũ��
		 * @return 
		 */
		BOOL				LoadMemory(LPD3DDEVICE pDevice, const WCHAR* TextureID, char* FilePoint, DWORD FileLen);

		/// �⺻ ��ũ��Ʈ ������ �����մϴ�.
		void				SetDefaultSpriteInfo();

		/**
		 * @brief	��������Ʈ Rect�� ���մϴ�.
		 * @param index �ε���
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
		 * @brief	��������Ʈ ���� ���ɴϴ�.
		 * @param index	�ε���
		 * @return	��������Ʈ
		 */
		inline LPSPRITEINFO	GetImageStruct(int index)
		{
			if(m_pSpriteInfo == NULL) return &m_stDefault;
			if(index >= m_iSpriteCnt ) return &m_stDefault;
			
			return &m_pSpriteInfo[index];
		}

	private:
		/// �ؽ��� �������̵�
		DWORD				m_dwTexture;
		/// �ؽ��� ������
		LPD3DTEXTURE		m_pTexture;

		/// ��������Ʈ ����
		INT					m_iSpriteCnt;	
		/// ��������Ʈ ��ü
		LPSPRITEINFO		m_pSpriteInfo;   
		/// �⺻ ��������Ʈ ����
		SPRITEINFO			m_stDefault;

		/// �ؽ��� ����ũ��
		float				m_fWidth;
		/// �ؽ��� ����ũ��
		float				m_fHeight;

		/// Ű�÷� ����
		BOOL				m_bKeyColor;
		/// Ű�÷�
		D3DCOLOR			m_dwKeyColor;
	};

}