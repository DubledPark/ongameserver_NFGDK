/** 
 *  @file		NFSmartPtr.h
 *  @brief		스마트포인터 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <windows.h>
#include <stdio.h>
#include <assert.h>

/*
// 사용법..
// 주의 : NFObject를 상속받은 객체와 NFAutoreleasePool을 사용할경우
// NFObject를 상속받은 객체는 절대 NFSmartPtr로 사용해서는 안된다.

#include "NFSmartPtr.h"

class Test
{
public:
	Test()
	{
		printf("Test::Test()\n");
	}
	~Test()
	{
		printf("Test::~Test()\n");
	}
};

INT _tmain(INT argc, WCHAR* argv[])
{
	printf("new Test\n");

	NFSmartPtr<Test> a = new Test();
	NFSmartPtr<Test> b = new Test();
	a = new Test();
	a = b;

	printf("return main\n");
	return 0;
}
*/

#pragma warning( disable : 4284 )
#pragma warning( disable : 4786 )

namespace Nave {

	///	Representation class just for reference counting
	template<class T>
	class NFRefCountRep 
	{
	//	Constructors and destructor
	public:
		NFRefCountRep( const T* ptr );
		~NFRefCountRep();

	//	Operations
	public:
		LONG	IncrRefCount();
		LONG	DecrRefCount();

		T*		GetPointer() const;
		T*		GetRealPointer() const;

		BOOL	IsNull() const;

	//	Implementation
	private:
		T*		m_pRealPtr;
		LONG	m_lCounter;
	};

	template<class T>
	NFRefCountRep<T>::NFRefCountRep( const T* ptr )	: m_pRealPtr( (T*)ptr ), m_lCounter( 0 ) 
	{
	}

	template<class T>
	NFRefCountRep<T>::~NFRefCountRep() 
	{
		assert( m_lCounter <= 0 );
		delete	m_pRealPtr;
	}

	template<class T>
	LONG	NFRefCountRep<T>::IncrRefCount() 
	{
		return	::InterlockedIncrement( &m_lCounter );
	}

	template<class T>
	LONG	NFRefCountRep<T>::DecrRefCount() 
	{
		return	::InterlockedDecrement( &m_lCounter );
	}

	template<class T>
	T*	NFRefCountRep<T>::GetPointer() const 
	{
		return	m_pRealPtr;
	}

	template<class T>
	T*	NFRefCountRep<T>::GetRealPointer() const 
	{
		return	m_pRealPtr;
	}

	template<class T>
	BOOL	NFRefCountRep<T>::IsNull() const 
	{
		return m_pRealPtr == NULL;
	}


	///	The NFSmartPtr class
	class	NFSmartPtrBase {
	public:
		NFSmartPtrBase() : m_pRep( NULL )
		{
		};

		void*	m_pRep;
	};

	//template<class T, class REP, class ACCESS = T*>
	//class NFSmartPtr : public NFSmartPtrBase {
	template<class T, class REP = NFRefCountRep<T>, class ACCESS = T*>
	class	NFSmartPtr : public NFSmartPtrBase 
	{
	//	Constructors and destructor
	public:
		//	Default constructor and destructor
		NFSmartPtr();
		~NFSmartPtr();

		//	Copy constructor
		NFSmartPtr( const NFSmartPtr& ptr );

		//	Other constructors
		NFSmartPtr( const T* ptr );
		NFSmartPtr( const NFSmartPtrBase& ptr );

	//	Assignment Operators
	public:
		NFSmartPtr& operator = ( const NFSmartPtr& ptr );
		NFSmartPtr& operator = ( const T* ptr );
		NFSmartPtr& operator = ( const NFSmartPtrBase& ptr );

	//	Operators
	public:
		ACCESS	operator -> ();
		T&		operator * ();

		//	Casting operator
		operator T* ();

		//	Comparison Operators
		BOOL	operator == ( const NFSmartPtrBase& ptr );
		BOOL	operator == ( const T* ptr );
		BOOL	operator != ( const NFSmartPtrBase& ptr );
		BOOL	operator != ( const T* ptr );

	//	Attributes
	public:
		BOOL	IsNull() const;
		LONG	GetRefCount() const;
		REP*	GetRepPtr() const;

	//	Helper methods
	private:
		void	CopyFrom( const NFSmartPtrBase& ptr );
		void	CopyFrom( const T* ptr );

	//	Implementation
	private:
		void	IncrRefCount();
		void	DecrRefCount();
	};
			
	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>::NFSmartPtr()
	{
	}

	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>::~NFSmartPtr()
	{
		DecrRefCount();
	}

	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>::NFSmartPtr( const NFSmartPtr& ptr )
	{
		CopyFrom( ptr );
	}

	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>::NFSmartPtr( const T* ptr )
	{
		CopyFrom( ptr );
	}

	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>::NFSmartPtr( const NFSmartPtrBase& ptr )
	{
		CopyFrom( ptr );
	}

	template<class T, class REP, class ACCESS>
	void	NFSmartPtr<T,REP,ACCESS>::CopyFrom( const NFSmartPtrBase& ptr )
	{
		if( m_pRep != ptr.m_pRep ) {
			DecrRefCount();
			m_pRep = ptr.m_pRep;
			IncrRefCount();
		}
	}

	template<class T, class REP, class ACCESS>
	void	NFSmartPtr<T,REP,ACCESS>::CopyFrom( const T* ptr )
	{
		DecrRefCount();
		m_pRep = (ptr != NULL) ? new REP( ptr ) : NULL;
		IncrRefCount();
	}

	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>& NFSmartPtr<T,REP,ACCESS>::operator = ( const NFSmartPtr& ptr ) 
	{
		CopyFrom( ptr );
		return	*this;
	}

	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>& NFSmartPtr<T,REP,ACCESS>::operator = ( const T* ptr ) 
	{
		CopyFrom( ptr );
		return	*this;
	}

	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>& NFSmartPtr<T,REP,ACCESS>::operator = ( const NFSmartPtrBase& ptr ) 
	{
		CopyFrom( ptr );
		return	*this;
	}

	template<class T, class REP, class ACCESS>
	ACCESS	NFSmartPtr<T,REP,ACCESS>::operator -> () 
	{
		assert( ! IsNull() );
		return GetRepPtr()->GetPointer();
	}

	template<class T, class REP, class ACCESS>
	T& NFSmartPtr<T,REP,ACCESS>::operator * () 
	{
		assert( ! IsNull() );
		return *(GetRepPtr()->GetRealPointer());
	}

	template<class T, class REP, class ACCESS>
	NFSmartPtr<T,REP,ACCESS>::operator T* () 
	{
		return	( IsNull() ) ? NULL : GetRepPtr()->GetRealPointer();
	}

	template<class T, class REP, class ACCESS>
	BOOL	NFSmartPtr<T,REP,ACCESS>::operator == ( const NFSmartPtrBase& ptr ) 
	{
		return	m_pRep == ptr.m_pRep;
	}

	template<class T, class REP, class ACCESS>
	BOOL	NFSmartPtr<T,REP,ACCESS>::operator == ( const T* ptr ) 
	{
		if( ! IsNull() ) {
			return	GetRepPtr()->GetRealPointer() == ptr;
		}
		return	ptr == NULL;
	}

	template<class T, class REP, class ACCESS>
	BOOL NFSmartPtr<T,REP,ACCESS>::operator != ( const NFSmartPtrBase& ptr ) 
	{
		return	m_pRep != ptr.m_pRep;
	}

	template<class T, class REP, class ACCESS>
	BOOL NFSmartPtr<T,REP,ACCESS>::operator != ( const T* ptr ) 
	{
		return	! (operator ==( ptr ));
	}

	template<class T, class REP, class ACCESS>
	BOOL	NFSmartPtr<T,REP,ACCESS>::IsNull() const 
	{
		return	m_pRep == NULL;
	}

	template<class T, class REP, class ACCESS>
	LONG	NFSmartPtr<T,REP,ACCESS>::GetRefCount() const 
	{
		assert( ! IsNull() );
		return	GetRepPtr()->m_lCounter;
	}

	template<class T, class REP, class ACCESS>
	REP*	NFSmartPtr<T,REP,ACCESS>::GetRepPtr() const 
	{
		return	(REP*)m_pRep;
	}

	template<class T, class REP, class ACCESS>
	void	NFSmartPtr<T,REP,ACCESS>::IncrRefCount() 
	{
		if( ! IsNull() ) 
		{
			GetRepPtr()->IncrRefCount();
		}
	}

	template<class T, class REP, class ACCESS>
	void	NFSmartPtr<T,REP,ACCESS>::DecrRefCount() 
	{
		if( ! IsNull() ) 
		{
			if( GetRepPtr()->DecrRefCount() <= 0 ) 
			{
				REP*	rep = (REP*)m_pRep;
				delete	rep;
			}
			m_pRep = NULL;
		}
	}

}