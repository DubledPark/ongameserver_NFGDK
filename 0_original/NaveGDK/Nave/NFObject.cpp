#include "Nave.h"
#include "NFObject.h"
#include "NFAutoreleasePool.h"

namespace Nave {

	NFObject::NFObject()
	{
		m_dwHash = reinterpret_cast<DWORD>(this);
	}

	NFObject::~NFObject()
	{
	}

	void * NFObject::operator new(size_t size)
	{
		void* p = ::operator new(size);

		if(Nave::NFAutoreleasePool::IsAlive())
		{
			DWORD dwThis = reinterpret_cast<DWORD>(p);
			Nave::NFAutoreleasePool::GetInstance().Register(dwThis, p, size);
		}

		return p;
	}

	void NFObject::operator delete(void *p, size_t size)
	{
		if(Nave::NFAutoreleasePool::IsAlive())
		{
			DWORD dwThis = reinterpret_cast<DWORD>(p);
			Nave::NFAutoreleasePool::GetInstance().UnRegister(dwThis, size);
		}

		return ::operator delete(p);
	}

	bool NFObject::Equals(NFObject& obj)
	{
		if(GetHashCode()==obj.GetHashCode())
			return true;

		return false;
	}

    bool NFObject::Equals(NFObject& objA, NFObject& objB)
	{
		if(objA.GetHashCode() == objB.GetHashCode())
			return true;

		return false;
	}

}