/** 
 *  @file		NFSprite.h
 *  @brief		ȭ�鿡 �̹����� ������ �ɱ����� ��ü
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include "NFAdapter.h"

namespace NaveFramework { 

	/** 
	 *  @class        NFSprite
	 *  @brief        ȭ�鿡 �̹����� ������ �ɱ����� ��ü
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFSprite
	{
	public:
		/// ���� ���
		enum BLEND
		{
			BLEND_NONE,				/// �׳� ���, D3DBLEND_ONE, D3DBLEND_ZERO
			// ���İ迭
			BLEND_ALPHA,			/// ���Ĺ�����, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA
			BLEND_ADD,				/// ����, D3DBLEND_ONE, D3DBLEND_ONE
			BLEND_TRANSTOWHITEOUT,	/// �������갰�� �Ǹ鼭 ����->ȭ��Ʈ�ƿ� ȿ��. D3DBLEND_SRCALPHA, D3DBLEND_DESTALPHA
			BLEND_SPRBRIGHT,		/// ��������Ʈ�� ��ü �� ������ �ȴ�. �����Ѱ�? D3DBLEND_SRCALPHA, D3DBLEND_INVDESTALPHA
			BLEND_LIGHTSRC,			/// �������� ���� D3DBLEND_SRCALPHA + D3DBLEND_ONE
			// �÷� �迭
			BLEND_COLORINV,			/// ����� �ƴҼ��� �ػ��� ���ĺ���. ���� �����ż� ���������ޱ�... �������� �ȳ���. D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR
			BLEND_SCREEN,			/// �� ��κ��� ��������.. ������������.. �������϶� 202�� ���
			BLEND_HARDLIGHT,		/// ����̸� Dodge �������̸� Burn
			BLEND_INVERSE			/// �����Ѵ�. ����̸� �� ��Ű��� �������̸� �� ���� �� ����.
		};

	public:
		/// NFSprite ������
		NFSprite(void);
		/// NFSprite �Ҹ���
		virtual ~NFSprite(void);

	public:
		/// ���� ��� ����
		void SetBlendMode(BLEND BlendMode);

		/// ��������Ʈ�� �����Ѵ�.
		virtual VOID	OnCreate( float fWidth = 32, float fHeight = 32);
		/// ��������Ʈ�� �����Ѵ�.
		virtual VOID	OnDestroy();

		/// �̹����� �����Ѵ�.
		VOID	SetImage(LPD3DTEXTURE pTexture, float fSX, float fSY, float fWidth, float fHeight);
	    	
		/// ��ġ�� �����Ѵ�.
		VOID			SetPos(float fX, float fY);
		/// ������Ʈ ����� Ű���.
		virtual VOID	SetSize(float fWidth, float fHeight);	
		/// �÷��� �����Ѵ�.
		VOID			SetColor(COLORREF Color);

		/// ������ ���´�.
		inline float	GetOpacity()						{ return m_fOpacity; }
		/// ������ �����Ѵ�.
		VOID			SetOpacity( float fOpacity );

		/// ������Ʈ�� Z������ ȸ����Ų��.
		inline  void	SetRotateZ( float dvRotateZ )		{ m_RotaionZ = dvRotateZ; }
		/// ������Ʈ ȸ������ ���´�.
		inline  float	GetRotateZ()						{ return m_RotaionZ; }

		/// ������ ������ �����Ѵ�.
		inline	void	SetSrcRect( RECT* rect )			{ m_BltSrcRect = *rect; }
		
		/// �̹����� Ȱ��ȭ ����
		virtual	BOOL	IsVisible()							{ return m_IsVisible; }

		/// �ؽ��� ���ϱ�
		inline	LPD3DTEXTURE	GetTexture()				{ return m_pTexture; }
		/// �÷� ���ϱ�
		inline	DWORD			GetColor()					{ return m_dwColor; }
		/// Src���� ��� ���ϱ�
		inline	DWORD			GetSrcBlend()				{ return m_dbSrc; }
		/// Dest���� ��� ���ϱ�
		inline	DWORD			GetDestBlend()				{ return m_dbDest; }
		/// ȸ�� �߽��� ���ϱ�
		inline	Nave::float2&	GetRotCenter()				{ return m_RotCenter; }
		/// ��� ���� ���ϱ�
		inline	RECT&			GetSrcRect()				{ return m_BltSrcRect; }
		/// ��� ��Ʈ���� ���ϱ�
		virtual VOID			GetMatrix(D3DXMATRIX& mat, float fX = -1, float fY = -1);

		/// ���� ũ��
		inline	float			GetWidth()					{ return m_fWidth; }
		/// ���� ũ��
		inline	float			GetHeight()					{ return m_fHeight; }

	protected:
		/// ��� X
		float				m_fX;
		/// ��� Y
		float				m_fY;
		/// ��� ���� ũ��
		float				m_fWidth;
		/// ��� ���� ũ��
		float				m_fHeight;

		/// ����
		float				m_fOpacity;

		/// �ؽ���
		LPD3DTEXTURE		m_pTexture;

		/// ������ ����
		RECT				m_BltSrcRect;

		/// Ȱ��ȭ ����
		BOOL				m_IsVisible;

		/// �÷�
		DWORD				m_dwColor;

		/// Scale X
		float				m_fScaleX;
		/// Scale Y
		float				m_fScaleY;

		/// Rotate Z
		float				m_RotaionZ;
		/// ȸ�� �߽�
		Nave::float2		m_RotCenter;
		/// Src���� ���
		DWORD				m_dbSrc;
		/// Dest���� ���
		DWORD				m_dbDest;		
	};

}