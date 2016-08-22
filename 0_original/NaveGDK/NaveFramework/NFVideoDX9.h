/** 
 *  @file		NFVideoDX9.h
 *  @brief		DirectX 9.0 ������ ��ü
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <Nave/NFSingleton.h>
#include "NFAdapter.h"
#include "NFD3DSettings.h"
#include "NFSprite.h"
#include <map>

namespace NaveFramework { 

	enum FILTER_MODE
	{
		POINT_FILTER,
		LINEAR_FILTER,	
	};

	/** 
	 *  @class        SWAPWND
	 *  @brief        ����ü�� ����ü
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	typedef struct SWAPWND												
	{	
		/// Swap chain
		LPDIRECT3DSWAPCHAIN9	pC;					
		/// Back buffer surface
		LPDIRECT3DSURFACE9		pB;					
		/// Stencil buffer surface
		LPDIRECT3DSURFACE9		pS;					

		/// Back buffer surface
		LPDIRECT3DSURFACE9		pTB;				
		/// Stencil buffer surface
		LPDIRECT3DSURFACE9		pTS;				

		/// Window Handle
		HWND					hW;					
		/// ���� ũ��
		INT						Width;				
		/// ���� ũ��
		INT						Height;				

		SWAPWND() :	pC(0), pB(0), pS(0), pTB(0), pTS(0), hW(0), Width(0), Height(0)
		{
		}

		/// ������ ����
		VOID RenderStart(LPD3DDEVICE pDevice, D3DCOLOR dcColor = D3DCOLOR_XRGB(0,0,255))
		{
			if(!pDevice)
				return;

			pDevice->GetRenderTarget(0, &pTB);
			pDevice->GetDepthStencilSurface(&pTS);

			pDevice->SetRenderTarget(0, pB);
			pDevice->SetDepthStencilSurface(pS);

			pDevice->Clear( 0L, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,  dcColor, 1.0f, 0L );
		}

		/// ������ ����
		VOID RenderEnd(LPD3DDEVICE pDevice)
		{
			if(!pDevice)
				return;

			pDevice->SetRenderTarget(0, pTB);
			pDevice->SetDepthStencilSurface(pTS);

			_RELEASE(pTB);
			_RELEASE(pTS);
		}

		/// �ø�
		VOID	Present()
		{
			if(pC)
				pC->Present(0, 0, 0, 0, 0);
		}

		/// ��ü �ʱ�ȭ
		void	Release()
		{
			_RELEASE(pC);
			_RELEASE(pB);
			_RELEASE(pS);
		}
	}*LPSWAPWND;

	/** 
	 *  @class        NFVideoDX9
	 *  @brief        DirectX 9.0 ������ ��ü
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFVideoDX9 : public Nave::NFSingleton<NFVideoDX9>
	{
	public:
		/// NFVideoDX9 ������
		NFVideoDX9(void);
		/// NFVideoDX9 �Ҹ���
		virtual ~NFVideoDX9(void);

	public:
		/// ������ �ڵ� ����
		inline HWND GetWnd() const { 	return m_hWnd; }
		/// ����ũ��
		inline int Width() const { return m_iWidth; }
		/// ����ũ��
		inline int Height()	const { return m_iHeight; }
		/// D3DDevice ���
		inline LPD3DDEVICE	Get3DDevice() const { return m_pd3dDevice; }

		/// DirectX�� �ʱ�ȭ �մϴ�.
		VOID		Release();

		/// ���� ��带 �����մϴ�. ������ <-> ��ü
		HRESULT		ChangeVideo(BOOL ChangeWindow);

		/// D3D�� ������ �մϴ�.
		HRESULT		ResizeD3D();
		/// CooperativeLevel �� �����մϴ�.
		HRESULT		TestCooperativeLevel();
		/// D3D�� �����մϴ�.
		HRESULT		Reset();

		/// Device�� Load�ߴ��� ����
		BOOL		IsLoadDevice() { return m_bDeviceLost; }

		/// D3D�� �ʱ�ȭ�մϴ�.
		HRESULT		InitVideo( HWND hWnd, int Width, int Heigth, BOOL WindowMode);
		/// D3D�� ���ϵ� â�� �ʱ�ȭ �մϴ�.
		HRESULT		InitChildVideo(HWND hWnd, int Width = 640, int Height = 480);

		/// ����ü�� �����츦 �߰��մϴ�.
		LPSWAPWND	AddSwapChain(HWND hWnd, int Width = 0, int Height = 0);
		/// ����ü�� �����츦 �����մϴ�.
		VOID		DelSwapChain(HWND hWnd);
		/// ����ü�� �����츦 ������ �մϴ�.
		HRESULT		ResizeSwapChain(HWND hWnd, int Width = 0, int Height = 0);
		
		/// D3D ����� �������մϴ�.
		HRESULT		ResizeVideo(int Width = 640, int Height = 480);

		/// Video Back Buffer�� �ʱ�ȭ �մϴ�.
		HRESULT		ClearScreen(D3DCOLOR dcColor = D3DCOLOR_XRGB(0,0,255));
		
		/// ������ ����
		LPD3DDEVICE RenderStart();
		/// ������ ����
		HRESULT		RenderEnd();
		/// �ø�
		VOID		Present();
		
		/// ȭ�鿡 ������ ������ �մϴ�.
		HRESULT		DrawLine(LPD3DXVECTOR2 pLines, int iLineSize, D3DCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255), float fLineWidth = 1.0f, BOOL bAntialias = FALSE, BOOL bGLLines = FALSE);

		/// ȭ�鿡 �ؽ��ĸ� ������ �մϴ�.
		HRESULT		BitBlt(int iX, int iY, LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, BOOL Alpha = FALSE, D3DCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255));
		/// ȭ�鿡 �ؽ��ĸ� ������ �մϴ�.
		HRESULT		BitBlt(RECT* pDestRect, LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, BOOL Alpha = FALSE, D3DCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255));

		/// �ؽ��ĸ� ������ �մϴ�.
		HRESULT		OnDraw(LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, D3DXMATRIX* World, D3DXVECTOR3* pRotationCenter, D3DXVECTOR3* pTranslation, D3DCOLOR Color);
		/// NFSprite�� ������ �մϴ�.
		HRESULT		OnDraw(NFSprite* Image, int iX = -1, int iY = -1);

	private:
		/// Internal functions to manage and render the 3D scene
		static BOOL ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
			D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat );

		/// �׷��� ī�� ���� ���.
		BOOL    FindBestWindowedMode( BOOL bRequireHAL, BOOL bRequireREF );
		/// ��ü��� �� �˻��մϴ�.
		BOOL    FindBestFullscreenMode( BOOL bRequireHAL, BOOL bRequireREF );
		/// D3D ������ �ʱ�ȭ �մϴ�.
		HRESULT InitialD3DSettings();

		/// D3D ������Ʈ ��ü ó��.
		HRESULT InitD3D();
		/// D3D ����̽� �ʱ�ȭ
		VOID	InitDevice();
		/// D3D ������Ʈ �ʱ�ȭ
		HRESULT	InitDeviceObjects();

	private:
		/// The main D3D object
		LPD3D				m_pD3D;             

		/// ��ü �ƴ����� ����.
		NFAdapter			m_d3dEnumeration;	
		/// ���� ������
		NFD3DSettings		m_d3dSettings;		

		/// ������ ����̴�.
		BOOL				m_bWindowed;		
		/// ����̽��� �н��ߴ�.
		BOOL				m_bDeviceLost;		

		/// ������ �ڵ�
		HWND				m_hWnd;				

		/// Parameters for CreateDevice/Reset
		D3DPRESENT_PARAMETERS m_d3dpp;			
		/// The D3D rendering device
		LPD3DDEVICE			m_pd3dDevice;       
		/// Caps for the device
		D3DCAPS9			m_d3dCaps;          
		/// Surface desc of the backbuffer
		D3DSURFACE_DESC		m_d3dsdBackBuffer;  

		/// 2D Sprite ��ü
		LPD3DXSPRITE		m_pSprite;
		/// ������ ��Ʈ���� 
		D3DXMATRIX			m_MatOrtho;

		// Line ��ü
		LPD3DXLINE			m_pLine;

		/// �����쿡 ��ϵ� �޴�.
		HMENU				m_hMenu;

		/// Width used to create window
		int					m_iWidth;			
		/// Height used to create window
		int					m_iHeight;			

		/// ���ϵ� �����쿡 �ٴ´�
		BOOL				m_bChild;

		/// ����ü�� ����Ʈ
		std::map<HWND, LPSWAPWND>	m_SwapList;
	};
}