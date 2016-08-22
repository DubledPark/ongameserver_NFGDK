#include <NaveFramework/NaveFramework.h>
#include <NaveFramework/NFAdapter.h>

#include "NFTextureManager.h"
#include "NFTextureLoader.h"

namespace Nave2D {

	NFTextureManager::NFTextureManager() : m_dwTextureIndex(0)
	{
	}

	NFTextureManager::~NFTextureManager()
	{
		ClearTextureList();
	}

	VOID NFTextureManager::ClearTextureList()
	{
		m_mapTexture.clear();
	}

	VOID NFTextureManager::ReleaseTexture(const DWORD textureIndex)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		if(m_mapTexture.empty() || textureIndex == 0xffffffff)
			return;

		std::map<DWORD, TEXTURENODE>::iterator obj = m_mapTexture.find(textureIndex);
		if(obj != m_mapTexture.end())
		{
			--obj->second.iRefCount;
			if(obj->second.iRefCount <= 0)
			{
				m_mapTexture.erase(obj);
				return;
			}
		}
	}
   
	LPD3DTEXTURE NFTextureManager::FindTexture(const DWORD textureIndex)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		std::map<DWORD, TEXTURENODE>::iterator obj = m_mapTexture.find(textureIndex);

		if(obj != m_mapTexture.end())
			return obj->second.pTexture;

		return NULL;
	}

	LPTEXTURENODE NFTextureManager::FindTextureNode(const DWORD textureIndex)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		std::map<DWORD, TEXTURENODE>::iterator obj = m_mapTexture.find(textureIndex);

		if(obj != m_mapTexture.end())
			return &obj->second;

		return NULL;
	}

	LPTEXTURENODE NFTextureManager::FindTextureFile(const WCHAR* fileName)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		std::map<DWORD, TEXTURENODE>::iterator obj = m_mapTexture.begin();
		std::map<DWORD, TEXTURENODE>::iterator end = m_mapTexture.end();

		while(obj != end)
		{
			if(obj->second.Compare(fileName))
			{
				return &obj->second;
			}
			++obj;
		}

		return NULL;
	}

	DWORD NFTextureManager::CreateTexture(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPD3DTEXTURE* pTextureOut)
	{
		LPTEXTURENODE pNode = FindTextureFile(szFileName);
		if(pNode) 
		{
			Nave::NFSyncLock Sync(&m_Lock);
			++pNode->iRefCount;

			*pTextureOut = pNode->pTexture;
			return pNode->dwIndex;
		}

		LPD3DTEXTURE pTexture;
		NFTextureLoader::CreateTexture(pd3dDevice, szFileName, &pTexture);

		DWORD index = GetIndex();
		m_mapTexture[index].Init(index, szFileName, pTexture);

		*pTextureOut = pTexture;
		return index;
	}

	DWORD NFTextureManager::CreateTexture(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, BOOL bUseTransparency, D3DCOLOR TransparencyColor, LPD3DTEXTURE* pTextureOut)
	{
		LPTEXTURENODE pNode = FindTextureFile(szFileName);
		if(pNode) 
		{
			Nave::NFSyncLock Sync(&m_Lock);
			++pNode->iRefCount;

			*pTextureOut = pNode->pTexture;
			return pNode->dwIndex;
		}

		LPD3DTEXTURE pTexture;
		NFTextureLoader::CreateTextureEx(pd3dDevice, szFileName, &pTexture, bUseTransparency, TransparencyColor);

		DWORD index = GetIndex();
		m_mapTexture[index].Init(index, szFileName, pTexture);

		*pTextureOut = pTexture;
		return index;
	}

	DWORD NFTextureManager::CreateTextureMemory(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPCVOID pSrcData, UINT SrcDataSize, LPD3DTEXTURE* pTextureOut)
	{
		LPTEXTURENODE pNode = FindTextureFile(szFileName);
		if(pNode) 
		{
			Nave::NFSyncLock Sync(&m_Lock);
			++pNode->iRefCount;

			*pTextureOut = pNode->pTexture;
			return pNode->dwIndex;
		}

		LPD3DTEXTURE pTexture;
		NFTextureLoader::CreateTextureMemory(pd3dDevice, pSrcData, SrcDataSize, &pTexture);

		DWORD index = GetIndex();
		m_mapTexture[index].Init(index, szFileName, pTexture);

		*pTextureOut = pTexture;
		return index;
	}

	DWORD NFTextureManager::CreateTextureMemory(LPD3DDEVICE pd3dDevice, const WCHAR* szFileName, LPCVOID pSrcData, UINT SrcDataSize, BOOL bUseTransparency, D3DCOLOR TransparencyColor, LPD3DTEXTURE* pTextureOut)
	{
		LPTEXTURENODE pNode = FindTextureFile(szFileName);
		if(pNode) 
		{
			Nave::NFSyncLock Sync(&m_Lock);
			++pNode->iRefCount;

			*pTextureOut = pNode->pTexture;
			return pNode->dwIndex;
		}

		LPD3DTEXTURE pTexture;
		NFTextureLoader::CreateTextureMemoryEx(pd3dDevice, pSrcData, SrcDataSize, &pTexture, bUseTransparency, TransparencyColor);

		DWORD index = GetIndex();
		m_mapTexture[index].Init(index, szFileName, pTexture);

		*pTextureOut = pTexture;
		return index;
	}

	DWORD NFTextureManager::CreateRenderTexture(LPD3DDEVICE pd3dDevice, int Width, int Height, LPD3DTEXTURE* pTextureOut)
	{
		LPD3DTEXTURE pTexture;
		D3DXCreateTexture(pd3dDevice, Width, Height, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture);

		WCHAR strID[32];
		_swprintf(strID, L"%p", pTexture);

		DWORD index = GetIndex();
		m_mapTexture[index].Init(index, strID, pTexture);

		*pTextureOut = pTexture;
		return index;
	}

	DWORD NFTextureManager::CreateTexture(LPD3DDEVICE pd3dDevice, int Width, int Height, D3DFORMAT d3dFormat, LPD3DTEXTURE* pTextureOut)
	{
		LPD3DTEXTURE pTexture;
		D3DXCreateTexture( pd3dDevice, Width, Height, 0, 0L, d3dFormat, D3DPOOL_MANAGED, &pTexture );
	
		WCHAR strID[32];
		_swprintf(strID, L"%p", pTexture);

		DWORD index = GetIndex();
		m_mapTexture[index].Init(index, strID, pTexture);

		*pTextureOut = pTexture;
		return index;
	}

	DWORD NFTextureManager::CreateColorTexture( LPD3DDEVICE pd3dDevice, D3DCOLOR dcColor, LPD3DTEXTURE* pTextureOut)
	{
		LPD3DTEXTURE pTexture;
		NFTextureLoader::CreateColorTexture( pd3dDevice, dcColor, &pTexture);
	
		WCHAR strID[32];
		_swprintf(strID, L"%p", pTexture);

		DWORD index = GetIndex();
		m_mapTexture[index].Init(index, strID, pTexture);

		*pTextureOut = pTexture;
		return index;
	}

}