/** 
 *  @file		NFAutoreleasePool.h
 *  @brief		AutoRelease Pool 
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <list>

namespace Nave { 
	/** 
	 *  @class        NFAutoreleasePool
	 *  @brief        NFObject를 상속받은 객체를 자동으로 Release 하기 위한 클래스
	 *  @remarks      Nave::NFAutoreleasePool pool = new Nave::NFAutoreleasePool();	\r\n
	 * 				  NFObject* pControl = new Nave::NFObject;						\r\n
	 * 				  pControl = new Nave::UIControl;								\r\n
	 * 				  _DELETE(pool);												\r\n
	 *                
	 *  @warning	NFObject를 상속받지 않은 객체는 이 클래스에서 관리하지 못한다.	\r\n
	 * 				1. NFObject를 상속받은 객체를 NFSmartPtr과 사용할 경우 종료시	\r\n
	 * 				   삭제 충돌이 날수 있다. AutoreleasePool이 삭제되기전에		\r\n
	 * 				   NFSmartPtr 객체가 초기화 되면 이상이없다.					\r\n
	 *																				\r\n
	 * 				2. AutoreleasePool 을 사용할경우 속도저하가 있다.				\r\n
	 * 				   2048의 메모리를 갖는 class 를 1만개 생성햇을때				\r\n
	 * 				   AutoreleasePool을 사용하면 1만개의 객체 생성시 2.6초			\r\n
	 * 				   하지만 AutoreleasePool을 사용하지 않으면 1만개 생성시		\r\n
	 * 				   1.6초로 속도에 확실한 저하가 발생하고 있다.					\r\n
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	class NFAutoreleasePool : public NFSingleton<NFAutoreleasePool>
	{
	public:
		/// NFAutoreleasePool 생성자
		NFAutoreleasePool();
		/// NFAutoreleasePool 소멸자
		~NFAutoreleasePool();

	public:
		/**
		 * @brief	Pool에 등록한다.
		 * @param id	고유아이디
		 * @param p		메모리 포인터
		 * @param size	사이즈
		 * @warning		NFObject의 new 오퍼레이터에서 자동으로 호출하는것이지 직접 호출하는게 아니다
		 */
		void	Register(DWORD id, const void* p, DWORD size);

		/**
		 * @brief	Pool에서 해제한다.
		 * @param id	고유아이디
		 * @param size	사이즈
		 * @warning		NFObject의 delete 오퍼레이터에서 자동으로 호출하는것이지 직접 호출하는게 아니다
		 */
		void	UnRegister(DWORD id, DWORD size);

		/**
		 * @brief	Pool에 등록된 아이템의 개수를 구한다.
		 * @return  총 아이템 개수
		 */
		const INT		GetCount() const;

	private:
		/// 생성된 메모리의 총사이즈
		DWORD				m_dwSize;
		/// AutoreleasePool의 사용여부
		BOOL				m_bRun;

		/// 생성된 객체의 리스트
		std::list<DWORD>	m_AutoPool;

		/// 이터레이터
		typedef std::list<DWORD>::iterator	IterAutoPool;
		/// const 이터레이터
		typedef std::list<DWORD>::const_iterator	ConstIterAutoPool; 
	};

}