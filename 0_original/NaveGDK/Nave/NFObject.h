/** 
 *  @file		NFObject.h
 *  @brief		Object Super 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave {

	/** 
	 *  @class       NFObject 
	 *  @brief		 Object의 Super 클래스
	 *  @remarks     AutoreleasePool에서 관리되는 객체를 생성하기	\r\n
	 *               위해서는 NFObject를 상속받으면 된다.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFObject
	{
	public:
		/// NFObject 생성자
		NFObject();
		/// NFObject 소멸자
		virtual ~NFObject();

	public:
		/// new 오퍼레이터 클래스 
		static void * operator new(size_t size);
		/// delete 오퍼레이터 클래스 
		static void operator delete(void *p, size_t size);

		/// 객체의 정보를 리턴한다 보통 클래스 이름등 공유의 문자열을 리턴한다.
	    virtual const char* ToString() { return "NFObject"; }

		/// 오브젝트의 고유 아이디이다.
		inline DWORD GetHashCode() { return m_dwHash; }

		/// 같은 객체인지 (Hash코드를 가지고 비교한다)
        virtual bool Equals(NFObject& obj);

		/// 두개의 객체가 같은 객체인지 (Hash코드를 가지고 비교한다)
		static bool Equals(NFObject& objA, NFObject& objB);

	private:
		/// Object의 고유 Hash코드 (메모리가 생성된 메모리포인터)
		DWORD	m_dwHash;
	};
}

