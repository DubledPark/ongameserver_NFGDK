/** 
 *  @file		NFTextureManager.h
 *  @brief		�ؽ����� ������ �����ϴ� �޴���
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
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
	 *  @brief        �ؽ����� ������ �����ϴ� ����ü
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	typedef struct TEXTURENODE
	{
		/// �ؽ��� �����ε���
		DWORD			dwIndex;
		/// ���� ī��Ʈ
		int				iRefCount;
		/// �ؽ��� 
		LPD3DTEXTURE	pTexture;		
		/// �ؽ��� ����
		D3DSURFACE_DESC SurfaceDesc;
		/// ���ϸ�
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
	 *  @brief        �ؽ��ĸ� �����ϴ� �ؽ��� �޴���.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFTextureManager : public Nave::NFSingleton<NFTextureManager>
	{
	public:
		/// NFTextureManager ������
		NFTextureManager();
		/// NFTextureManager �Ҹ���
		~NFTextureManager();

	public:
		/// �ؽ��� ����Ʈ Ŭ����
		VOID			ClearTextureList();
		/// �ؽ��� ����
		VOID			ReleaseTexture(const DWORD textureIndex);

		/// �ؽ��� �˻�
		LPD3DTEXTURE	FindTexture(const DWORD textureIndex);
		/// �ؽ��� ��� �˻�
		LPTEXTURENODE	FindTextureNode(const DWORD textureIndex);
		/// ���� �̸����� �ؽ��� ��� �˻�
		LPTEXTURENODE	FindTextureFile(const WCHAR* fileName);

		/// ���Ͽ��� �ؽ��� ����
		DWORD			CreateTexture(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPD3DTEXTURE* pTextureOut);
		/// ���Ͽ��� �ؽ��� ����
		DWORD			CreateTexture(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, BOOL bUseTransparency, D3DCOLOR TransparencyColor, LPD3DTEXTURE* pTextureOut);
		/// �޸𸮿��� �ؽ��� ����
		DWORD			CreateTextureMemory(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPCVOID pSrcData, UINT SrcDataSize, LPD3DTEXTURE* pTextureOut);
		/// �޸𸮿��� �ؽ��� ����
		DWORD			CreateTextureMemory(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPCVOID pSrcData, UINT SrcDataSize, BOOL bUseTransparency, D3DCOLOR TransparencyColor, LPD3DTEXTURE* pTextureOut);

		/// �ؽ��� ����
		DWORD			CreateTexture(LPD3DDEVICE pd3dDevice, int Width, int Height, D3DFORMAT d3dFormat, LPD3DTEXTURE* pTextureOut);
		/// RENDERTARGET �ؽ��� ����
		DWORD			CreateRenderTexture(LPD3DDEVICE pd3dDevice, int Width, int Height, LPD3DTEXTURE* pTextureOut);
		/// �÷� �ؽ��� ����
		DWORD			CreateColorTexture( LPD3DDEVICE pd3dDevice, D3DCOLOR dcColor, LPD3DTEXTURE* pTextureOut);


	private:
		/// �ؽ����� �ε��� ���ϱ�
		inline DWORD	GetIndex()  
		{ 
			Nave::NFSyncLock Sync(&m_Lock);

			return ++m_dwTextureIndex;

			/// ���� �̷��� �ؾ�������.
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
		/// �ؽ��� �ε��� ī��Ʈ
		DWORD								m_dwTextureIndex;
		/// ������ ������ ����
		std::map<DWORD, TEXTURENODE>		m_mapTexture;
		/// Sync ����
		Nave::NFSync						m_Lock;
	};

}
