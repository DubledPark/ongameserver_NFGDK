/** 
 *  @file		NFTextureManager.h
 *  @brief		텍스쳐의 정보를 관리하는 메니저
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <map>
#include <Nave/NFSingleton.h>
#include <Nave/NFSync.h>
#include <NaveFramework/NFAdapter.h>

namespace Nave2D {

	/** 
	 *  @class        TEXTURENODE
	 *  @brief        텍스쳐의 정보를 관리하는 구조체
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	typedef struct TEXTURENODE
	{
		/// 텍스쳐 고유인덱스
		DWORD			dwIndex;
		/// 참조 카운트
		int				iRefCount;
		/// 텍스쳐 
		LPD3DTEXTURE	pTexture;		
		/// 텍스쳐 정보
		D3DSURFACE_DESC SurfaceDesc;
		/// 파일명
		WCHAR			szFileName[128];

		TEXTURENODE()
		{
			dwIndex = 0;
			iRefCount = 0;
			pTexture = NULL;
			ZeroMemory(&SurfaceDesc, sizeof(SurfaceDesc));
			wcscpy(szFileName, L"");
		}

		~TEXTURENODE()
		{
			dwIndex = 0;
			iRefCount = 0;
			_RELEASE(pTexture);
		}

		void Init(DWORD index, const WCHAR* fileName, LPD3DTEXTURE texture)
		{
			dwIndex = index;
			iRefCount = 1;
			pTexture = texture;

			pTexture->GetLevelDesc( 0, &SurfaceDesc );

			wcscpy(szFileName, fileName);
		}

		inline UINT GetWidth()
		{
			return SurfaceDesc.Width;
		}

		inline UINT GetHeight()
		{
			return SurfaceDesc.Height;
		}

		bool Compare(const WCHAR* fileName)
		{
			if(wcsicmp(szFileName, fileName) == 0)
				return true;

			return false;
		}

	}*LPTEXTURENODE;

	/** 
	 *  @class        NFTextureManager
	 *  @brief        텍스쳐를 관리하는 텍스쳐 메니져.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFTextureManager : public Nave::NFSingleton<NFTextureManager>
	{
	public:
		/// NFTextureManager 생성자
		NFTextureManager();
		/// NFTextureManager 소멸자
		~NFTextureManager();

	public:
		/// 텍스쳐 리스트 클리어
		VOID			ClearTextureList();
		/// 텍스쳐 삭제
		VOID			ReleaseTexture(const DWORD textureIndex);

		/// 텍스쳐 검색
		LPD3DTEXTURE	FindTexture(const DWORD textureIndex);
		/// 텍스쳐 노드 검색
		LPTEXTURENODE	FindTextureNode(const DWORD textureIndex);
		/// 파일 이름으로 텍스쳐 노드 검색
		LPTEXTURENODE	FindTextureFile(const WCHAR* fileName);

		/// 파일에서 텍스쳐 생성
		DWORD			CreateTexture(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPD3DTEXTURE* pTextureOut);
		/// 파일에서 텍스쳐 생성
		DWORD			CreateTexture(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, BOOL bUseTransparency, D3DCOLOR TransparencyColor, LPD3DTEXTURE* pTextureOut);
		/// 메모리에서 텍스쳐 생성
		DWORD			CreateTextureMemory(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPCVOID pSrcData, UINT SrcDataSize, LPD3DTEXTURE* pTextureOut);
		/// 메모리에서 텍스쳐 생성
		DWORD			CreateTextureMemory(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPCVOID pSrcData, UINT SrcDataSize, BOOL bUseTransparency, D3DCOLOR TransparencyColor, LPD3DTEXTURE* pTextureOut);

		/// 텍스쳐 생성
		DWORD			CreateTexture(LPD3DDEVICE pd3dDevice, int Width, int Height, D3DFORMAT d3dFormat, LPD3DTEXTURE* pTextureOut);
		/// RENDERTARGET 텍스쳐 생성
		DWORD			CreateRenderTexture(LPD3DDEVICE pd3dDevice, int Width, int Height, LPD3DTEXTURE* pTextureOut);
		/// 컬러 텍스쳐 생성
		DWORD			CreateColorTexture( LPD3DDEVICE pd3dDevice, D3DCOLOR dcColor, LPD3DTEXTURE* pTextureOut);


	private:
		/// 텍스쳐의 인덱스 구하기
		inline DWORD	GetIndex()  
		{ 
			Nave::NFSyncLock Sync(&m_Lock);

			return ++m_dwTextureIndex;

			/// 추후 이렇게 해야할지도.
			DWORD dwID = 0;
			std::map<DWORD, TEXTURENODE>::iterator obj;
			std::map<DWORD, TEXTURENODE>::iterator end = m_mapTexture.end();
			while(true)
			{
				dwID = ++m_dwTextureIndex;

				obj = m_mapTexture.find(dwID);
				if(obj == end)
					return dwID;
			}
			return dwID; 
		}

	private:
		/// 텍스쳐 인덱스 카운트
		DWORD								m_dwTextureIndex;
		/// 텍즈쳐 정보가 저장
		std::map<DWORD, TEXTURENODE>		m_mapTexture;
		/// Sync 정보
		Nave::NFSync						m_Lock;
	};

}
