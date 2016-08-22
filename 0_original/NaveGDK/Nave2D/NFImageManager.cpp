#include <NaveFramework/NaveFramework.h>
#include <NaveFramework/NFAdapter.h>
#include <NaveFramework/NFVideoDX9.h>

#include "NFImageManager.h"

using namespace NaveFramework;

namespace Nave2D {

	NFImageManager::NFImageManager(void)
	{
	}

	NFImageManager::~NFImageManager(void)
	{
		ReleaseAll();
	}

	VOID NFImageManager::ReleaseAll()
	{
		Nave::NFSyncLock Sync(&m_Lock);

		ImageIter obj = m_ImageMap.begin();
		ImageIter end = m_ImageMap.end();

		while(obj != end)
		{
			_DELETE(obj->second);

			++obj;
		}

		m_ImageMap.clear();
	}

	NFImage* NFImageManager::CreateImage(DWORD dwID)
	{
		NFImage* pImage = FindImage(dwID);
		
		if(pImage == NULL)
		{
			pImage = new NFImage();
			m_ImageMap[dwID] = pImage;
		}

		return pImage;
	}

	NFImage* NFImageManager::FindImage(DWORD dwID)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		ImageIter obj = m_ImageMap.find(dwID);

		if(obj == m_ImageMap.end())
			return NULL;

		return obj->second;
	}

	NFImage* NFImageManager::AddFromFile(DWORD dwID, const WCHAR* ImageFile)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		NFImage* pImage = CreateImage(dwID);

		if(pImage == NULL)
			return NULL;

		pImage->LoadFile(NFVideoDX9::GetInstance().Get3DDevice(), ImageFile);
		return pImage;
	}

	NFImage* NFImageManager::AddFromMemory(DWORD dwID, const WCHAR* TextureID, char* FilePoint, DWORD FileLen)
	{
		Nave::NFSyncLock Sync(&m_Lock);

		NFImage* pImage = CreateImage(dwID);

		if(pImage == NULL)
			return NULL;

		pImage->LoadMemory(NFVideoDX9::GetInstance().Get3DDevice(), TextureID, FilePoint, FileLen);
		return pImage;
	}

}