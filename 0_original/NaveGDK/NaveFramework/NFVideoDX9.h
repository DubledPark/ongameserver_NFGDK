/** 
 *  @file		NFVideoDX9.h
 *  @brief		DirectX 9.0 렌더링 객체
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
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
	 *  @brief        스왑체인 구조체
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
		/// 가로 크기
		INT						Width;				
		/// 세로 크기
		INT						Height;				

		SWAPWND() :	pC(0), pB(0), pS(0), pTB(0), pTS(0), hW(0), Width(0), Height(0)
		{
		}

		/// 렌더를 시작
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

		/// 렌더를 종료
		VOID RenderEnd(LPD3DDEVICE pDevice)
		{
			if(!pDevice)
				return;

			pDevice->SetRenderTarget(0, pTB);
			pDevice->SetDepthStencilSurface(pTS);

			_RELEASE(pTB);
			_RELEASE(pTS);
		}

		/// 플립
		VOID	Present()
		{
			if(pC)
				pC->Present(0, 0, 0, 0, 0);
		}

		/// 객체 초기화
		void	Release()
		{
			_RELEASE(pC);
			_RELEASE(pB);
			_RELEASE(pS);
		}
	}*LPSWAPWND;

	/** 
	 *  @class        NFVideoDX9
	 *  @brief        DirectX 9.0 렌더링 객체
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFVideoDX9 : public Nave::NFSingleton<NFVideoDX9>
	{
	public:
		/// NFVideoDX9 생성자
		NFVideoDX9(void);
		/// NFVideoDX9 소멸자
		virtual ~NFVideoDX9(void);

	public:
		/// 윈도우 핸들 리턴
		inline HWND GetWnd() const { 	return m_hWnd; }
		/// 가로크기
		inline int Width() const { return m_iWidth; }
		/// 세로크기
		inline int Height()	const { return m_iHeight; }
		/// D3DDevice 얻기
		inline LPD3DDEVICE	Get3DDevice() const { return m_pd3dDevice; }

		/// DirectX를 초기화 합니다.
		VOID		Release();

		/// 비디오 모드를 변경합니다. 윈도우 <-> 전체
		HRESULT		ChangeVideo(BOOL ChangeWindow);

		/// D3D를 재조정 합니다.
		HRESULT		ResizeD3D();
		/// CooperativeLevel 를 조절합니다.
		HRESULT		TestCooperativeLevel();
		/// D3D를 리셋합니다.
		HRESULT		Reset();

		/// Device를 Load했는지 여부
		BOOL		IsLoadDevice() { return m_bDeviceLost; }

		/// D3D를 초기화합니다.
		HRESULT		InitVideo( HWND hWnd, int Width, int Heigth, BOOL WindowMode);
		/// D3D를 차일드 창에 초기화 합니다.
		HRESULT		InitChildVideo(HWND hWnd, int Width = 640, int Height = 480);

		/// 스왑체인 윈도우를 추가합니다.
		LPSWAPWND	AddSwapChain(HWND hWnd, int Width = 0, int Height = 0);
		/// 스왑체인 윈도우를 삭제합니다.
		VOID		DelSwapChain(HWND hWnd);
		/// 스윕체인 윈도우를 재조정 합니다.
		HRESULT		ResizeSwapChain(HWND hWnd, int Width = 0, int Height = 0);
		
		/// D3D 사이즈를 재조정합니다.
		HRESULT		ResizeVideo(int Width = 640, int Height = 480);

		/// Video Back Buffer를 초기화 합니다.
		HRESULT		ClearScreen(D3DCOLOR dcColor = D3DCOLOR_XRGB(0,0,255));
		
		/// 렌더를 시작
		LPD3DDEVICE RenderStart();
		/// 렌더를 종료
		HRESULT		RenderEnd();
		/// 플립
		VOID		Present();
		
		/// 화면에 라인을 렌더링 합니다.
		HRESULT		DrawLine(LPD3DXVECTOR2 pLines, int iLineSize, D3DCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255), float fLineWidth = 1.0f, BOOL bAntialias = FALSE, BOOL bGLLines = FALSE);

		/// 화면에 텍스쳐를 렌더링 합니다.
		HRESULT		BitBlt(int iX, int iY, LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, BOOL Alpha = FALSE, D3DCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255));
		/// 화면에 텍스쳐를 렌더링 합니다.
		HRESULT		BitBlt(RECT* pDestRect, LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, BOOL Alpha = FALSE, D3DCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255));

		/// 텍스쳐를 렌더링 합니다.
		HRESULT		OnDraw(LPD3DTEXTURE pSrcTexture, RECT* pSrcRect, D3DXMATRIX* World, D3DXVECTOR3* pRotationCenter, D3DXVECTOR3* pTranslation, D3DCOLOR Color);
		/// NFSprite를 렌더링 합니다.
		HRESULT		OnDraw(NFSprite* Image, int iX = -1, int iY = -1);

	private:
		/// Internal functions to manage and render the 3D scene
		static BOOL ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
			D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat );

		/// 그래픽 카드 정보 얻기.
		BOOL    FindBestWindowedMode( BOOL bRequireHAL, BOOL bRequireREF );
		/// 전체모드 를 검색합니다.
		BOOL    FindBestFullscreenMode( BOOL bRequireHAL, BOOL bRequireREF );
		/// D3D 셋팅을 초기화 합니다.
		HRESULT InitialD3DSettings();

		/// D3D 오브젝트 객체 처리.
		HRESULT InitD3D();
		/// D3D 디바이스 초기화
		VOID	InitDevice();
		/// D3D 오브젝트 초기화
		HRESULT	InitDeviceObjects();

	private:
		/// The main D3D object
		LPD3D				m_pD3D;             

		/// 전체 아답터의 정보.
		NFAdapter			m_d3dEnumeration;	
		/// 현재 설정값
		NFD3DSettings		m_d3dSettings;		

		/// 윈도우 모드이다.
		BOOL				m_bWindowed;		
		/// 디바이스를 분실했다.
		BOOL				m_bDeviceLost;		

		/// 윈도우 핸들
		HWND				m_hWnd;				

		/// Parameters for CreateDevice/Reset
		D3DPRESENT_PARAMETERS m_d3dpp;			
		/// The D3D rendering device
		LPD3DDEVICE			m_pd3dDevice;       
		/// Caps for the device
		D3DCAPS9			m_d3dCaps;          
		/// Surface desc of the backbuffer
		D3DSURFACE_DESC		m_d3dsdBackBuffer;  

		/// 2D Sprite 객체
		LPD3DXSPRITE		m_pSprite;
		/// 렌더링 매트릭스 
		D3DXMATRIX			m_MatOrtho;

		// Line 객체
		LPD3DXLINE			m_pLine;

		/// 윈도우에 등록된 메뉴.
		HMENU				m_hMenu;

		/// Width used to create window
		int					m_iWidth;			
		/// Height used to create window
		int					m_iHeight;			

		/// 차일드 윈도우에 붙는다
		BOOL				m_bChild;

		/// 스왑체인 리스트
		std::map<HWND, LPSWAPWND>	m_SwapList;
	};
}