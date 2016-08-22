#include "Nave.h"
#include "NFAutoreleasePool.h"
#include "NFObject.h"

namespace Nave { 

	NFAutoreleasePool::NFAutoreleasePool(void) : m_bRun(TRUE), m_dwSize(0)
	{
	}

	NFAutoreleasePool::~NFAutoreleasePool(void)
	{
		m_bRun = FALSE;
		// AutoreleasePool¿Ã º“∏Í«ﬂ¥Ÿ.
		IterAutoPool obj = m_AutoPool.begin();
		IterAutoPool end = m_AutoPool.end();

		while(obj != end)
		{
			NFObject* pT = reinterpret_cast<NFObject*>(*obj);
			_DELETE(pT);

			++obj;
		}

		m_AutoPool.clear();
	}

	void NFAutoreleasePool::Register(DWORD id, const void* p, DWORD size)
	{
		if(!m_bRun)
			return;

		m_dwSize += size;
		m_AutoPool.push_back(id);
	}

	void NFAutoreleasePool::UnRegister(DWORD id, DWORD size)
	{
		if(!m_bRun)
			return;

		// AutoreleasePool¿Ã º“∏Í«ﬂ¥Ÿ.
		IterAutoPool obj = m_AutoPool.begin();
		IterAutoPool end = m_AutoPool.end();
		while(obj != end)
		{
			if((*obj) == id)
			{
				m_dwSize -= size;
				m_AutoPool.erase(obj);
				return;
			}
			++obj;
		}
	}

	const INT	NFAutoreleasePool::GetCount() const 
	{
		return (INT)m_AutoPool.size();
	}

}